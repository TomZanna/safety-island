# Note per agenti (e persone) che lavorano su safety-island

Contesto: i.MX94 EVK, silicio **A0**, sviluppo da **WSL**. Leggi prima `README.md`.

## Ambiente e USB
- I dispositivi USB arrivano in WSL con il client Linux `usbip` dal server usbipd-win (niente `.exe`):
  servizio systemd `safety-island-usb` (`scripts/usb-attach.sh`), che li attacca per VID:PID
  (`board/usb-devices.conf`) e li ri-attacca quando ricompaiono. Stato: `make usb-status`.
- Se un dispositivo sparisce da WSL (es. il J-Link dopo un GDB server ucciso a metà) senza il servizio
  attivo, va ri-attaccato; col servizio torna da solo in ~1 s.
- `uuu` è quello Linux pinnato (`ext/tools/uuu-*`); la ROM SDP (`1fc9:0027`) deve essere in WSL.
  `-DUUU=uuu.exe` solo come ripiego.
- Reset: `scripts/reset.sh`, default hardware via pin nRESET del J-Link (`JLinkExe` `r0`/`r1`, nessun
  core coinvolto): non dipende dall'SM. Il monitor SM (`reset`) è solo l'ultima risorsa. BCU è l'alternativa
  hardware (profilo `imx943evk19b1`, SW7-1 OFF: con SW7-1 sbagliato `get gpio failed`): non passa dalla
  boot ROM, comanda reset e pin BOOT_MODE via expander, quindi recupera anche un boot media con
  immagine valida ma bloccata. `make run` ci ripiega se dopo il reset J-Link la ROM non compare.
- `uuu -b sd/emmc` passa per ROM `1fc9:0027`, SPL `1fc9:0151`, fastboot `1fc9:0152`: tutti devono essere in
  `board/usb-devices.conf` (e condivisi su Windows), altrimenti uuu aspetta all'infinito.
- Seriali sempre per `/dev/serial/by-id/` (l'ordine dei `ttyUSBn` cambia). Monitor SM: FTDI quad `if03`
  (LPUART2): `reset` riporta la board in SDP, `cpu.r` mostra lo stato dei core. Se il monitor non
  risponde, la board è quasi certamente tornata in SDP: `make run`.

## Firmware e dipendenze
- App in `fw/<app>/` (m33s, m70, m71), codice comune in `fw/common/`. Build: ExternalProject per app
  (`fw/CMakeLists.txt`) con `imx94_firmware()` (`fw/cmake/imx94.cmake`).
- `fw/common/flow/` è **codice nostro** (virtual safe PLC, sezione dedicata sotto). `fw/common/fsoe/` è la
  libreria rt-labs vendored: non si modifica, si compila com'è (senza `FSoEappl/`), con il port in
  `fw/common/flow/port/`.
- Documentazione typst: `docs/CMakeLists.txt` pinna typst (oggi **0.15.1** + sha256) e
  `docs/lib/packages.typ` pinna i package (cetz, codly, codly-languages, hydra, glossarium). Le versioni si
  cambiano **solo in quei due file**; i capitoli non importano mai `@preview/...`.
- SDK MCUXpresso minimo **solo** via `deps/west.yml` (4 repo pinnati per SHA, in `ext/sdk/`). Non copiare
  file dell'SDK nel repo e non modificarli in `ext/`: si perde la tracciabilità. Se serve cambiare un
  comportamento SDK, farlo da `fw/common` (es. `SystemInitHook`, linker script) o aggiornare il manifest.
- Toolchain e strumenti: `si_tool()` in `cmake/SafetyIsland.cmake` (tabella per host + sha256, in
  `ext/tools/`). `ext/toolchain` è un link alla toolchain in uso (lo usano VS Code e gdb).
- `flash.bin` non inizializza la DDR: niente codice/dati in DDR sui core secondari. Il SDK azzera la
  resource table RPMsg (in DDR per gli M7) in `SystemInit()`: il nostro `tcm.ld.in` la disattiva con
  `__RscTblShareWithMpuStart > __RscTblShareWithMpuEnd`.
- Log/input delle app: RTT con **un control block condiviso** in OCRAM (`0x204D0000`, patch SM) e un
  canale per core (0 m33s, 1 m70, 2 m71), `fw/common/rtt_shared.c` (non `SEGGER_RTT.c`: la sua init
  cancella i canali degli altri core). Motivo: il motore RTT del J-Link è uno per sonda (verificato: con
  un control block per core, più sessioni leggevano tutte quello del primo core). Lettura con
  `scripts/rtt.py` (pylink): più processi su canali diversi ok, due sullo stesso canale si rubano i
  dati. Cortex-Debug `rttConfig` è usato: **un canale per sessione** (`port` 0/1/2 in `launch.json`).
- **J-Link: max 5 connessioni contemporanee.** Ogni GDB server e ogni console `make rtt` è una
  connessione (in debug l'RTT passa dal `rttConfig`, senza aggiungerne): con 3 GDB server restano 2
  slot. Un GDB server per core (il server è single-core; la sonda è condivisa a livello di link).
  L'hub TCP (19100-19102) è stato rimosso: non reintrodurlo.
- `rtt.py` non riprova: se la sonda è occupata o le connessioni sono esaurite stampa un errore e
  termina. `make run` chiude le console `make rtt` prima del reset e, se il caricamento fallisce,
  ricorda di chiudere eventuali altre sessioni J-Link. I compound sfasano gli attach di 0,5/1 s
  (`Attendi …`): se gdb parte insieme, la corsa d'avvio fa fallire l'attach. Un GDB server che si
  collega ferma il core finché gdb non lo fa ripartire. `g_debugger_go` deve restare nell'ELF
  (riferita in `SystemInitHook`): se gdb non la trova, il `postAttachCommands` fallisce e l'attach di
  Cortex-Debug si blocca. Le app non usano periferiche.
- Con l'SM, pin mux e clock passano da SCMI. Il `fsl_iomuxc.h` del SDK, se l'SM rifiuta, resta in
  `while (result != SCMI_ERR_SUCCESS);`: se servono periferiche, chiamare `SCMI_PinctrlSettingsConfigure`
  con gestione dell'errore (vedi README "Console seriali", codice nella history git).

## Virtual safe PLC (`fw/common/flow/`)

Regole che non si possono sbagliare. Il perché sta in `docs/` (`make docs`).

- Ruoli: **m70 = SAFE_A**, **m71 = SAFE_B** (1oo2), **m33s = FIELDBUS** (fa l'A55: slave FSoE +
  campo). `FLOW_CORE_ID` è deciso da `FLOW_ROLE` in `imx94.cmake`; i tre ruoli compilano la stessa
  sorgente.
- **Non esiste una MU fra M33S e i due M7** (`mx94evk.cfg`: i lati MU sono cablati). Il cross-check è
  solo M7_0 ↔ M7_1 via **MU18** (lato B su M7_0, lato A su M7_1) + payload in OCRAM. Non si "risolve" con
  una patch SM: semmai si cambia il trasporto (ICC), non la logica.
- La MU18 ha 4 TR + 4 RR da 32 bit (**16 B** per direzione): porta solo il doorbell
  `{seq, tag, channel}` + CRC-32 del payload. Il messaggio `flow_mu_message_t` (72 B) sta in
  `flow_icc`; il ricevente **ricalcola** il CRC e lo confronta con quello arrivato via MU.
- **Barrier**: il numero di ciclo lo scrive **solo il FIELDBUS** (`flow_icc_cycle_release`); i core safe
  pubblicano `ready[me]` e aspettano che `cycle` cambi. C'è un **gate d'avvio** (il ciclo non avanza
  finché entrambi non hanno chiuso il primo) e un timeout di 30 ms: senza il gate i due core partono con
  numeri di ciclo diversi e i cross-check falliscono per sempre.
- **Una RX per ciclo safe**: il fieldbus pubblica la risposta dello slave una volta sola, prima di aprire
  il ciclo successivo. Se la pubblichi a 2 ms i due core leggono frame diversi.
- Taglie e offset sono protetti da `_Static_assert`: se cambi una struttura aggiorna anche l'assert e
  `docs/ap/appendici.typ`.
- `flow_platform_mu_post()` deve trasportare **l'intero** `flow_mu_message_t` (72 B): lo slot ICC non è
  dimensionato sul payload.
- Un attach SWD / `make rtt` ferma il core e fa scadere barrier + watchdog FSoE: **è il fail-safe che
  funziona** (poi si riconnette). Non "aggiustarlo": per un run pulito non ci si attacca.
- `WAIT_DEBUGGER` è OFF nella cache (serve al ciclo flash→RTT): per riattivarlo
  `cmake --preset mx94evk -D<APP>_WAIT_DEBUGGER=ON`.
- Comandi RTT: `p i l x` sui core safe (fault injection), `s d` sul fieldbus (discrepanza, drop frame),
  `?` per lo stato. Vedi il README.
- Per aggiungere codice: file in `fw/common/flow/` + `target_sources` in `fw/cmake/imx94.cmake`. Le
  finestre OCRAM stanno nella patch SM `0002`: cambiarle richiede `sm/patches/` + `make sm-update` +
  prova in board prima del commit.
- Non portare qui il driver Linux di `imx-flow` (`flow_a55_drv.c`): il ruolo A55 è m33s. Se servisse, va
  allineato all'ABI v2 (heartbeat per tre ruoli).

## Debug J-Link: cose da sapere
- **LPUART8 = pin SWD.** LPUART8 è muxata su `DAP_TCLK_SWCLK`/`DAP_TMS_SWDIO`. Qualsiasi immagine che la usi
  (hello_world SDK dell'M33S di default, demo LF dell'M33S) rende il DAP irraggiungibile. J-Link allora
  scrive `Could not power-up system power domain` / `Target voltage too low (0.0 V)` e il suo ripiego
  "connect under reset" **resetta tutta la board**, anche se ti stai collegando a M33_0.
- I device SEGGER standard `MIMX94398_{M33_0,M33_1,M7_0,M7_1}` funzionano senza JLinkScript. Su M33_1/M7_x
  solo attach: l'app va caricata prima con `make run`.
- `DHCSR.C_DEBUGEN` resta a 1 dopo una sessione di debug anche attraverso il reset della board: non usarlo
  per capire se c'è un debugger. `WAIT_DEBUGGER` aspetta `g_debugger_go` (in `SystemInitHook`).
- Prima di dare la colpa ai permessi dell'SM (TRDC): il DAP ha DID 9 con accesso ovunque (`DEBUG did=9` in
  `devices/MIMX94/configtool/device.cfg` di imx-sm); il problema visto finora era sempre il pin mux.
- `make debug` è un core alla volta (una sola porta GDB); il compound di VS Code apre 3 sessioni, ognuna
  con il suo GDB server. Dopo un attach in `-batch` assicurati che parta `monitor go`: se un comando gdb
  fallisce, il batch si interrompe e il core resta fermo.
- Con il virtual safe PLC attivo un attach (o `make rtt`) **ferma il core** e fa scadere barrier e watchdog
  FSoE: si vede `barrier timeout` + `WD_EXPIRED` e poi la riconnessione. È il comportamento atteso.

## SM, ELE e config
- SM e ELE vanno in coppia: `SM_ELE_VER` in `sm/makefiles/build_info.mak` del tag SM = versione del
  pacchetto `firmware-ele-imx`. Oggi: SM `lf-6.18.20-2.0.0` + ELE 2.0.6 (contiene ancora `mx943a0`).
- Le config del checkout locale `~/imx-sm` possono essere di un tag diverso da quello in `sm/`: controlla
  sempre la config del tag in uso (`build/_deps/imx-sm-<tag>/configs/` o `git show <tag>:configs/...`).
  Esempio: in lf-6.12.20 LPUART3 era dell'AP, da lf-6.12.49 è dell'M33S.
- La config `mx94evk` dell'SM è quella NXP più `sm/patches/` (oggi: OCRAM condivisa per RTT). Per cambiare
  proprietà di periferiche/pin: nuova patch al `.cfg` lì, `make sm-update`, prova sulla board.
- Ricompilare l'SM: `make sm-update` (Docker riproducibile), poi `make run` sulla board prima di committare.
