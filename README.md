# safety-island

Flow di build e run per la **i.MX94 EVK (silicio A0)**: System Manager (SM) con debug monitor
sul core M33_0, più immagini opzionali per M33_1 (M33S), M7_0 e M7_1.

L'immagine viene caricata **in RAM** con `uuu` (SDP): non scrive su SD/eMMC, e si itera in pochi secondi.

## Avvio rapido

```bash
make ele        # una volta: scarica il firmware ELE NXP (ti chiede la EULA)
make run        # image + flash: ELE + SM -> board (reset automatico se serve)
make uart       # console del debug monitor SM (comandi: help, info, cpu.r, err, reset, ...)
```

`make help` elenca tutti i target; le variabili sono in `config.mk` e si sovrascrivono da riga
di comando.

## Cosa serve

- **Sempre:** `curl`, `make`, `git`, `python3`, `gcc` (per compilare `mkimage_imx8`), `uuu` (su WSL: `uuu.exe` nel PATH).
- **Solo per `make sm`:** toolchain `arm-gnu-toolchain-<ver>-x86_64-arm-none-eabi` (`TOOLS=<dir che lo contiene>`) e `srec_cat`.
- **Per `uart`:** `screen` o `picocom`. **Per `debug`:** J-Link (`JLinkGDBServerCLExe`) e `arm-none-eabi-gdb`.
- In WSL: porte seriali e J-Link condivisi con `usbipd`; la ROM della board (USB `1fc9:0027`) resta lato Windows, dove la vede `uuu.exe`.

Senza nulla di installato: `make docker-image` e `make docker-sm` fanno build dentro un container
(vedi sotto).

## Come funziona

```
build/ele/*.img      ──┐  (make ele)
prebuilt/sm/*.bin   ──┼─► mkimage_imx8 ─► build/flash.bin ─► uuu SDPS: boot ─► ROM ─► ELE FW + SM (M33_0)
M33S/M70/M71 (opz.) ──┘      (scaricato e compilato dal tag NXP)                       └► altri core in TCM
```

| Target | Cosa fa |
|---|---|
| `ele` | scarica `ELE_URL` (2.0.2), fa estrarre il container i.MX94 A0 in `build/ele/` e ne verifica lo sha256. EULA NXP: te la chiede, o `ACCEPT_ELE_EULA=1` |
| `sm` | clona `imx-sm` al tag `SM_TAG`, compila `SM_CONFIG` -> `build/sm/sm_<cfg>.{bin,elf}` |
| `sm-promote` | copia l'SM compilato in `prebuilt/` e aggiorna `SHA256SUMS` |
| `image` | verifica i checksum dei prebuilt, scarica e compila `mkimage_imx8` (tag `MKIMAGE_TAG`), crea `build/flash.bin`. Usa `build/sm/` se presente, altrimenti il prebuilt |
| `flash` | carica `build/flash.bin` in RAM. Se la board non è in SDP manda `reset` al monitor e aspetta |
| `reset` | comando `reset` del monitor SM sulla `SM_UART` |
| `uart` / `debug` / `debug-flash` | console, J-Link GDB server + gdb, flash e poi debug |
| `docs` / `docs-live` | compila `docs/main.typ` in `build/docs/safety-island.pdf` con typst (se non c'è nel PATH scarica `TYPST_VERSION`, checksum verificato); `docs-live` = `typst watch`, con `DOCS_VIEWER=wslview` apre il PDF |
| `inspect` | elenca container e immagini AHAB di `build/flash.bin` (`scripts/ahab_walk.py`) |

### Aggiungere altri core

```bash
make image M70=path/m70.bin M71=path/m71.bin M33S=path/m33s.bin MSEL=0
```

Indirizzi usati (come `iMX94/soc.mak` di mkimage):

| Core | id | entry | load |
|---|---|---|---|
| SM (M33_0) | 0 | `0x1FFC0000` | `0x1FFC0000` |
| M33S | 1 | `0x1FFC0000` | `0x309C0000` |
| M7_0 | 0 | `0x0` | `0x303C0000` |
| M7_1 | 1 | `0x0` | `0x302C0000` |

### Config dell'SM: `mx94evk` e `mx94alt`

- `mx94evk` (default): periferiche divise tra le LM (AP, M33S, M7_0, M7_1). Con questa config GPIO2 e LPUART4 sono dell'**AP**.
- `mx94alt`: una sola LM possiede tutto, ed è quella pensata per i demo del SDK MCUXpresso.
  Nei test l'M7_0 con `alt` è rimasto in `HOLD`; si è avviato da monitor con
  `cpu.w 1 vector 0x303C0000` e `cpu.w 1 start`.

## Docker

```bash
make docker-env     # costruisce l'immagine safety-island-build (Ubuntu 24.04 + gcc + srecord + toolchain ARM)
make docker-sm      # make sm dentro il container
make docker-image   # make image dentro il container
```

Il toolchain ARM **non viene scaricato**: `docker-env` lo copia dalla directory `TOOLCHAIN_DIR`
(default `/arm-gnu-toolchain-15.3.rel2-x86_64-arm-none-eabi`) tramite un named build context, senza
metterlo nel repo. Su un'altra macchina imposta `TOOLCHAIN_DIR=<dir>`. Gli URL di developer.arm.com
provati per la 15.3.rel2 rispondono 404.

Il container gira con il tuo uid/gid e monta il repo in `/work`, quindi i file in `build/` restano tuoi.
Se `docker` dà "permission denied", il tuo utente non ha il gruppo attivo: `newgrp docker` (o riapri la
shell). Flash, UART e debug non girano nel container (servono USB e `uuu.exe`).

## Limiti noti

- **Docker:** `docker-env`, `docker-sm` e `docker-image` sono provati su questa macchina. Il container
  produce un `sm_mx94evk.bin` che differisce dal prebuilt (percorsi e data di build), e un `flash.bin` delle
  stesse dimensioni.
- **`make ele`** provato con `ACCEPT_ELE_EULA=1`: scarica, estrae e verifica lo sha256 (`mx943a0-ahab-container.img`).
- **Provato:** `make sm`, `make image`, `make ele`, `make docs`, `make docker-*`, `make flash`/`run` con reset
  automatico, `make debug` su M33_0.
- **`debug`:** l'attach ferma l'SM e il `detach` non lo fa ripartire: dopo gdb il monitor tace finché non
  fai `continue` in gdb o un `go` dal J-Link. Su M33_1, M7_0 e M7_1 il J-Link non riesce ad accendere il DAP
  (`Could not power-up system power domain`), e i tentativi hanno resettato la board.
- **Core secondari:** M33S/M7 risultano in `RUN` dal monitor (`cpu.r`), ma la loro UART non è esposta
  dalla USB della EVK: servono un adattatore sui pin UART3/UART4 o un blink su `GPIO2_31` (J44 pin 10).
- **`REV=A0` soltanto:** `make ele` estrae solo il container A0; per la A1 serve `mx943a1-...` da un
  pacchetto ELE che lo contenga.
- **Non implementato:** boot persistente da SD/eMMC (`uuu -b sd <flash_all LF> <flash.bin>` è l'idea) e
  le parti di `ideas.md` sul CMake che installa i tool.
- Il repo contiene ancora lo scheletro del template CMake (`cmake/`, `CMakeLists.txt` vuoto).
