# safety-island

Flow di build, run e debug per la **i.MX94 EVK (silicio A0)**: System Manager (SM) con debug monitor sul
core M33_0, più un'app firmware per ciascun core secondario: M33_1 (M33S), M7_0 e M7_1.

L'immagine viene caricata **in RAM** con `uuu` (SDP): non scrive su SD/eMMC, e si itera in pochi secondi.

## Avvio rapido

```bash
make usb-setup  # setup WSL (sudo): usbip, udev, servizio di attach; stampa le policy Windows
make run        # build (toolchain, SDK, app, flash.bin) + caricamento in RAM con uuu
make rtt        # log di tutti i core via RTT (rtt-m70/-m71/-m33s: un core, con input; "make uart" = monitor SM)
make debug-m70  # gdb su M7_0 (anche debug-m71, debug-m33s; "make debug" = M33_0/SM)
make help       # tutti i target
```

Da VS Code: `Ctrl+Shift+B` compila, `F5` con "M7_0: run + attach" compila, carica e attacca il debugger
(vedi [VS Code](#vs-code)).

Al primo build vengono scaricati e verificati: toolchain Arm GNU, `uuu`, l'SDK MCUXpresso minimo (west),
il firmware ELE NXP (ti chiede di accettare la EULA) e `mkimage_imx8`. Tutto finisce in `ext/` (condiviso
tra i preset; sopravvive a `make clean` e `make distclean`, solo `make clean-all` lo rimuove) o in
`build/`. Le volte successive si ricostruisce solo quello che è cambiato (~15 s, niente download).
Doxygen è una dipendenza solo di `docs`: viene scaricato (1.18.0, sha256) al primo `make docs`.

Il `Makefile` è solo una scorciatoia: `make <target> [PRESET=mx94alt]` equivale a
`cmake --build --preset <PRESET> --target <target>`, dopo aver configurato il preset se serve.

## Cosa serve

Da un clone pulito serve solo questo sull'host; **tutto il resto lo scarica il build al primo uso**
(toolchain Arm, `uuu`, `bcu`, SDK via `west`, `mkimage`, ELE; Doxygen solo per `docs`), con versioni
pinnate e sha256.

- **Sempre:** `cmake` ≥ 3.25, `make`, `gcc` (per `mkimage_imx8`), `git`, `curl`, `python3`, e `uv`
  oppure il pacchetto `python3-venv` (il venv in `ext/venv` serve a `west` e a `make rtt`).
- **WSL, una tantum (con `sudo`):** `usbipd-win` su Windows, systemd attivo in WSL e `make usb-setup`
  (usbip, udev, servizio di attach). La policy per Windows (`usbipd policy add ...`) va data una volta
  per PC da PowerShell amministratore: lo script stampa i comandi, non è automatizzabile da WSL.
- **Solo per `debug`/`rtt`:** J-Link Linux (`JLinkGDBServerCLExe`, provato V9.94), proprietario: non è
  scaricabile dal build.
- **Solo per `sm-verify`/`sm-update`/`docker-image`:** Docker (opzionale).
- **`make uart`:** `picocom`, lo installa `make usb-setup` (`screen` come ripiego).
- **EULA ELE:** il primo `make image` scarica il firmware ELE e l'installer NXP chiede di accettare la
  EULA; in un ambiente non interattivo: `cmake --preset mx94evk -DACCEPT_ELE_EULA=ON` (equivale a
  "ho letto e accetto la EULA NXP").
- **`docs`:** nessun prerequisito manuale: Doxygen 1.18.0 viene scaricato in `ext/tools/` al primo
  `make docs` (sha256). Mermaid e MathJax sono caricati dal browser via CDN; non servono Graphviz né LaTeX.
- **Solo per `format`/`lint`/`misra`/`check` (opzionali):** `clang-format` ≥ 19.1.7,
  `clang-tidy` ≥ 19.1.7 e `cppcheck` ≥ 2.19.0. Su Ubuntu:
  `sudo apt install clang-format-19 clang-tidy-19 cppcheck`. Non servono per `image`, `run` o `docs`:
  `make check` fallisce subito se mancano, avvisa se la versione è vecchia (niente sha256: dipende dalla
  distro). Dettagli in `docs/qualita.md`.

## Struttura

È un superbuild CMake senza linguaggi: ogni modulo ha il suo `CMakeLists.txt`.

```
CMakeLists.txt       superbuild: include i moduli
CMakePresets.json    mx94evk, mx94alt, docker
cmake/               si_tool() (scarica un tool per l'host con sha256), verifica sha256, helper
deps/                west.yml (SDK MCUXpresso minimo, pinnato) + toolchain Arm per l'host
fw/                  app (m33s, m70, m71) + ruoli (safe/, fieldbus/) + common/ (board glue e ABI) + cmake/
ele/                 firmware ELE NXP: scaricato, EULA, ele.sha256
sm/                  binari SM committati + Dockerfile riproducibile + patches/ alla config NXP
image/               mkimage_imx8 (ExternalProject) e flash.bin
board/               flash, run, reset, uart, debug, usb: wrapper agli script; usb-devices.conf
docs/                Doxyfile + pagine Markdown (architettura/setup/misure) + tema; il resto è generato
quality/             target opzionali clang-format/clang-tidy/cppcheck (MISRA non certificato)
scripts/             script per USB/seriale/J-Link/uuu, west e Docker dell'SM
docker/              container per il target image
.vscode/             task, debug (Cortex-Debug), IntelliSense
ext/                 (generato, gitignored) toolchain, uuu, SDK, venv di west; ext/toolchain -> toolchain in uso
```

```
ele/  (target ele) ──┐
sm/sm_<cfg>.bin    ──┼─► mkimage_imx8 ─► flash.bin ─► uuu SDPS: boot ─► ROM ─► ELE FW + SM (M33_0)
fw/<app> o M*_BIN  ──┘                                                       └► app dei core nei TCM
```

## Target

| Target | Cosa fa |
|---|---|
| `image` | app di `fw/`, verifica sha256 di SM ed ELE, `build/<preset>/flash.bin`. Default di `cmake --build` |
| `flash` / `run` | `image`, reset della board (vedi [Reset](#reset)), poi caricamento in RAM con uuu (secondo tentativo se fallisce) |
| `fw` / `fw-m33s` / `fw-m70` / `fw-m71` | solo il firmware (tutte le app o una) |
| `deps` | `west update` dell'SDK (`deps/west.yml`); il build lo fa da solo quando il manifest cambia |
| `reset` | reset della board (`RESET_METHOD`, default hardware via J-Link): riparte in serial download |
| `uart` | console del debug monitor dell'SM (`SM_UART`) |
| `rtt` / `rtt-m33s` / `rtt-m70` / `rtt-m71` | log RTT di tutti i core / di un core con input (J-Link in attach, core mai fermati) |
| `debug` / `debug-flash` | J-Link GDB server + gdb sul core `JLINK_CORE`, con i simboli. `GDB_BATCH=1`: PC e backtrace, poi il core riparte |
| `debug-m33s` / `debug-m70` / `debug-m71` | `debug` su M33_1 / M7_0 / M7_1 con l'ELF dell'app |
| `usb-setup` / `usb-status` | setup una tantum di USB/IP in WSL (sudo) / stato di dispositivi e seriali |
| `inspect` | container e immagini AHAB di `flash.bin` (`scripts/ahab_walk.py`) |
| `ele` | scarica il firmware ELE (2.0.6), estrae il container i.MX94 A0, verifica lo sha256 |
| `sm-verify` / `sm-update` | ricostruiscono l'SM da sorgenti nel Docker dedicato: confronto con `sm/`, o sovrascrittura |
| `docs` / `docs-serve` | `build/<preset>/docs/html/index.html` con Doxygen; `docs-serve` = server locale su `DOCS_PORT` |
| `format` / `format-check` | `clang-format` sui sorgenti `fw/` (esclusa la libreria FSoE); `format-check` non modifica |
| `lint` / `lint-fix` | `clang-tidy` sul `compile_commands.json`; `lint-fix` applica i fix, da rivedere |
| `misra` / `misra-check` | `cppcheck --addon=misra` (MISRA C:2012, **non certificato**): report, o gate che fallisce su finding nuovi e su deviazioni obsolete della baseline |
| `check` / `check-misra` | precheck tool + format-check + lint + report MISRA; `check-misra` include il gate MISRA |
| `docker-image` | `image` dentro un container (preset `docker`, senza `fw/`, output in `build/docker/`) |
| `clean` / `distclean` / `clean-all` | pulizia (solo `make`, senza riconfigurare): build del preset corrente / tutti i build / anche `ext/` e `.west/`. Le prime due non richiedono rete |
| `py-deps` | solo il venv `ext/venv` (west + pylink): dipendenza di `rtt` e `deps` |

## Firmware (`fw/`)

Un'app completa per core, bare metal (niente RTOS), con un SDK MCUXpresso **minimo** come dipendenza
esterna:

```
fw/m70/   CMakeLists.txt + main.c     app M7_0:  board_init + safe_main(FLOW_CORE_SAFE_A)
fw/m71/   CMakeLists.txt + main.c     app M7_1:  board_init + safe_main(FLOW_CORE_SAFE_B)
fw/m33s/  CMakeLists.txt + main.c     app M33_1: board_init + fieldbus_main()
fw/safe/                              ruolo 1oo2 (m70+m71): 4 passi, logica safe, callback master, ciclo, MU18
fw/fieldbus/                          ruolo campo (m33s): slave FSoE, campo, merge A55, callback slave, ciclo
fw/common/board.[ch]                  init comune: SCMI con l'SM, RTT (LOG, board_getkey), SysTick, attesa debugger
fw/common/rtt_shared.[ch]             RTT con control block condiviso, un canale per core
fw/common/flow_platform_imx94.c       HAL del virtual safe PLC: tick, tempo DWT, RNG, log, fatal
fw/common/flow/                       ABI e astrazioni condivise del virtual safe PLC (mailbox, ICC, MU, CRC)
fw/common/tcm.ld.in                   linker script (solo TCM), generato per core
fw/cmake/imx94.cmake                  imx94_firmware(): sorgenti SDK e del ruolo, flag, linker, .bin/.map
```

- Ogni app è solo wiring: `board_init()`, banner RTT (agente SCMI e clock del core) e punto di ingresso del
  ruolo. Il codice di un ruolo sta in `fw/safe/` o `fw/fieldbus/`; in `fw/common/` solo quello che serve
  identico a più ruoli. `ROLE` in `imx94_firmware()` sceglie i sorgenti (la struttura è in
  `docs/architettura.md`).
- Nessuna periferica: niente UART, pin o clock da chiedere all'SM.
- **SDK come dipendenza, non copiato:** `deps/west.yml` pinna per commit 4 repo NXP (`mcuxsdk-core`,
  `mcux-devices-imx`, `mcux-component`, `mcu-sdk-cmsis`), clonati shallow in `ext/sdk/` da west
  (venv in `ext/venv`). Dell'SDK usiamo solo: CMSIS, header/startup/`system_*.c` del MIMX94398, driver
  `common`, `mu1`, `cache`, `fsl_clock`, componenti `scmi`/`smt` (client SCMI verso l'SM). RTT è nostro
  (`fw/common/rtt_shared.c`, formato SEGGER). Niente `west build`, Kconfig o board files SDK. Aggiornare l'SDK = cambiare gli
  SHA nel manifest.
- **Perché SCMI anche "bare metal":** con l'SM, IOMUX, clock e power sono protetti dal TRDC: si chiedono
  all'SM via SCMI (MU + shared memory, `SystemPlatformInit()` del SDK). Le app di partenza lo usano solo
  per leggere il nome dell'agente; serve appena tocchi una periferica (vedi "Console seriali").
- **Toolchain:** Arm GNU 15.2.rel1 (lo stesso dell'SM) scaricata per l'host (Linux x86_64/aarch64,
  macOS arm64) in `ext/tools/`, sha256 verificato; `ARMGCC_DIR` per usarne una già installata.
- **Niente DDR:** `flash.bin` non inizializza la DDR. Il linker script mette tutto nei TCM e disattiva
  l'azzeramento della resource table RPMsg che il SDK fa in DDR all'avvio (sarebbe un HardFault).
- Uscite: `build/<preset>/fw/<app>/<app>.{elf,bin,map}`, `build/<preset>/compile_commands.json` (unito).

## Virtual safe PLC (FSoE)

Le tre app non sono più solo un banner: insieme formano un **1oo2 FSoE** che gira sulla board. Due core
eseguono lo stesso codice di sicurezza, il terzo fa il campo. L'architettura e le misure sono in
`docs/` (`make docs` → `build/<preset>/docs/html/index.html`); API e protocolli sono nei commenti
Doxygen di `fw/`. Qui solo il minimo per usarlo.

| App | Core | Ruolo |
|---|---|---|
| `fw/m70` | M7_0 | `SAFE_A`: master FSoE #1, pubblica la **metà CRC** del frame |
| `fw/m71` | M7_1 | `SAFE_B`: master FSoE #2, pubblica la **metà dati** |
| `fw/m33s` | M33_1 | `FIELDBUS`: fa l'A55 — unisce le due metà, esegue lo slave FSoE e il campo |

I due M7 sono il 1oo2 perché la **MU18** (lato A su M7_1, lato B su M7_0) è l'unica MU fra due core
secondari: l'M33S non ne ha verso gli M7. La MU ha 16 byte per direzione, quindi porta solo il doorbell
`{seq, tag, channel}` + CRC-32 del payload; il messaggio (72 B) viaggia in OCRAM.

Memoria condivisa (OCRAM, patch SM `0001`/`0002`, concessa alle tre LM):

| Indirizzo | Uso |
|---|---|
| `0x204D0000` | control block RTT (canali 0 m33s, 1 m70, 2 m71) |
| `0x204D4000` | mailbox FSoE: frame spezzato metà CRC / metà dati, 3 banchi |
| `0x204D5000` | cross-check 1oo2 (payload + CRC) e barrier di ciclo |

```bash
make run && make rtt
```

Log atteso: entrambi i master arrivano a `Data` con lo **stesso** session id, `anom 0`, e il fieldbus
vede le uscite abilitate.

```text
[m70/A] cyc 3900 Data anom 0 icc p11700 r11700 to0 crc0 sq0 cy0 db0 mu to0/0 sess 0xb7d5 hb 3900/3900/3900
[m33s/fld] bus 19000 Data out 1 en 1 ok 18705 drop 0 a55 0 hb 3900/3900/3801
```

Comandi sul canale RTT del core (`make rtt-m70`, `make rtt-m33s`, …):

| Tasto | Dove | Effetto |
|---|---|---|
| `p` `i` `l` `x` | m70/m71 | fault injection: PDU, inputs, logic, session → uscite 0, reset, riconnessione |
| `?` | m70/m71 | stato completo (contatori ICC, anomalie, session id) |
| `s` | m33s | discrepanza fra i due canali di campo |
| `d` | m33s | scarta 60 frame (~120 ms) → fa scadere il watchdog FSoE |
| `?` | m33s | stato del fieldbus |

Note:

- `WAIT_DEBUGGER` è **OFF** nella cache per il ciclo flash→RTT; per il debug con attesa:
  `cmake --preset mx94evk -DM70_WAIT_DEBUGGER=ON -DM71_WAIT_DEBUGGER=ON -DM33S_WAIT_DEBUGGER=ON`.
- un attach SWD ferma il core: se apri `make rtt` vedrai un `barrier timeout` e un `WD_EXPIRED` seguito da
  riconnessione. È il fail-safe che funziona, non un guasto.
- limiti: il Session ID usa un RNG di banco (DWT+SysTick, `FLOW_BENCH_RNG`, TODO TRNG ELE), niente
  MPU/ECC, i due core sono uguali (nessuna diversità hardware). Dettagli in `docs/` e `AGENTS.md`.

## VS Code

Estensioni consigliate (`.vscode/extensions.json`): **Cortex-Debug** (`marus25.cortex-debug`) e C/C++.

- **Task** (`Terminal > Run Task`): `Build` (default, `Ctrl+Shift+B`), `Run` (core partono subito),
  `Run (attesa debugger)` (`WAIT_DEBUGGER=ON` per tutti: ricompila il firmware, ~7 s), `Reset`,
  `Deps: sync`, `USB: stato`, `UART: monitor SM`, `RTT: tutti (terminale)`.
- **Debug** (`F5`):
  - `Tutti i core: run + attach` (compound): task `Run (attesa debugger)` (build con
    `WAIT_DEBUGGER=ON` e caricamento), poi attach a M7_0, M7_1 (+0,5 s) e M33S (+1 s).
    Ogni core resta fermo prima di `main` finché il suo debugger non si attacca; l'attach J-Link ferma
    il core: parte quando premi Continue in quella sessione. Nessuna sincronizzazione fra core.
  - `Tutti i core: attach` (compound): attach ai core già in esecuzione (+ RTT dal `rttConfig` di ogni sessione).
  - `<core>: attach`: un solo core.
  - `M33_0 (SM): attach`: l'SM (simboli da `sm-verify`/`sm-update`).

  Il core si sceglie dal Call Stack o dalla tendina della barra di debug; stop chiude tutte le sessioni.
  J-Link usa **un GDB server per core** (il server è single-core: la sonda è condivisa a livello di
  link), quindi ogni sessione è una connessione, l'RTT compreso (`rttConfig`): 3 core = 3 connessioni
  (vedi il limite in [Log: SEGGER RTT](#log-segger-rtt)).
  Live Watch è per sessione. Ogni sessione scrive `g_debugger_go = 1` all'attach (`postAttachCommands`):
  la variabile è sempre nel firmware (se gdb non la trova l'attach si blocca). gdb è quello della
  toolchain scaricata. Il terminale "gdb-server" logga ogni lettura di registri e memoria: va lasciato
  così (`-silent` nasconde anche la riga che Cortex-Debug aspetta).
- **Log dei core (RTT):** ogni sessione di debug mostra il canale del suo core (`rttConfig`: 0 m33s,
  1 m70, 2 m71) in un terminale `RTT <core>`, con input, affiancabile con lo split. Non serve una
  connessione J-Link in più: l'RTT passa dal GDB server della sessione. Non aprire `make rtt` sugli
  stessi canali durante il debug: due lettori si rubano i dati.
  Vedi [Log: SEGGER RTT](#log-segger-rtt).
- **Variabili dal vivo:** vista **Cortex Live Watch** nella sidebar Run and Debug (`liveWatch` nel
  `launch.json`): aggiungi espressioni (`s_ticks`, campi di struct...) e si aggiornano 4 volte al secondo
  a core in esecuzione. La vista Watch normale si aggiorna solo a core fermo.
- **Logpoint:** click destro sul margine → "Add Logpoint…" (`tick {s_ticks}`); l'output esce nella
  **Debug Console** (`Ctrl+Shift+Y`). Con Cortex-Debug un logpoint ferma il core, gdb stampa e lo fa
  ripartire (millisecondi via SWD): ok su codice lento, non nei loop veloci o nelle ISR (meglio `LOG`).
- **Breakpoint nell'init:** con `Run` normale l'app è già partita quando il debugger si attacca (i tick
  avanzano prima dell'attach). Con `Run (attesa debugger)` (o `-D<APP>_WAIT_DEBUGGER=ON`) ogni core si
  ferma prima di `main` finché il suo debugger non scrive `g_debugger_go = 1` (lo fanno il
  `launch.json` e `make debug-<app>`). Senza debugger, un core con l'attesa attiva resta fermo; `make rtt`
  aspetta il control block finché un core non arriva a `board_init()`.
- **IntelliSense:** `.vscode/c_cpp_properties.json` usa `build/mx94evk/compile_commands.json` (scritto da
  `make image`/`make fw`).
- In WSL apri la cartella con l'estensione Remote-WSL: J-Link, usbip e uuu girano tutti in WSL.

## Log: SEGGER RTT

Le app scrivono con `LOG(...)` (printf di newlib-nano) e leggono input con `board_getkey()` via **SEGGER
RTT**: buffer in RAM che il J-Link legge **mentre i core girano**, via SWD. Niente UART, pin o clock.

**Un canale per core, un solo control block condiviso.** Il motore RTT del J-Link è uno per sonda: se
ogni core avesse il suo control block nella propria TCM, se ne vedrebbe uno solo (provato: più sessioni
su core diversi leggevano tutte quello del primo core e si rubavano i dati). Per questo control block e
buffer stanno in **16 KB di OCRAM condivisi** da M33S, M7_0, M7_1 a `0x204D0000` (patch SM
`sm/patches/0001-mx94evk-rtt-shared-ocram.patch`), con:

| Canale (up e down) | Core | Buffer |
|---|---|---|
| 0 | m33s | 4 KB out, 64 B in |
| 1 | m70 | 4 KB out, 64 B in |
| 2 | m71 | 4 KB out, 64 B in |

Il formato in memoria è quello standard di SEGGER (`fw/common/rtt_shared.[ch]`, al posto di
`SEGGER_RTT.c`, la cui init cancellerebbe i canali degli altri core: ogni core scrive solo i suoi
descrittori). Una sessione J-Link collegata a **qualunque** core vede tutti i canali (stesso indirizzo
per tutti). Non bloccante: se nessuno legge e il buffer è pieno, il messaggio si perde.

- `make rtt`: tutti i core in un terminale (le righe hanno già `[m70]`...); input con `m70: testo`.
- `make rtt-m70` (`-m71`, `-m33s`): solo quel core, input diretto. Ogni console è una connessione.
- **Limite della sonda:** il J-Link accetta al massimo **5 connessioni** contemporanee. Ogni
  `make rtt`/`rtt-<app>` e ogni GDB server ne occupa una (in debug l'RTT passa dal `rttConfig` della
  sessione, senza aggiungerne); con 3 GDB server restano 2 slot. Anche RTT Viewer, J-Scope e
  `JLinkRTTLogger` contano. `make rtt` non riprova: se la sonda è occupata stampa un errore e termina.
- `make run` chiude le console `make rtt` prima del reset; se il caricamento fallisce, l'errore lo dice
  e ricorda di chiudere eventuali altre sessioni J-Link (GDB server, RTT Viewer, ...).
- VS Code: task `RTT: tutti (terminale)` per la console fuori dal debug; in debug l'RTT arriva dal
  `rttConfig` di ogni sessione (vedi [VS Code](#vs-code)).
- Lettura via DLL J-Link (`pylink` nel venv di `ext/`): si collega a M7_1 di default (`RTT_CORE`),
  senza reset né halt.
- Strumenti SEGGER (RTT Viewer, J-Scope, `JLinkRTTLogger`): control block a `0x204D0000`, canale =
  core. Usali con le console chiuse sullo stesso canale.
- Cortex-Debug `rttConfig` è usato con J-Link: ogni sessione legge **un canale** (il `port` 0/1/2 del
  suo core), che è esattamente il caso d'uso del control block condiviso.
- Nessuna D-cache attiva sull'area (gli M7 abilitano solo la I-cache, l'M33S solo la cache del bus
  codice): se se ne abilita una, l'area va resa non-cacheable via MPU.

## USB in WSL

Board, J-Link e seriali arrivano in WSL via USB/IP: server `usbipd-win` su Windows, client Linux `usbip`
in WSL. Una tantum:

- **Windows** (PowerShell da amministratore): policy AutoBind per i VID:PID di `board/usb-devices.conf`;
  i comandi esatti li stampa `make usb-setup`.
- **WSL**: `make usb-setup` (sudo) installa usbip/libusb/udev e il servizio `safety-island-usb`, che
  attacca da solo i dispositivi (anche la ROM SDP a ogni reset).

Stato: `make usb-status`; log: `journalctl -u safety-island-usb -f`. Seriali sempre per
`/dev/serial/by-id/`. Dettagli e variabili (`USBIP_HOST`, `USB_POLL`): intestazioni di
`scripts/usb-setup.sh` e `scripts/usb-attach.sh`.

## Reset

`make reset` (e ogni `make run`, prima di caricare) resetta la board con `scripts/reset.sh`.
`RESET_METHOD`:

| Metodo | Come | Note |
|---|---|---|
| `jlink` | pin nRESET del connettore di debug (J-Link Commander `r0`/`r1`) | **Hardware**, come premere il pulsante: non dipende dall'SM né dai core. Serve solo la sonda collegata. Provato |
| `bcu` | NXP Board Control Utility sull'FTDI della EVK: `bcu reset usb -keep` | Hardware, e in più forza il boot mode `usb` (serial download) senza toccare i boot switch; resta forzato anche dopo (`bcu deinit -board=…` per tornare agli switch). Serve il profilo giusto in `BCU_BOARD`: `imx943evk19b1` (default, la nostra board) con **SW7-1 OFF**, `imx943evk19a0` (proto1) con **SW7-1 ON**. Con SW7-1 sbagliato: `get gpio failed`. La EEPROM dell'FTDI è vuota, quindi `-auto` non riconosce la board; l'FTDI quad (`0403:6011`) lo trova lo script da sysfs (`BCU_ID` per forzarlo). BCU prende il canale `if01` dell'FTDI (non è una UART: sparisce da `/dev/serial/by-id`). Provato |
| `monitor` | comando `reset` del debug monitor dell'SM sulla seriale | Solo se l'SM gira e risponde |
| `auto` (default) | `jlink`, poi `bcu` (se `BCU_BOARD`), poi `monitor` | il primo che riesce |

**BCU non dipende dalla boot ROM né dall'immagine flashata.** È hardware della EVK: l'FTDI pilota un
expander I2C che comanda la linea di reset e, con `remote_en`, un mux che scavalca i boot switch sui pin
`BOOT_MODE` del SoC. La ROM legge solo quei pin al reset, quindi con BCU la board torna in serial download
qualunque cosa ci sia su SD/eMMC/flash.

Con `jlink` e `monitor` invece la board riparte nel boot mode dei boot switch:
- boot media vuoto o immagine non valida: la ROM ripiega da sola sul serial download (provato con gli
  switch su SD e una SD senza immagine avviabile);
- immagine **valida ma rotta**: la ROM la avvia e non ripiega; solo BCU (o gli switch a mano) la
  riportano in serial download. Provato: con l'immagine di boot di LF 6.12.20 scritta sulla SD
  (`uuu -b sd imx-boot-imx943evk-sd.bin-flash_all`) e gli switch su SD, la board si blocca nell'OEI DDR
  (M33_0 fermo a `0x1FFC20AA`, nessun output); il reset J-Link la fa ripartire sempre da lì, `make run`
  passa a BCU e carica (~20 s).

Per questo `make run`, se dopo il primo reset la ROM non compare in serial download entro 10 s, fa il
secondo tentativo con `bcu` (se `BCU_BOARD` è impostato). Provato tenendo la board in reset con BCU: il
reset J-Link non basta, il secondo tentativo BCU recupera e carica. `FLASH_NO_RESET=1 make run` salta
il reset (board appena resettata a mano). Il boot mode forzato da BCU resta attivo
(`bcu get_boot_mode -board=imx943evk19b1`); `bcu deinit -board=imx943evk19b1` torna agli switch.

### Scrivere SD/eMMC

`uuu -b sd <imx-boot...flash_all> [<immagine .wic>]` (o `-b emmc`) dalla cartella dei file LF, con la
board in serial download (`make reset`). La catena passa per tre dispositivi USB: ROM (`1fc9:0027`), SPL
(`1fc9:0151`) e U-Boot fastboot (`1fc9:0152`), tutti in `board/usb-devices.conf` (e nelle policy
Windows). Sulla nostra board l'immagine di boot di LF 6.12.20 scritta su SD si blocca nell'OEI DDR al boot
da SD (via SDP lo stesso file funziona): non indagato.

## Configurazione

Le opzioni sono variabili della cache CMake. Si passano alla configurazione, oppure si mettono in un
`CMakeUserPresets.json` personale (è nel `.gitignore`):

```bash
cmake --preset mx94evk -DM70_WAIT_DEBUGGER=ON -DRESET_METHOD=bcu
```

| Variabile | Default | Note |
|---|---|---|
| `SM_CONFIG` | dal preset | `mx94evk` (Linux + M7) o `mx94alt` (demo SDK) |
| `SM_BIN` | vuoto | SM alternativo da provare, senza verifica sha256 |
| `MSEL` | `0` | passato all'SM: sceglie il blocco `MODE msel=N` del suo cfg |
| `FW_APPS`, `FW_BUILD_TYPE` | `ON`, `Debug` | compila le app di `fw/` / loro `CMAKE_BUILD_TYPE` |
| `M33S_WAIT_DEBUGGER`, `M70_..`, `M71_..` | `OFF` | l'app aspetta il debugger prima di `main` |
| `M33S_BIN`, `M70_BIN`, `M71_BIN` | vuoto | binari esterni per i core: hanno la precedenza sulle app di `fw/` |
| `ARMGCC_DIR` | vuoto | toolchain già installata invece di quella scaricata |
| `SM_UART` | `/dev/serial/by-id/usb-FTDI_Quad_RS232-HS-if03-port0` | monitor dell'SM (LPUART2) |
| `RESET_METHOD`, `BCU_BOARD`, `BCU_ID` | `auto`, `imx943evk19b1`, vuoto | vedi [Reset](#reset) |
| `ACCEPT_ELE_EULA` | `OFF` | `ON` = hai letto e accetti la EULA NXP; serve senza terminale (Docker, CI) |
| `ELE_IMAGE` | vuoto | container ELE già disponibile, senza download né verifica |
| `JLINK_CORE`, `JLINK_PORT`, `GDB_ELF` | `M33_0`, `2331`, vuoto | debug (`GDB_ELF` vale per tutti i target `debug*`) |
| `MKIMAGE_TAG`, `MKIMAGE_EXE` | `lf-6.18.20-2.0.0`, vuoto | versione di mkimage, o un mkimage già compilato |
| `SM_TAG`, `SM_CONFIGS` | `lf-6.18.20-2.0.0`, `mx94evk;mx94alt` | per `sm-verify`/`sm-update` |
| `SI_TOOLS_DIR` | `ext/tools` | dove scaricare toolchain e strumenti |
| `DOXYGEN_VERSION`, `DOCS_PORT`, `DOCS_AWESOME_CSS` | `1.18.0`, `8000`, `ON` | versione Doxygen scaricata, porta di `docs-serve`, tema committato |
| `SI_CLANG_MIN_VERSION`, `SI_CPPCHECK_MIN_VERSION` | `19.1.7`, `2.19.0` | versioni minime dei tool quality; sotto la minima solo warning |

I percorsi relativi dei binari sono risolti rispetto alla radice del repo.

Indirizzi dei core secondari (come `iMX94/soc.mak` di mkimage):

| Core | id | entry | load |
|---|---|---|---|
| SM (M33_0) | 0 | `0x1FFC0000` | `0x1FFC0000` |
| M33S | 1 | `0x1FFC0000` | `0x309C0000` |
| M7_0 | 0 | `0x0` | `0x303C0000` |
| M7_1 | 1 | `0x0` | `0x302C0000` |

### Config dell'SM: `mx94evk` e `mx94alt`

- `mx94evk`: periferiche divise tra le LM (AP, M33S, M7_0, M7_1). Con questa config GPIO2 e LPUART4 sono
  dell'**AP**. È la config NXP più la patch che condivide 16 KB di OCRAM (`0x204D0000`) fra M33S, M7_0 e M7_1 per
  RTT (`sm/patches/`). Le UART di ogni LM sono in
  [Console seriali](#console-seriali-alternativa-a-rtt).
- `mx94alt`: una sola LM possiede tutto, ed è quella pensata per i demo del SDK MCUXpresso. Nei test
  l'M7_0 con `alt` è rimasto in `HOLD` anche con `MSEL=4`; si è avviato dal monitor con
  `cpu.w 1 vector 0x303C0000` e `cpu.w 1 start`.

### Console seriali (alternativa a RTT)

Le app usano RTT. Se servono le seriali (niente J-Link, output senza debugger, log da conservare), queste
sono le UART che la config `mx94evk` (SM lf-6.18.20, senza patch) dà a ogni LM:

| LM | UART | Pad | Dove sulla EVK |
|---|---|---|---|
| SM (M33_0) | LPUART2 | `UART2_*` | USB J15 (FTDI quad `if03`): debug monitor (`make uart`) |
| M33S | LPUART3 | `GPIO_IO30` (TX), `GPIO_IO31` (RX) | header, convertitore USB-seriale esterno (`GPIO_IO31` = J44-10) |
| M33S | LPUART8 | `DAP_TMS_SWDIO` (TX), `DAP_TCLK_SWCLK` (RX) | **mai: sono i pin SWD** (vedi Debug J-Link) |
| M7_0 | LPUART11 | `GPIO_IO24` (TX), `GPIO_IO25` (RX) | J48-4 (TXD), J48-2 (RXD), convertitore esterno |
| M7_1 | LPUART12 | `GPIO_IO26` (TX), `GPIO_IO27` (RX) | J44-2 (TXD), J44-4 (RXD), convertitore esterno |
| AP | LPUART1, 4, 5, 6, 7, 9, 10 | | LPUART1 su USB J15 (`if02`), le altre su header |

Porte USB J15 (FTDI quad): `if00` = LPUART8, `if01` = canale usato da BCU (non UART), `if02` = LPUART1,
`if03` = LPUART2. Nell'SM lf-6.12.20 LPUART3 e i pad `GPIO_IO30/31` erano dell'AP.

Come si faceva (provato, codice nella history git di `fw/common/board.c`):
- pin mux e pad config **via SCMI** (`SCMI_PinctrlSettingsConfigure`, con la tupla `IOMUXC_PAD_...` del
  SDK; non `IOMUXC_SetPinMux` del SDK, che se l'SM rifiuta resta in un `while` infinito), clock a 24 MHz
  via SCMI (`CLOCK_SetRate`/`CLOCK_EnableClock`), poi `DbgConsole_Init()` di `debug_console_lite` (o
  `fsl_lpuart`); nel build `drivers/lpuart`, `components/uart`, `components/str`,
  `components/debug_console_lite`, `SDK_DEBUGCONSOLE=1`;
- per avere M7_0 su USB senza convertitore: patch SM che sposta `LPUART1` e `PIN_UART1_RXD/TXD` dalla LM
  AP alla LM M7 (esempio in `sm/patches/README.md`): l'A55 resta senza console.
- console: `make uart` con `SM_UART` puntato alla porta, o `UART_PORT=<porta> scripts/uart.sh`.

## Debug J-Link

Un core alla volta, sempre dopo `make run` (l'immagine va caricata con `uuu`, il J-Link non scarica nulla):

```bash
make debug-m70                 # gdb interattivo su M7_0 ("continue" prima di uscire)
GDB_BATCH=1 make debug-m33s    # PC + backtrace, poi il core riparte
make debug JLINK_CORE=M7_1     # equivalente a debug-m71 (o -DJLINK_CORE=... in configurazione)
```

- Device J-Link: `MIMX94398_M33_0`, `_M33_1`, `_M7_0`, `_M7_1` (quelli standard SEGGER, nessuno script).
  Su M33_1 e M7_x J-Link fa solo **attach**: niente reset né download, il core deve già girare (lo avvia
  l'SM da `flash.bin`).
- **LPUART8 rompe il debug.** È muxata sui pad `DAP_TCLK_SWCLK`/`DAP_TMS_SWDIO`. Se un core la
  configura (l'`hello_world` SDK dell'M33S lo fa di default, e anche i demo LF), il DAP non risponde più:
  J-Link segnala `Could not power-up system power domain` / `Target voltage too low (0.0 V)`, poi il suo
  ripiego "connect under reset" **resetta tutta la board**, che torna in SDP. Il sintomo non dipende dal
  core scelto: fallisce anche l'attach a M33_0. La nota NXP è in `enable_mcu_uarts.md` dei docs del SDK
  ("JTAG cannot be used when LPUART8 is used").
- Log del GDB server: `build/<preset>/jlink-gdbserver-<core>.log`.
- I simboli dell'SM per `make debug` (M33_0) sono in `build/<preset>/sm-docker/sm_<config>.elf`, prodotto
  da `make sm-verify`/`sm-update` (Docker): senza, gdb si attacca senza simboli.

## SM: binari committati e build riproducibile

I binari dell'SM sono in `sm/` e sono quelli che usa `image`. Per verificare come sono stati ottenuti
basta Docker: `make sm-verify` li ricostruisce da sorgenti (toolchain e tag NXP pinnati) e confronta gli
sha256. La build è riproducibile al byte. Le modifiche locali alla config NXP stanno in `sm/patches/`
(oggi: 16 KB di OCRAM condivisi per il control block RTT). Dettagli in [sm/README.md](sm/README.md).

## Docker per `image`

`make docker-image` esegue `image` in un container Ubuntu 24.04 (cmake, gcc, git, python3) con il preset
`docker`, riusando il firmware ELE già scaricato sull'host, senza le app di `fw/` (`FW_APPS=OFF`). Il
container gira con il tuo uid/gid e monta il repo in `/work`. Se `docker` dà "permission denied", il tuo
utente non ha il gruppo attivo: `newgrp docker` (o riapri la shell). Flash, UART e debug non girano nel
container.

## Limiti noti

- **Provato (SM lf-6.18.20 + ELE 2.0.6, preset `mx94evk`):** download di toolchain/uuu/SDK da zero,
  `image` (anche incrementale), `make usb-setup`, servizio di attach (ROM SDP ri-attaccata a ogni reset,
  J-Link staccato a forza e ri-attaccato), `run` con `uuu` Linux e reset hardware via J-Link (~8 s, più
  volte di fila), `reset` J-Link, BCU (SW7-1 OFF, `imx943evk19b1`) e monitor, app `fw/` con RTT condiviso (patch SM: OCRAM a `0x204D0000`; `make rtt`
  e `rtt-<app>` anche tutti aperti insieme, input per core, debug di altri core in parallelo), app su seriale (versione precedente, vedi "Console seriali"),
  `debug-m33s`/`-m70`/`-m71`/`debug` con `GDB_BATCH=1`, attach con gli argomenti di Cortex-Debug +
  `WAIT_DEBUGGER` + breakpoint in `board_init`, `sm-update`.
- **Non provati:** VS Code dall'interfaccia (configurazioni multi-core, task RTT come preLaunchTask,
  Live Watch: provati solo gli equivalenti da terminale), riavvio di WSL (`wsl --shutdown`) con il
  servizio, macOS/aarch64, `docs-serve`, preset `mx94alt` con il nuovo SM,
  `docker-image` e `sm-verify` senza cache Docker dopo l'upgrade.
- **`debug`:** l'attach ferma il core. In modalità interattiva bisogna fare `continue` prima di uscire;
  con `GDB_BATCH=1` lo script lo fa ripartire da solo. Un core alla volta (una sola porta GDB).
- **Un download ELE per preset:** ogni `build/<preset>/` scarica il suo firmware ELE.
- **Solo silicio A0:** il container ELE estratto è quello A0.
- **Quality:** `misra` è un report di cppcheck, **non una certificazione** MISRA (mancano rule texts e tool
  qualification); `format`/`lint` dipendono dalle versioni host e per questo il build avvisa se sono vecchie.
- **Non implementato:** boot persistente da SD/eMMC (`uuu -b sd <flash_all LF> <flash.bin>` è l'idea).
