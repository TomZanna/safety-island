# Build e immagine di boot {#build}

## Il superbuild {#superbuild}

`CMakeLists.txt` dichiara `LANGUAGES NONE`: non compila sorgenti, crea solo
target. L'ordine dei sottoprogetti e' una dipendenza reale, non alfabetica:

```text
deps   toolchain Arm + SDK (west)          -> fornisce SI_ARMGCC_DIR, SI_SDK_ROOT
ele    firmware ELE (EULA NXP)             -> fornisce il container AHAB
sm     binari del System Manager           -> fornisce sm_mx94evk.bin (+ patch)
fw     app m33s/m70/m71 (ExternalProject)  -> fornisce i .bin dei tre core
image  mkimage_imx8 + flash.bin            -> unisce ELE, SM e le app
board  flash/run/reset/uart/rtt/debug      -> parla con la board
docs   Doxygen (questo sito)
```

Il `Makefile` e' solo una scorciatoia: `make run` equivale a
`cmake --build --preset mx94evk --target run`, e la prima volta configura da
solo con `cmake --preset mx94evk`.

## Tool e SHA {#tool-sha}

Ogni strumento esterno di progetto e' dichiarato con `si_tool()`
(`cmake/SafetyIsland.cmake`): tabella per host, URL e *sha256*; se il programma
e' gia' nel `PATH` si usa quello, altrimenti viene scaricato in
`ext/tools/<nome>-<versione>/` e verificato al primo uso. Vale per la toolchain
Arm, `uuu`, `bcu` e Doxygen. La toolchain in uso e' anche linkata in
`ext/toolchain`, perche' VS Code e `gdb` la cerchino li'.

Doxygen e' l'eccezione voluta: e' dichiarato con `NO_PATH` e versione **1.18.0**,
quindi il build usa sempre il binario pinnato (non quello di sistema). Il primo
`make docs` scarica ~115 MB in `ext/tools/doxygen-1.18.0/`; i successivi no.
Mermaid e MathJax non sono tool di build: sono caricati dal browser (CDN), e nel
`Doxyfile.in` sono pinnati `mermaid@11.17.2` e `mathjax@3.2.2`.

Il tema `doxygen-awesome-css` **2.5.0** e' committato in
`docs/theme/doxygen-awesome-css/` (MIT) e si disattiva con
`-DDOCS_AWESOME_CSS=OFF`.

## Bootstrap e pulizia {#bootstrap}

`make` configura da solo (`cmake --preset <preset>`); poi ogni sottoprogetto
scarica al primo uso cio' che gli serve: toolchain, `uuu`, `bcu` e Doxygen via
`si_tool()` (`ext/tools/`, sha256), SDK via `west` (`ext/sdk/`), `mkimage` come
`ExternalProject` (`build/_deps/`), ELE dal sito NXP. La lista dei prerequisiti
host, dei tool opzionali e delle eccezioni (EULA, J-Link, Docker, USB/WSL) sta
nel `README.md` ("Cosa serve"); qui c'e' il meccanismo.

I target dichiarano le proprie dipendenze: `rtt` porta il venv (`py-deps`),
`debug-<app>` la toolchain e l'ELF dell'app, `docs` il download di Doxygen. In un
ambiente pulito non serve nessun comando a mano.

La pulizia ha tre livelli, nel `Makefile` e fuori da CMake (non riconfigurano):

| Target | Rimuove | Build successivo |
|---|---|---|
| `make clean` | `build/<preset>/` | riconfigura, nessun download |
| `make distclean` | `build/` (tutti i preset) | riconfigura, nessun download |
| `make clean-all` | `build/`, `ext/`, `.west/` | riscarica toolchain, SDK, venv, Doxygen |

Dopo `clean-all` il build successivo e' la prova del bootstrap da zero. Restano
fuori le immagini Docker (`safety-island-build`, lo SM): sono dell'utente.

## Qualita' del codice {#qualita-build}

Il modulo `quality/` aggiunge target opzionali per `clang-format`,
`clang-tidy` e `cppcheck` (MISRA non certificato). Non sono dipendenze di
`image`/`run`/`docs`: se i tool mancano, solo i target quality falliscono con
l'istruzione di installazione; se la versione e' sotto la minima stampano un
warning. Versioni e comportamento sono in @ref qualita.

## SDK come dipendenza {#sdk}

`deps/west.yml` pinna quattro repo NXP per commit (SDK core, device i.MX,
componenti, CMSIS). `west` li clona shallow in `ext/sdk/mcuxsdk`. Dell'SDK
usiamo volutamente poco: CMSIS, header/startup e `system_*.c` del MIMX94398,
`fsl_clock`, driver `common`/`mu1`/`cache`, i componenti `scmi`/`smt`/`sm_crc`.
Niente `west build`, Kconfig o board files: il linker script e' nostro
(`fw/common/tcm.ld.in`).

## Le app: `imx94_firmware()` {#app}

Ogni app e' un progetto CMake separato (`fw/m70/CMakeLists.txt` e' una riga),
compilato come `ExternalProject` con il toolchain Arm e l'SDK. La funzione
`imx94_firmware()` (`fw/cmake/imx94.cmake`) sceglie CPU, cache, indirizzi dei
TCM, sorgenti SDK, flag e linker. La keyword `ROLE` aggiunge il virtual safe PLC,
linka il codice del ruolo (`fw/safe/` o `fw/fieldbus/`) e la libreria FSoE.

> La libreria FSoE e' un *archivio statico per app* (`libfsoe-m70.a`,
> `-m71.a`, `-m33s.a`): stesso codice, ma `-mcpu` diverso. Il link finale e'
> dell'app.

## `flash.bin` {#flash-bin}

`image/CMakeLists.txt` chiama `mkimage_imx8` (pinnato a un tag NXP) e produce un
unico file:

```text
flash.bin = [container AHAB: ELE] + [container: SM (M33_0) + m33s (M33_1) + m70 (M7_0) + m71 (M7_1)]
             -m33 sm_mx94evk.bin 0 0x1FFC0000
             -m33 m33s.bin 1 0x1FFC0000 0x309C0000
             -m7  m70.bin  0 0x0        0x303C0000
             -m7  m71.bin  1 0x0        0x302C0000
```

Gli indirizzi sono quelli di `iMX94/soc.mak` di mkimage; `-msel` sceglie il
blocco `MODE msel=N` della config dell'SM. `make inspect` (`scripts/ahab_walk.py`)
stampa i container per verificare cosa c'e' dentro.

> `flash.bin` *non inizializza la DDR*. Per questo codice e dati dei core
> secondari stanno nei TCM e la memoria condivisa sta in OCRAM: un accesso in
> DDR prima dell'inizializzazione e' un HardFault. Il nostro linker script
> disattiva anche l'azzeramento della resource table RPMsg che l'SDK fa in DDR.
