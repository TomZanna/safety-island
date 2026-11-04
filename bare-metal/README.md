# imx943-flow — bare metal safety firmware + A55 Linux transport

Re-implementation for a bare metal i.MX943 safety target of the architecture
described in `../sequence diag.md` and `../pseudo_code.md`.

The prototype in `../src` runs on a Linux PC with POSIX threads. This folder
contains the target-oriented version:

- **M7 and M33** run the same bare metal safety code. Each core has its own
  FSoE master stack, its own MU sequence state and its own SHMEM TX channels.
- **A55** runs Linux and a small kernel driver that maps the shared memory
  mailbox and exposes the already protected FSoE PDUs to the fieldbus process.
- The rt-labs FSoE library is taken from `../bare-metal-code/fsoe`, compiled
  file by file and linked as a separate static library. `FSoEappl/` is an
  example application and is **not** compiled.

The host test (`make check`) runs the two safety cores and an emulated FSoE
slave in one process to validate the architecture before porting to silicon.

---

## 1. What is protected where

The FSoE PDU is a black channel: it already has the FSoE CRC, FSoE sequence
number and FSoE watchdog. **No second CRC is added to the PDU payload.**

The mailbox metadata is a different matter. The A55 needs `len`, the frame
counter and the publication index to assemble the two halves correctly, so the
slot header has a CRC-16 over those fields and a seqlock protects against torn
reads. The MU cross-check messages between M7 and M33 are not end-to-end
protected by FSoE either, so they carry their own protocol id, source,
destination, connection tag, independent sequence number and CRC-32.

| Path | Protection |
|---|---|
| FSoE master ↔ slave (through A55) | FSoE CRC, FSoE sequence, FSoE watchdog |
| Slot metadata in SHMEM | seqlock + CRC-16 over `len`, `flags`, `fc` |
| MU messages M7 ↔ M33 | protocol id, routing, independent sequence, CRC-32 |
| PDU payload in SHMEM | none added: it is the FSoE black channel |

---

## 2. Single source of truth for the A55 ↔ safety core interface

`include/flow_shm_api.h` is included by:

- the bare metal safety firmware,
- the Linux A55 driver,
- the host test.

It defines the exact same packed structures for both worlds, with only
`flow_u8`/`flow_u16`/`flow_u32` fields. There is no serialization and no
runtime code generation:

```c
typedef struct FLOW_PACKED flow_shm_header
{
   flow_u32 magic;
   flow_u32 protocol_id;
   flow_u32 version;
   flow_u32 layout_hash;
   flow_u32 conn_count;
   flow_u32 banks;
   flow_u32 slot_size;
   flow_u32 header_size;
   flow_u32 state;
   flow_u32 heartbeat[FLOW_CORE_COUNT];
   flow_u32 reserved[5];
} flow_shm_header_t;
```

The driver checks `magic`, `protocol_id`, `version`, `layout_hash` and the
geometry at probe time. A mismatch is reported as `-EPROTO`; a mailbox not yet
initialised by the safety cores is reported as `-EPROBE_DEFER`.

### Why not protobuf?

Protobuf is a serialization format, not a safe black-channel protocol. It has
no CRC, no sequence number, no anti-replay and no fixed maximum size guarantee
unless additional mechanisms are added. It normally introduces dynamic
allocation, varints, unknown fields and a generator that would need tool
qualification for SIL 3.

For a memory-mapped mailbox between a non-safe Linux core and two safety cores,
a fixed packed layout plus explicit versioning is simpler, reviewable and
verifiable:

- compile-time `_Static_assert` on sizes and offsets;
- `FLOW_SHM_MAGIC`, `FLOW_SHM_PROTOCOL_ID`, `FLOW_SHM_VERSION`,
  `FLOW_SHM_LAYOUT_HASH`;
- seqlock for every bank;
- monotonic publication counter and `MIN(count_m7, count_m33)` on the A55;
- no pointers, no `size_t`, no enums, no bit fields in shared structures;
- `tools/abi_check.sh` checks the geometry on the host and, when available,
  with a 32-bit compiler (the A55 is 64-bit, the safety cores are 32-bit).

If the layout changes, increment `FLOW_SHM_VERSION` and
`FLOW_SHM_LAYOUT_HASH`. The runtime check then refuses a stale driver or
firmware instead of misinterpreting the mailbox.

---

## 3. Directory map

```
bare-metal/
├── include/
│   ├── flow_types.h        fixed width types, kernel/freestanding abstraction
│   ├── flow_config.h       core id, sizes, timeouts, FLOW_DEBUG
│   ├── flow_shm_api.h      SINGLE SOURCE OF TRUTH: packed mailbox layout
│   ├── flow_shm.h          seqlock, stage/commit/read, split/assemble
│   ├── flow_crc.h          CRC-32 for MU, CRC-16 for slot headers
│   ├── flow_mem.h          freestanding memcpy/memset/memcmp
│   ├── flow_mu.h           MU message format and cross-check API
│   ├── flow_safe_logic.h   pure safety function
│   ├── flow_safe.h         safe core state and callbacks
│   ├── flow_platform.h     tick, time, TRNG, MU, fatal, watchdog hooks
│   └── flow_debug.h        FLOW_LOG -> no-op when FLOW_DEBUG=0
├── common/                 portable code, no OS, no heap
├── safe/                   safe cycle, FSoE callbacks, PAL
├── port/fsoeport_macros.h  bare metal PAL macros
├── platform/host/          POSIX test platform
├── platform/target/        i.MX943 skeleton + explicit TODOs
├── app/main.c              bare metal entry point of a safety core
├── a55/                    Linux driver + portable A55 merge + uapi
├── tests/                  host validation and ABI check
└── tools/abi_check.sh
```

---

## 4. Safe cycle (same as the sequence diagram)

`flow_safe_step()` executes:

1. **Latch RX** — every core reads the newest frame from `shm->rx[k]` using its
   own `rx_count`, copies it to local RAM and checks the slot header CRC.
2. **FSoE sync** — `fsoemaster_sync_with_slave()`.
   - `fsoeapp_recv()` returns the latched frame;
   - `fsoeapp_send()` performs the MU cross-check of the **complete PDU**. If
     the two cores generated the same PDU, each writes **its own half** into
     its own SHMEM channel (M7 = CRC bytes, M33 = command/data/ConnID), reads
     its half back, verifies it and publishes the frame;
   - if the PDUs differ: fail-safe and
     `fsoemaster_set_reset_request_flag()`.
3. **Cross-check SafeInputs** — exchange raw inputs, status, `tx_index`,
   `rx_index` and the identity of the latched RX frame. A mismatch forces
   SafeInputs to 0 and clears process-data sending. A counter divergence also
   requests a reset and re-aligns both counters to `MIN(own, peer)`.
4. **Safe logic + cross-check outputs** — `flow_safe_logic_step()` is pure
   `(inputs, state) -> (outputs, state)`. The complete output bytes and the
   logic state bits are exchanged. Equal: enable sending. Divergent: outputs
   0, disable sending, request reset.

Session ID: each core takes a 16-bit hardware random value and the MU exchange
returns the peer value; the Session ID is the **sum** of the two. A TRNG or MU
failure is fail-safe; there is no pseudo-random fallback on the target.

The external tick, the monotonic clock, the TRNG, the MU and the external
watchdog are platform hooks. `flow_platform_tick_wait()` is the only place the
cycle is advanced:

```c
for (;;)
{
   if (flow_platform_tick_wait ())
   {
      flow_safe_step (&safe_core);
      flow_platform_watchdog_kick ();
   }
}
```

`platform/target/flow_platform_target.c` contains the explicit TODOs for the
hardware timer ISR, the independent time source, the TRNG, the MU registers and
the external watchdog. Until they are implemented the target functions fail
closed.


---

## 5. Debug and production builds

`FLOW_DEBUG=0` in the target build. In that configuration:

- `FLOW_LOG()` expands to nothing;
- no `printf`, `stdio`, `snprintf`, fault injection or test hooks are linked;
- `FLOW_FREESTANDING=1` provides `memcpy`/`memset`/`memcmp` from
  `common/flow_mem.c` (no libc dependency);
- `FSOEPORT_ASSERT()` calls `flow_platform_fatal()`, which disables interrupts
  and waits for the external watchdog; it never prints or calls `assert()`.

The host test is built with `FLOW_DEBUG=1` and fault injection enabled. The
production safety build does not contain any of that code.

---

## 6. Build and test

Prerequisite: `../bare-metal-code/fsoe` (the rt-labs library copied by the
user). `FSoEappl/` is not used.

```
make host        # build the POSIX validation test
make run         # build and run the host validation
make check       # ABI check + fault injection matrix
make abi         # layout check only
make m7          # cross build M7 (arm-none-eabi-gcc, if installed)
make m33         # cross build M33
make a55 KDIR=/lib/modules/$(uname -r)/build
make clean
```

The FSoE library is always a separate archive (`build/libfsoe-host.a`,
`build/libfsoe-m7.a`, `build/libfsoe-m33.a`). Every component is compiled on
its own and the final link happens at the end.

Host result on this container:

```
  [PASS] clean run
  [PASS] divergent egress PDU (M33)
  [PASS] divergent decoded SafeInputs (M33)
  [PASS] divergent SafeOutputs (M33)
  [PASS] Session ID divergence (M33)
  [PASS] SHMEM tx counter divergence (M33 stall)
```

The same clean run was executed with ASan+UBSan and reported no error.

`tools/abi_check.sh` output:

```
ABI CHECK host: header=64 slot=24 channel=88 shm=328 tx=64 rx=240
ABI CHECK 32-bit: skipped (no -m32 toolchain)
```

The 32-bit line is skipped in this container because multilib is not
installed; the packed structures use only 8/16/32-bit fields and the static
asserts enforce the same offsets on both worlds.

---

## 7. A55 Linux driver

The A55 driver is a **transport**, not a safety component. It:

- maps the reserved SHMEM region (`devm_ioremap`, non cached);
- validates the mailbox header and exposes `/dev/flow-a55`;
- implements `GET_ABI`, `GET_TX`, `PUT_RX`, `GET_STATUS` ioctls;
- never modifies the PDU and never repairs an invalid slot;
- does not expose `mmap`: user space cannot bypass the validation;
- allows one open at a time.

The EtherCAT/FSoE fieldbus stack remains in user space: it calls `GET_TX` at
the bus cycle, sends the PDU, receives the reply and calls `PUT_RX`. See
`a55/README.md` for the device tree example and a userspace sketch.

---

## 8. Target TODOs (already marked in the code)

- `TODO(target)`: hardware safe timer / sync tick, absolute deadline, jitter.
- `TODO(target)`: independent monotonic timer for the FSoE watchdog (avoid a
  common cause with the tick).
- `TODO(target)`: hardware TRNG and health tests; no pseudo-random fallback.
- `TODO(target)`: i.MX943 MU/MU2 register map, doorbell, barriers, timeout.
- `TODO(target)`: MPU/TRDC, non-cacheable SHMEM, ECC initialisation.
- `TODO(target)`: linker script, vector table, `.data`/`.bss` startup, clock,
  external watchdog and M7/M33 boot order / SHMEM init owner.
- `TODO(a55-dt)`: device tree node, reserved-memory carve-out, cache
  attributes, interrupt/poll strategy.
- `TODO(fieldbus)`: EtherCAT user space integration and alignment between bus
  cycle and safe cycle.
- `TODO(safety)`: FMEDA, WCET, stack usage, MISRA, tool qualification, target
  integration tests.

---

## 9. Limits

- The kernel module cannot be built in this container (no kernel headers);
  it is provided with `Kbuild` and must be built against the target kernel.
- `arm-none-eabi-gcc` is not installed here, so the M7/M33 link is not executed;
  the target sources are syntax-checked with the host compiler.
- The target platform functions are skeletons: TRNG and MU return fail-safe
  values until implemented. That is deliberate: the code fails closed.
- Linux and the A55 are **not** safety components. The safety decision remains
  in M7/M33 and in the FSoE end-to-end protection.

