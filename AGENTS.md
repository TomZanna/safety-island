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
  dati. Cortex-Debug `rttConfig` con J-Link vede un solo canale: non usato. La sonda accetta
  **max 5 connessioni** contemporanee ("Connecting to J-Link failed" sulla sesta): per questo l'RTT passa
  da un hub (`rtt.py hub`, una connessione, canali su TCP 19100-19102) a cui si collegano i terminali.
  Aperture della sonda nello stesso istante vengono rifiutate: `rtt.py` riprova, i GDB server di VS Code
  partono da `scripts/jlink-gdbserver.sh`, che riprova. Un GDB server che si collega ferma il core
  finché gdb non lo fa ripartire. `g_debugger_go` deve restare nell'ELF (riferita in `SystemInitHook`):
  se gdb non la trova, il `postAttachCommands` fallisce e l'attach di Cortex-Debug si blocca. Le app non usano periferiche.
- Con l'SM, pin mux e clock passano da SCMI. Il `fsl_iomuxc.h` del SDK, se l'SM rifiuta, resta in
  `while (result != SCMI_ERR_SUCCESS);`: se servono periferiche, chiamare `SCMI_PinctrlSettingsConfigure`
  con gestione dell'errore (vedi README "Console seriali", codice nella history git).

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
- Un core alla volta. Dopo un attach in `-batch` assicurati che parta `monitor go`: se un comando gdb
  fallisce, il batch si interrompe e il core resta fermo.

## SM, ELE e config
- SM e ELE vanno in coppia: `SM_ELE_VER` in `sm/makefiles/build_info.mak` del tag SM = versione del
  pacchetto `firmware-ele-imx`. Oggi: SM `lf-6.18.20-2.0.0` + ELE 2.0.6 (contiene ancora `mx943a0`).
- Le config del checkout locale `~/imx-sm` possono essere di un tag diverso da quello in `sm/`: controlla
  sempre la config del tag in uso (`build/_deps/imx-sm-<tag>/configs/` o `git show <tag>:configs/...`).
  Esempio: in lf-6.12.20 LPUART3 era dell'AP, da lf-6.12.49 è dell'M33S.
- La config `mx94evk` dell'SM è quella NXP più `sm/patches/` (oggi: OCRAM condivisa per RTT). Per cambiare
  proprietà di periferiche/pin: nuova patch al `.cfg` lì, `make sm-update`, prova sulla board.
- Ricompilare l'SM: `make sm-update` (Docker riproducibile), poi `make run` sulla board prima di committare.
