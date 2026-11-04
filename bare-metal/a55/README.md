# A55 Linux transport driver

`flow_a55_drv.c` is a small platform driver that maps the shared memory
mailbox used by the M7/M33 safety cores and exposes the two directions through
a char device. It is **not** a safety component.

## Device tree

The actual addresses and cache attributes belong to the board integration
(TODO). The driver matches a memory resource:

```dts
reserved-memory {
   flow_shm: flow-shm@... {
      compatible = "fsl,imx943-flow-shm";
      reg = <0x0 0x... 0x0 0x1000>;
      no-map;
   };
};
```

`no-map` + `devm_ioremap()` gives a non-cached mapping. The same physical
region must be mapped non-cacheable by the M7/M33 MPU; a cached A55 mapping
would not be coherent with the safety cores.

## Build

```
make -C /lib/modules/$(uname -r)/build M=$(pwd) modules
```

or from the parent directory:

```
make a55 KDIR=/lib/modules/$(uname -r)/build
```

## ABI

`flow_a55_uapi.h` defines the ioctl ABI. The structures are fixed width and
versioned:

- `FLOW_A55_IOCTL_GET_ABI` — driver ABI version, mailbox protocol id, layout
  hash, connection count, slot size and maximum PDU size;
- `FLOW_A55_IOCTL_GET_TX` — last frame written by **both** safety cores (the
  driver computes `MIN(count_m7, count_m33)` and interleaves the two halves);
- `FLOW_A55_IOCTL_PUT_RX` — publish a PDU received from the fieldbus into the
  RX mailbox;
- `FLOW_A55_IOCTL_GET_STATUS` — header state, heartbeats, counters and the
  number of structurally invalid frames.

`GET_TX` returns `-EAGAIN` when no new frame is available and `-EBADMSG` when
the selected slots are structurally invalid. The driver never repairs a PDU:
the FSoE slave is the component that performs the end-to-end check.

## User space sketch

```c
int fd = open("/dev/flow-a55", O_RDWR);
struct flow_a55_abi abi;

ioctl(fd, FLOW_A55_IOCTL_GET_ABI, &abi);

for (;;) {
   struct flow_a55_frame_u frame;
   struct flow_a55_frame_u reply;

   if (ioctl(fd, FLOW_A55_IOCTL_GET_TX, &frame) == 0) {
      fieldbus_send(frame.pdu, frame.len);
   }

   if (fieldbus_receive(reply.pdu, &reply.len)) {
      reply.conn = 0;
      ioctl(fd, FLOW_A55_IOCTL_PUT_RX, &reply);
   }
}
```

## Notes and TODOs

- Only one user space process may open the device.
- No `mmap`: it would let user space bypass the mailbox validation.
- `GET_TX` currently uses connection 0. For multiple FSoE connections, add the
  connection index to the ioctl input and select `shm->tx[conn][...]`.
- The cycle is polled from user space. `TODO(a55-irq)`: hardware mailbox
  interrupt or `poll()` support if the fieldbus cycle needs it.
- `TODO(a55-dt)`: reserved memory carve-out, MPU/TRDC configuration, cache
  attributes, real SoC addresses.
- The Linux kernel is not a safety component: all safety decisions remain in
  the two cores and in the FSoE end-to-end protection.
