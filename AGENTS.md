# Note per agenti (e persone) che lavorano su safety-island

Contesto: i.MX94 EVK, silicio **A0**, sviluppo da **WSL**. Qui solo regole e rimandi: l'uso è nel
`README.md`, il perché (architettura, protocolli, diagnosi) in `docs/` (`make docs`). Se una cosa si può
dedurre da codice o documentazione, non si duplica in questo file.

## Dove sta la verità

| Cosa | Dove |
|---|---|
| Comandi, target, ambiente WSL/USB, problemi d'uso | `README.md` |
| Architettura, ruoli, protocolli, tempi, diagnosi, limiti | `docs/ch/*.typ` |
| Layout delle strutture e mappa dei file del virtual safe PLC | `docs/ap/appendici.typ` |
| Dettagli operativi di uno script | intestazione dello script |
| Invarianti e scelte di un modulo | commento nel `.c`/`.h` del modulo |
| Regole per chi lavora qui | questo file |

## Linee guida di scomposizione (fw e resto del repo)

- **Una responsabilità, un posto.** Codice di un ruolo in `fw/safe/` o `fw/fieldbus/`; `fw/common/` contiene
  solo ciò che serve identico a più ruoli e non conosce il ruolo. Struttura e mappa: `docs/ch/10`.
- **La variabilità ha un solo punto di dispatch.** fw: `ROLE` in `imx94_firmware()` + il `main.c`
  dell'app; script: `scripts/common.sh`; tool/versioni: `si_tool()`, `deps/west.yml`,
  `docs/lib/packages.typ`; SM: `sm/patches/`. Fuori da lì niente `#if` o copie con piccole differenze.
- **Niente `#if` sul ruolo per scegliere codice.** Il ruolo è un argomento del punto di ingresso
  (`safe_main(FLOW_CORE_SAFE_A)`, `fieldbus_main()`), non una macro che accende pezzi di file.
- **Astrai al secondo uso reale, non prima.** Nel codice safe ogni indirezione in più è costo di review.
- **Il dettaglio vive vicino a chi lo usa**: invariante → commento nel `.c`/`.h`; uso → README;
  architettura e perché → docs typst; operatività → intestazione dello script. Una cosa nuova si scrive nel
  posto giusto, non qui.

## Regole non negoziabili

- **SDK**: minimo, solo via `deps/west.yml` (4 repo pinnati per SHA, in `ext/sdk/`). Non copiare file
  dell'SDK nel repo e non modificarli in `ext/`: si perde la tracciabilità. Se serve un comportamento
  diverso, si interviene da `fw/common` (es. `SystemInitHook`, linker script) o si aggiorna il manifest.
- **FSoE**: `fw/common/fsoe/` è la libreria rt-labs vendored: non si modifica, si compila com'è (senza
  `FSoEappl/`); il port è in `fw/common/flow/port/`.
- **Versioni pinnate** solo in `cmake/SafetyIsland.cmake` (toolchain/strumenti, sha256), `deps/west.yml`
  (SDK), `docs/CMakeLists.txt` (typst **0.15.1**) e `docs/lib/packages.typ` (package typst). I capitoli non
  importano mai `@preview/...`. `ext/toolchain` è un link alla toolchain in uso (VS Code e gdb).
- **Niente DDR** sui core secondari: `flash.bin` non la inizializza. Il linker script è
  `fw/common/tcm.ld.in`; disattiva l'azzeramento della resource table RPMsg del SDK in `SystemInit()`.
- **Periferiche**: le app non ne usano. Con l'SM, pin mux e clock passano da SCMI; se servono, gestire
  l'errore (`SCMI_PinctrlSettingsConfigure`), non lasciare il `while` di `fsl_iomuxc.h` (README,
  "Console seriali").
- **Script**: fail fast (niente retry o attese "di precauzione": se un comando può fallire, esce subito
  dicendo cosa fare) e niente chiusura di processi/sessioni non propri. Wait solo dove servono, con timeout.
- **SM ed ELE in coppia**: `SM_ELE_VER` del tag SM = versione del pacchetto `firmware-ele-imx`. Oggi SM
  `lf-6.18.20-2.0.0` + ELE 2.0.6. La config `mx94evk` è quella NXP più `sm/patches/`: per cambiarla,
  patch, `make sm-update`, prova in board prima del commit. Dettagli: `sm/README.md`.
- **Config del checkout locale** (`~/imx-sm`) possono essere di un tag diverso: controlla sempre
  `build/_deps/imx-sm-<tag>/configs/` o `git show <tag>:configs/...`.

## RTT

Control block unico in OCRAM (`0x204D0000`), un canale per core: 0 m33s, 1 m70, 2 m71. Dettagli in
`docs/ch/03` §RTT, `fw/common/rtt_shared.[ch]`, `scripts/rtt.py`. Due lettori sullo stesso canale si
rubano i dati: un `make rtt` per connessione (la sonda ne accetta 5); l'hub TCP è stato rimosso, non
reintrodurlo.

## Virtual safe PLC: invarianti che rompono tutto

Il perché e i protocolli sono in `docs/ch/10`, `ch/11`, `ch/12`; taglie e offset in
`docs/ap/appendici.typ`. Qui solo ciò che non si può sbagliare.

- Ruoli: **m70 = SAFE_A**, **m71 = SAFE_B** (1oo2), **m33s = FIELDBUS** (fa l'A55: slave FSoE + campo).
- **Non esiste una MU fra M33S e i due M7**: il cross-check è solo M7_0 ↔ M7_1 via **MU18** (lato B su
  M7_0, lato A su M7_1) + payload in OCRAM. Non si "risolve" con una patch SM.
- La MU18 ha 16 B per direzione: porta solo il doorbell `{seq, tag, channel}` + CRC-32 del payload. Il
  messaggio `flow_mu_message_t` (72 B) sta in `flow_icc`; il ricevente **ricalcola** il CRC.
- **Barrier**: il numero di ciclo lo scrive **solo il FIELDBUS**; i core safe pubblicano `ready[me]` e
  aspettano che `cycle` cambi. Ci sono il gate d'avvio e un timeout di 30 ms.
- **Una RX per ciclo safe**: il fieldbus pubblica la risposta dello slave una volta sola, prima di aprire
  il ciclo successivo.
- Taglie e offset sono protetti da `_Static_assert`: se cambi una struttura aggiorni l'assert **e**
  `docs/ap/appendici.typ`.
- Le finestre OCRAM stanno nella patch SM `0002`: cambiarle richiede `sm/patches/` + `make sm-update` +
  prova in board prima del commit.
- Un attach SWD / `make rtt` ferma il core e fa scadere barrier + watchdog FSoE: **è il fail-safe che
  funziona** (poi si riconnette). Non "aggiustarlo": per un run pulito non ci si attacca.
- `WAIT_DEBUGGER` è OFF nella cache: per riattivarlo `cmake --preset mx94evk -D<APP>_WAIT_DEBUGGER=ON`.
- Comandi RTT (`p i l x` sui safe, `s d` sul fieldbus) e significato dei log: `docs/ch/14`.

## Debug J-Link: due cose

- **LPUART8 = pin SWD**: un'immagine che la usa rende il DAP irraggiungibile e il ripiego "connect under
  reset" del J-Link resetta tutta la board. Device e dettagli: `docs/ch/03`.
- **Un attach ferma il core**: con il virtual safe PLC fa scadere barrier e watchdog FSoE, è atteso
  (`docs/ch/14`). `g_debugger_go` deve restare nell'ELF, è referenziata in `SystemInitHook` (`board.c`).

## Ambiente e USB

- Tutto in WSL: USB via usbip (server usbipd-win) + servizio `safety-island-usb`
  (`scripts/usb-attach.sh`, VID:PID di `board/usb-devices.conf`). Setup: `make usb-setup`; stato:
  `make usb-status`; dettagli in `scripts/usb-*.sh`.
- `uuu` è quello Linux pinnato (`ext/tools/uuu-*`); la ROM SDP (`1fc9:0027`) deve essere in WSL.
- Seriali sempre per `/dev/serial/by-id/`; monitor SM sulla FTDI `if03` (LPUART2).
- Reset: `scripts/reset.sh` (jlink → bcu → monitor); dettagli nell'intestazione dello script.
