#import "../lib/style.typ": *
= La repo e i suoi componenti

La repo è un *superbuild*: non compila niente direttamente, orchestra tool esterni (toolchain, `west`,
`mkimage`, `uuu`, `J-Link`, `typst`) e i sottoprogetti del firmware. Tutto ciò che è scaricato finisce in
`ext/` o `build/`, entrambi gitignored: nel repo restano solo sorgenti, script, configurazioni e i binari
dell'SM (che sono la "sorgente di verità" del boot).

== Albero

```text
safety-island/
  CMakeLists.txt        superbuild: deps → ele → sm → fw → image → board → docs
  CMakePresets.json     preset mx94evk | mx94alt | docker
  Makefile              scorciatoia: make <target> = cmake --build --preset
  deps/                 west.yml: 4 repo SDK NXP pinnati per SHA (in ext/sdk)
  ele/                  firmware ELE NXP: URL + sha256, scaricato (EULA)
  sm/                   System Manager: binari committati + patches/ + Dockerfile
  fw/                   firmware dei core secondari (m33s, m70, m71) + common
  image/                mkimage → flash.bin (AHAB + SM + app)
  board/                target make: run, reset, uart, rtt, debug, usb-*
  scripts/              script bash/python usati dai target
  docs/                 questo documento (typst)
  ext/                  toolchain, SDK, strumenti, venv (gitignored)
  build/<preset>/       uscite (gitignored): flash.bin, elf/bin delle app, pdf
```

== Componenti, uno per uno

#si-table(
  columns: (1.1fr, 2.6fr, 1.5fr, 1.9fr),
  header: ([Componente], [Cosa fa], [Committato?], [Come si produce]),
  rows: (
    ([`deps/`], [pinna toolchain Arm e SDK MCUXpresso minimo], [solo `west.yml`], [`make deps`]),
    ([`ele/`], [fornisce il firmware ELE (`mx943a0-ahab-container.img`)], [no (EULA NXP)], [`make ele`]),
    ([`sm/`], [binari del System Manager + patch locali + Dockerfile], [sì (`.bin`, `SHA256SUMS`)], [`make sm-update` / `sm-verify`]),
    ([`fw/`], [le tre app dei core secondari e il codice comune], [sì], [`make fw` / `make image`]),
    ([`image/`], [impacchetta AHAB + SM + app in `flash.bin`], [sì (solo CMake)], [`make image`]),
    ([`board/`], [target che parlano con la board], [sì], [(nessuna uscita)]),
    ([`scripts/`], [implementazione dei target: USB, uuu, RTT, gdb, SM in Docker], [sì], [-]),
    ([`docs/`], [questo documento], [sì], [`make docs`]),
  ),
)

#nota[
  La regola d'oro: *l'SDK non si copia e non si modifica* dentro `ext/`. Se serve un comportamento
  diverso, si interviene da `fw/common` (per esempio `SystemInitHook`, linker script) o si aggiorna il
  manifest `deps/west.yml` cambiando gli SHA.
]

== Gli script che contano

#si-table(
  columns: (1.9fr, 3.2fr),
  header: ([Script], [Ruolo]),
  rows: (
    ([`scripts/common.sh`], [funzioni condivise; default delle variabili, `die`, `info`]),
    ([`scripts/deps.sh`], [crea il venv (`west` + `pylink`) e fa `west update`]),
    ([`scripts/fetch-ele.sh`], [scarica il pacchetto ELE, verifica lo sha256, estrae il container]),
    ([`scripts/sm-docker.sh`], [ricostruisce l'SM nel container e confronta/copia i binari]),
    ([`scripts/flash.sh`], [reset + `uuu SDPS: boot`, con fallback BCU]),
    ([`scripts/reset.sh`], [J-Link (`r0`/`r1`) → BCU → monitor SM, in quest'ordine]),
    ([`scripts/uart.sh`], [console seriale del monitor SM (`picocom`)]),
    ([`scripts/rtt.py`], [lettura/scrittura RTT via `pylink`, una connessione per terminale (errore se la sonda è occupata)]),
    ([`scripts/debug.sh`], [GDB server + gdb su un core, `GDB_BATCH=1` per l'attach non interattivo]),
    ([`scripts/usb-attach.sh`], [servizio `safety-island-usb`: attacca/riattacca i dispositivi via usbip]),
  ),
)
