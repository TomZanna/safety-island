#import "../lib/style.typ": *
= Build e immagine di boot

== Il superbuild

`CMakeLists.txt` dichiara `LANGUAGES NONE`: non compila sorgenti, crea solo target. L'ordine dei
sottoprogetti è una dipendenza reale, non alfabetica:

```text
deps   toolchain Arm + SDK (west)          → fornisce SI_ARMGCC_DIR, SI_SDK_ROOT
ele    firmware ELE (EULA NXP)             → fornisce il container AHAB
sm     binari del System Manager           → fornisce sm_mx94evk.bin (+ patch)
fw     app m33s/m70/m71 (ExternalProject)  → fornisce i .bin dei tre core
image  mkimage_imx8 + flash.bin            → unisce ELE, SM e le app
board  flash/run/reset/uart/rtt/debug      → parla con la board
docs   typst (questo documento)
```

Il `Makefile` è solo una scorciatoia: `make run` equivale a `cmake --build --preset mx94evk --target run`,
e la prima volta configura da solo con `cmake --preset mx94evk`.

== Tool e SHA

Ogni strumento esterno è dichiarato con `si_tool()` (`cmake/SafetyIsland.cmake`): tabella per host,
URL e *sha256*; se il programma è già nel `PATH` si usa quello, altrimenti viene scaricato in
`ext/tools/<nome>-<versione>/` e verificato al primo uso. Vale per la toolchain Arm, `uuu`, `bcu` e
`typst`. La toolchain in uso è anche linkata in `ext/toolchain`, perché VS Code e `gdb` la cerchino lì.

#attenzione[
  Il pin di `typst` in `docs/CMakeLists.txt` è *0.15.1*: i package di questo documento (cetz 0.5.x,
  codly 2.x) richiedono typst ≥ 0.15. Cambiare versione significa aggiornare *anche* lo sha256 nella
  stessa `set(...)`, altrimenti il download viene rifiutato.
]

== SDK come dipendenza

`deps/west.yml` pinna quattro repo NXP per commit (SDK core, device i.MX, componenti, CMSIS). `west`
li clona shallow in `ext/sdk/mcuxsdk`. Dell'SDK usiamo volutamente poco: CMSIS, header/startup e
`system_*.c` del MIMX94398, `fsl_clock`, driver `common`/`mu1`/`cache`, i componenti `scmi`/`smt`/`sm_crc`.
Niente `west build`, Kconfig o board files: il linker script è nostro (`fw/common/tcm.ld.in`).

== Le app: `imx94_firmware()`

Ogni app è un progetto CMake separato (`fw/m70/CMakeLists.txt` è una riga), compilato come
`ExternalProject` con il toolchain Arm e l'SDK. La funzione `imx94_firmware()` sceglie CPU, cache, indirizzi
dei TCM, sorgenti SDK, flag e linker. La keyword `FLOW_ROLE` aggiunge il virtual safe PLC e la libreria FSoE:

#si-listing(
  caption: [La keyword `FLOW_ROLE` di `imx94_firmware()`: sorgenti, include, `FLOW_CORE_ID` e libreria FSoE],
  src: "/fw/cmake/imx94.cmake",
  lines: (76, 122),
)

#nota[
  La libreria FSoE è un *archivio statico per app* (`libfsoe-m70.a`, `-m71.a`, `-m33s.a`): stesso codice,
  ma `-mcpu` diverso. Il link finale è dell'app, come chiede la traccia di progetto ("la libreria si linka,
  non si ingloba").
]

== `flash.bin`

`image/CMakeLists.txt` chiama `mkimage_imx8` (pinnato a un tag NXP) e produce un unico file:

```text
flash.bin = [container AHAB: ELE] + [container: SM (M33_0) + m33s (M33_1) + m70 (M7_0) + m71 (M7_1)]
             -m33 sm_mx94evk.bin 0 0x1FFC0000
             -m33 m33s.bin 1 0x1FFC0000 0x309C0000
             -m7  m70.bin  0 0x0        0x303C0000
             -m7  m71.bin  1 0x0        0x302C0000
```

Gli indirizzi sono quelli di `iMX94/soc.mak` di mkimage; `-msel` sceglie il blocco `MODE msel=N` della
config dell'SM. `make inspect` (`scripts/ahab_walk.py`) stampa i container per verificare cosa c'è dentro.

#attenzione[
  `flash.bin` *non inizializza la DDR*. Per questo codice e dati dei core secondari stanno nei TCM e la
  memoria condivisa sta in OCRAM: un accesso in DDR prima dell'inizializzazione è un HardFault. Il nostro
  linker script disattiva anche l'azzeramento della resource table RPMsg che l'SDK fa in DDR.
]
