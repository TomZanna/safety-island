#import "../lib/style.typ": *
= Appendice A — Layout delle strutture

Tutto ciò che attraversa la memoria condivisa è `packed`, con soli campi a 8/16/32 bit e offset verificati a
compile time (`FLOW_STATIC_ASSERT`). Se una di queste taglie cambia, il build si ferma invece di rompersi a
runtime.

#si-table(
  columns: (1.8fr, 1fr, 1fr, 1fr),
  header: ([Struttura], [Dimensione], [Dove], [Note]),
  rows: (
    ([`flow_shm_header_t`], [64 B], [mailbox], [`magic` per ultimo, `state = READY`]),
    ([`flow_shm_slot_t`], [24 B], [mailbox], [CRC-16 su len/flags/fc; PDU max 11 B]),
    ([`flow_shm_channel_t`], [88 B], [mailbox], [3 banchi + seqlock + contatore]),
    ([`flow_shm_t`], [328 B], [`0x204D4000`], [1 connessione, 2 canali TX + 1 RX]),
    ([`flow_icc_slot_t`], [88 B], [`0x204D5000`], [seq/len/ciclo/CRC + messaggio MU 72 B]),
    ([`flow_icc_t`], [724 B], [`0x204D5000`], [8 slot + barrier + magic]),
    ([`flow_mu_message_t`], [72 B], [ICC], [header 36 B + payload max 48 B]),
  ),
)

= Appendice B — MU18

#si-table(
  columns: (1.4fr, 1.4fr, 1fr, 1.6fr),
  header: ([Lato], [Core], [Indirizzo], [Simbolo SDK]),
  rows: (
    ([A], [M7_1 (SAFE_B)], [`0x42B70000`], [`WAKEUP__MUI_A18__MUA`]),
    ([B], [M7_0 (SAFE_A)], [`0x42B80000`], [`WAKEUP__MUI_A18__MUB`]),
    ([-], [M33_1], [nessuno], [non esiste un lato MU18 per l'M33]),
  ),
)

- 4 registri TX (`TR[0..3]`) e 4 RX (`RR[0..3]`) da 32 bit: *16 byte per direzione*;
- stato: `TSR.TE0..TE3` (TX vuoto), `RSR.RF0..RF3` (RX pieno; la lettura di `RRn` azzera `RFn`);
- IRQ: `MU18_A_IRQn = 129`, `MU18_B_IRQn = 130` (non usate: si fa polling con timeout);
- accesso *diretto ai registri*, senza `MU_Init()`/`CLOCK_EnableClock()`: il clock è dell'SM. Se il
  registro non risponde, la strada è chiedere il clock via SCMI, non forzare il CCM.

= Appendice C — Mappa dei file del virtual safe PLC

#si-table(
  columns: (2fr, 3.1fr),
  header: ([File], [Responsabilità]),
  rows: (
    ([`flow.c`], [cicli dei tre ruoli, comandi RTT, telemetria]),
    ([`flow_config.h`], [taglie, indirizzi, tempi, ruolo (`FLOW_CORE_ID`) — niente magic number sparsi]),
    ([`flow_shm_api.h/.c`], [layout e accesso alla mailbox: seqlock, CRC-16, split/assemble]),
    ([`flow_mu.h/.c`], [messaggi di cross-check: routing, seq indipendente, CRC-32, verifica]),
    ([`flow_icc.h/.c`], [trasporto: slot in OCRAM, doorbell MU18, barrier di ciclo]),
    ([`flow_mu18.h/.c`], [accesso ai registri del proprio lato MU18, statistiche]),
    ([`flow_safe.h/.c`], [i 4 passi del diagramma, cross-check egress/inputs/logic/session]),
    ([`flow_safe_logic.h/.c`], [funzione di sicurezza pura (1oo2, discrepanza, latch, reset)]),
    ([`flow_field.h/.c`], [slave FSoE + generatore di campo deterministico + comandi]),
    ([`flow_a55.h/.c`], [merge `MIN` + interleave delle due metà, pubblicazione RX]),
    ([`flow_fsoe_app.c`], [le 5 callback dello stack, dispatch per ruolo]),
    ([`flow_selftest.h/.c`], [KAT (CRC, split/assemble), probe OCRAM, scambio ICC]),
    ([`flow_platform.h` + `flow_platform_imx94.c`], [HAL: tick, tempo DWT, RNG, log, fatal]),
    ([`port/fsoeport.c`, `port/fsoeport_macros.h`], [PAL per la libreria FSoE]),
  ),
)

= Appendice D — Comandi rapidi

```bash
# ambiente
make deps                 # toolchain + SDK (west)
make usb-status           # dispositivi USB in WSL
# build e board
make image && make run    # flash.bin + reset + uuu
make rtt                  # RTT di tutti i core; rtt-<app> per uno
make reset                # reset hardware (J-Link → BCU → monitor)
make uart                 # console del monitor SM
GDB_BATCH=1 make debug-m70   # attach non interattivo (poi monitor go)
# SM e docs
make sm-update            # ricostruisce l'SM nel Docker e aggiorna sm/
make sm-verify            # verifica che i binari in sm/ siano riproducibili
make docs                 # questo documento
```

Comandi RTT: `p i l x` sui core safe (PDU, inputs, logic, session), `s d` sul fieldbus (discrepanza, drop
frame), `?` per lo stato.

= Appendice E — Registro delle misure <sec-misure>

Run fatte su i.MX94 EVK A0 durante lo sviluppo, con l'immagine costruita da questa repo.

#si-table(
  columns: (1.1fr, 2.2fr, 2.8fr),
  header: ([Passo], [Cosa è stato misurato], [Risultato]),
  rows: (
    ([Piattaforma], [tick 10 ms, tempo DWT, selftest], [`slip 0` su >10 000 cicli; selftest `PASS` sui 3 core]),
    ([Memoria], [probe OCRAM condivisa + mailbox], [`ocram condivisa PASS`; heartbeat incrociati `100/100/100`]),
    ([MU18], [round-trip del doorbell], [`mu18 link (ping) PASS`, peer `0x5a5a0000`]),
    ([ICC], [scambio full-duplex + barrier], [`icc exchange PASS`; 10 600 cicli, `bar 1` solo all'avvio]),
    ([FSoE], [due master + slave], [`RESET→DATA`, session `0xdd22`/`0xb7d5` identici, `anom 0` su 3 900 cicli]),
    ([Uscite], [logica safe + campo], [`en 1` (0x00C3) con condizioni soddisfatte, `0x0000` altrimenti]),
    ([Fault injection], [`p` `i` `l` `x` `d`], [vedi la matrice in @sec-fault: fail-safe + riconnessione ogni volta]),
    ([Footprint], [`--print-memory-usage`], [text 42/42/37 KB, data 10.7 KB, stack 4 KB]),
  ),
)
