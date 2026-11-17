#import "../lib/style.typ": *
= Come si parla con la board

Ci sono cinque canali indipendenti, e ognuno ha le sue trappole. In WSL i dispositivi USB arrivano con il
client `usbip` dal server `usbipd-win`: il servizio `safety-island-usb` li attacca per VID:PID e li
ri-attacca quando ricompaiono (`make usb-status` per lo stato).

#si-table(
  columns: (1.3fr, 2.2fr, 2.6fr, 2.4fr),
  header: ([Interfaccia], [A cosa serve], [Comando], [Trappole]),
  rows: (
    ([USB SDP], [caricare `flash.bin` in RAM dalla boot ROM], [`make run`], [la ROM deve essere in WSL (`1fc9:0027`)]),
    ([SWD], [debug/attach, reset hardware, RTT], [`make debug-<app>`, `make rtt`], [un GDB server per core; GDB server sfasati; LPUART8 = pin SWD]),
    ([Seriale], [console del monitor SM (LPUART2)], [`make uart`], [sempre per `/dev/serial/by-id/`]),
    ([RTT], [log e input delle app], [`make rtt` / `rtt-<app>`], [due lettori sullo stesso canale si rubano i dati]),
    ([BCU], [reset + boot mode forzato via FTDI], [`make reset` con `BCU_BOARD`], [SW7-1 deve essere OFF sul profilo b1]),
  ),
)

== SDP: caricare l'immagine

La boot ROM dell'i.MX94 espone una modalità *serial download*: `uuu` le manda `flash.bin` in RAM (nessuna
scrittura su SD/eMMC). `scripts/flash.sh` fa sempre *prima* un reset hardware e poi carica, perché non
c'è modo di sapere dall'USB se la ROM sta davvero aspettando:

```bash
make run            # image + reset + uuu SDPS: boot, ~10 s in tutto
FLASH_NO_RESET=1 make run   # board appena resettata a mano
```

Se dopo il reset la ROM non compare, `flash.sh` riprova forzando il boot mode con la #term("bcu")
(`-board=imx943evk19b1`, richiede SW7-1 OFF): è l'unico modo che funziona anche quando su SD/eMMC c'è
un'immagine valida che parte da sola.

== SWD: J-Link

I device SEGGER standard `MIMX94398_{M33_0,M33_1,M7_0,M7_1}` funzionano senza JLinkScript. `M33_0` è l'SM
(reset e download possibili); `M33_1`, `M7_0`, `M7_1` sono *solo attach* all'app già caricata.

#attenzione[
  *LPUART8 è muxata sui pin SWD* (`DAP_TCLK_SWCLK`/`DAP_TMS_SWDIO`). Qualsiasi immagine che la usi rende
  il DAP irraggiungibile: il J-Link scrive `Could not power-up system power domain` o `Target voltage too
  low` e il suo ripiego "connect under reset" *resetta tutta la board*. Le nostre app non usano
  periferiche, quindi il problema non si presenta: se compare, è un'immagine sbagliata.
]

J-Link usa *un GDB server per core* (il server è single-core; la sonda è condivisa a livello di link).
Ogni GDB server e ogni console `make rtt` è una connessione: la sonda ne accetta al massimo *5*, quindi
con 3 GDB server restano 2 slot. I compound sfasano gli attach di 0,5/1 s, perché se gdb parte insieme
la corsa d'avvio fa fallire l'attach.

== Seriale

L'ordine dei `ttyUSBn` cambia a ogni attach: si usano sempre i path `/dev/serial/by-id/…`. L'FTDI quad
della EVK espone quattro porte; la `if03` è l'LPUART2, cioè il *monitor del System Manager*:

```bash
make uart           # picocom sulla if03: prompt del monitor SM
reset               # dal monitor: riporta la board in SDP
cpu.r               # stato dei core
```

== RTT: log e input

Il control block RTT è *uno solo, condiviso*, in OCRAM a `0x204D0000` (patch SM 0001) con un canale per
core: `0` m33s, `1` m70, `2` m71. Non si usa `SEGGER_RTT.c` perché la sua init azzera i descrittori degli
altri core; `fw/common/rtt_shared.c` scrive solo i propri.

```bash
make rtt            # tutti i canali (una connessione per terminale)
make rtt-m70        # un solo core, con input diretto
```

- il canale è *no-block-skip*: se il buffer da 4 KB è pieno il messaggio intero si perde (mai righe
  spezzate). Per questo i log per-ciclo sono dietro `FLOW_LOG_VERBOSE`, spento di default;
- due processi sullo *stesso* canale si rubano i dati; su canali diversi convivono;
- `scripts/rtt.py` disabilita le dialog della DLL J-Link: senza, in un terminale non grafico una dialog
  resta appesa e `connect()` non torna mai;
- un attach SWD *ferma il core* finché non lo si fa ripartire: è la causa dei `barrier timeout` e dei
  `WD_EXPIRED` che si vedono subito dopo aver aperto `make rtt` (vedi @sec-attach).

== Reset

Tre modi, in ordine di preferenza (`RESET_METHOD=auto` li prova in quest'ordine):

1. *J-Link* (`r0`/`r1` sul pin nRESET): hardware, non coinvolge nessun core, funziona qualunque sia lo
   stato del software;
2. *BCU*: reset + boot mode "usb" forzato via expander; l'unico che recupera un boot media con
   un'immagine valida ma bloccata;
3. *monitor SM* (`reset`): solo se l'SM gira e risponde.

== Debug

`scripts/debug.sh` avvia `JLinkGDBServerCLExe` e `gdb` (dal toolchain pinnato) su un core:

```bash
GDB_BATCH=1 make debug-m70    # attach, info registers pc, bt, monitor go, detach
make debug-m71                # interattivo
```

- per i core secondari l'attach *non* scarica codice: l'app deve essere già in esecuzione (`make run`);
- con `WAIT_DEBUGGER=ON` l'app si ferma in `SystemInitHook()` finché il debugger non scrive
  `g_debugger_go = 1` (Cortex-Debug lo fa in `postAttachCommands`). Per il ciclo flash→RTT conviene
  `WAIT_DEBUGGER=OFF`, altrimenti i core non partono finché non li si attacca;
- l'attach ferma il core: dopo un `-batch` assicurati che parta `monitor go`, altrimenti resta fermo;
- `DHCSR.C_DEBUGEN` resta a 1 anche dopo una sessione: non usarlo per capire se c'è un debugger.
