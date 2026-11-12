# safety-island

Flow di build e run per la **i.MX94 EVK (silicio A0)**: System Manager (SM) con debug monitor sul core
M33_0, più immagini opzionali per M33_1 (M33S), M7_0 e M7_1.

L'immagine viene caricata **in RAM** con `uuu` (SDP): non scrive su SD/eMMC, e si itera in pochi secondi.

## Avvio rapido

```bash
make run        # = cmake --preset mx94evk && cmake --build --preset mx94evk --target run
make uart       # console del debug monitor SM (help, info, cpu.r, err, reset, ...)
make help       # tutti i target
```

Al primo `run` vengono scaricati e verificati il firmware ELE NXP (ti chiede di accettare la EULA) e
`mkimage_imx8`. Le volte successive si ricostruisce solo quello che è cambiato.

Il `Makefile` è solo una scorciatoia: `make <target> [PRESET=mx94alt]` equivale a
`cmake --build --preset <PRESET> --target <target>`, dopo aver configurato il preset se serve.

## Cosa serve

- **Sempre:** `cmake` ≥ 3.25, `make`, `gcc` (per `mkimage_imx8`), `git`, `curl`, `python3`, `uuu`
  (su WSL: `uuu.exe` nel PATH).
- **Per `sm-verify` e `docker-image`:** Docker.
- **Per `uart`:** `screen` o `picocom`. **Per `debug`:** J-Link (`JLinkGDBServerCLExe`) e `arm-none-eabi-gdb`.
- **typst** per `docs`: se non è nel PATH viene scaricato (versione pinnata, sha256 verificato).
- In WSL: porte seriali e J-Link condivisi con `usbipd`; la ROM della board (USB `1fc9:0027`) resta
  lato Windows, dove la vede `uuu.exe`.

## Struttura

È un superbuild CMake senza linguaggi: ogni modulo ha il suo `CMakeLists.txt`.

```
CMakeLists.txt       superbuild: include i moduli
CMakePresets.json    mx94evk, mx94alt, docker
cmake/               si_tool() (trova o scarica un tool con sha256), verifica sha256, helper
ele/                 firmware ELE NXP: scaricato, EULA, ele.sha256
sm/                  binari SM committati + Dockerfile riproducibile per verificarli
image/               mkimage_imx8 (ExternalProject) e flash.bin
board/               flash, run, reset, uart, debug: wrapper agli script
docs/                typst
scripts/             script per le azioni su USB/seriale/J-Link e per il Docker dell'SM
docker/              container per il target image
```

```
ele/  (target ele) ──┐
sm/sm_<cfg>.bin    ──┼─► mkimage_imx8 ─► flash.bin ─► uuu SDPS: boot ─► ROM ─► ELE FW + SM (M33_0)
M33S/M70/M71       ──┘                                                       └► altri core in TCM
```

## Target

| Target | Cosa fa |
|---|---|
| `image` | verifica lo sha256 di SM ed ELE e crea `build/<preset>/flash.bin`. Default di `cmake --build` |
| `flash` / `run` | `image`, poi caricamento in RAM. Se il monitor dell'SM risponde, prima lo resetta in serial download |
| `reset` | comando `reset` del monitor SM: la board torna in serial download |
| `uart` | console del debug monitor SM (`SM_UART`) |
| `debug` / `debug-flash` | J-Link GDB server + gdb sul core `JLINK_CORE`, con i simboli dell'SM se c'è l'ELF di `sm-verify`. `GDB_BATCH=1 make debug`: PC e backtrace, poi il core riparte |
| `inspect` | container e immagini AHAB di `flash.bin` (`scripts/ahab_walk.py`) |
| `ele` | scarica il firmware ELE (2.0.2), estrae il container i.MX94 A0, verifica lo sha256 |
| `sm-verify` / `sm-update` | ricostruiscono l'SM da sorgenti nel Docker dedicato: confronto con `sm/`, o sovrascrittura |
| `docs` / `docs-live` | `build/<preset>/docs/safety-island.pdf` con typst; `docs-live` = `typst watch` |
| `docker-image` | `image` dentro un container (preset `docker`, output in `build/docker/`) |

## Configurazione

Le opzioni sono variabili della cache CMake. Si passano alla configurazione, oppure si mettono in un
`CMakeUserPresets.json` personale (è nel `.gitignore`):

```bash
cmake --preset mx94evk -DSM_UART=/dev/ttyUSB2 -DM70_BIN=path/m70.bin
```

| Variabile | Default | Note |
|---|---|---|
| `SM_CONFIG` | dal preset | `mx94evk` (Linux + M7) o `mx94alt` (demo SDK) |
| `SM_BIN` | vuoto | SM alternativo da provare, senza verifica sha256 |
| `MSEL` | `0` | passato all'SM: sceglie il blocco `MODE msel=N` del suo cfg |
| `M33S_BIN`, `M70_BIN`, `M71_BIN` | vuoto | binari per i core secondari |
| `ACCEPT_ELE_EULA` | `OFF` | `ON` = hai letto e accetti la EULA NXP; serve senza terminale (Docker, CI) |
| `ELE_IMAGE` | vuoto | container ELE già disponibile, senza download né verifica |
| `SM_UART` | `/dev/ttyUSB3` | UART del debug monitor |
| `JLINK_CORE`, `GDB_ELF`, `ARM_TOOLS` | `M33_0`, vuoto, vuoto | debug |
| `MKIMAGE_TAG`, `MKIMAGE_EXE` | `lf-6.18.20-2.0.0`, vuoto | versione di mkimage, o un mkimage già compilato |
| `SM_TAG`, `SM_CONFIGS` | `lf-6.12.20-2.0.0`, `mx94evk;mx94alt` | per `sm-verify`/`sm-update` |
| `TYPST_VERSION`, `DOCS_VIEWER` | `0.13.1`, vuoto | `DOCS_VIEWER=wslview` apre il PDF in `docs-live` |

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
  dell'**AP**.
- `mx94alt`: una sola LM possiede tutto, ed è quella pensata per i demo del SDK MCUXpresso. Nei test
  l'M7_0 con `alt` è rimasto in `HOLD` anche con `MSEL=4`; si è avviato dal monitor con
  `cpu.w 1 vector 0x303C0000` e `cpu.w 1 start`.

## SM: binari committati e build riproducibile

I binari dell'SM sono in `sm/` e sono quelli che usa `image`. Per verificare come sono stati ottenuti
basta Docker: `make sm-verify` li ricostruisce da sorgenti (toolchain e tag NXP pinnati) e confronta gli
sha256. La build è riproducibile al byte. Dettagli in [sm/README.md](sm/README.md).

## Docker per `image`

`make docker-image` esegue `image` in un container Ubuntu 24.04 (cmake, gcc, git, python3) con il preset
`docker`, riusando il firmware ELE già scaricato sull'host. Il container gira con il tuo uid/gid e
monta il repo in `/work`. Se `docker` dà "permission denied", il tuo utente non ha il gruppo attivo:
`newgrp docker` (o riapri la shell). Flash, UART e debug non girano nel container.

## Limiti noti

- **Provato:** `image` (anche incrementale), `run` con l'SM già in esecuzione per entrambi i preset,
  `docs` con download di typst, `debug` con `GDB_BATCH=1` (backtrace con simboli e ripartenza),
  `docker-image` (identico all'host), `sm-verify`, `ele` con `ACCEPT_ELE_EULA=ON`.
- **Non provati:** `uart`, `docs-live` e `debug` interattivi, `sm-update`. La richiesta interattiva della
  EULA da dentro il build non l'ho provata (ho sempre usato `ACCEPT_ELE_EULA=ON`).
- **`debug`:** l'attach ferma il core. In modalità interattiva bisogna fare `continue` prima di uscire;
  con `GDB_BATCH=1` lo script lo fa ripartire da solo. Su M33_1, M7_0 e M7_1 il J-Link non riesce ad
  accendere il DAP (`Could not power-up system power domain`), e i tentativi hanno resettato la board.
- **Core secondari:** M33S/M7 risultano in `RUN` dal monitor (`cpu.r`), ma la loro UART non è esposta
  dalla USB della EVK: servono un adattatore sui pin UART3/UART4 o un blink su `GPIO2_31` (J44 pin 10).
- **Un download ELE per preset:** ogni `build/<preset>/` scarica il suo firmware ELE.
- **Solo silicio A0:** il container ELE estratto è quello A0.
- **Non implementato:** boot persistente da SD/eMMC (`uuu -b sd <flash_all LF> <flash.bin>` è l'idea) e
  le app dei core come sottoprogetti CMake con il SDK.
