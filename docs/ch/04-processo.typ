#import "../lib/style.typ": *
= Il processo di sviluppo

== Il ciclo

Il ciclo di lavoro è volutamente corto, perché il collo di bottiglia è sempre il tempo fra "ho cambiato una
riga" e "ho visto cosa succede in board":

```bash
# 1. modifica fw/... (una o più app, o fw/common)
make image     # ricompila solo ciò che è cambiato (~5 s) e rifà flash.bin
make run       # reset + uuu + boot (~10 s)
make rtt       # guarda i log dei tre core (Ctrl-C per uscire)
```

Se il cambiamento tocca la configurazione dell'SM (memoria, pin, clock) o le patch SM:

```bash
# 2. modifica sm/patches/*.patch
make sm-update # ricostruisce l'SM nel container Docker, aggiorna sm/*.bin + BUILDINFO
make run       # prova sulla board PRIMA di committare
```

== Regole che tengono in piedi tutto

- *prova in board prima del commit*: vale per l'SM (che è binario) e per il firmware. Un commit che non è
  stato flashato è un commit che non sappiamo se funziona;
- *l'SDK non si tocca*: se serve un comportamento diverso si passa da `fw/common` o dal manifest;
- *le patch SM sono la traccia*: ogni modifica alla config NXP vive in `sm/patches/`, numerata, con una
  riga di spiegazione e lo sha256 in `BUILDINFO`;
- *niente `.exe`*: in WSL si usa `uuu` Linux pinnato; `-DUUU=uuu.exe` è solo un ripiego;
- *attach sfasati*: i GDB server dei compound partono a 0,5/1 s l'uno dall'altro (`Attendi …` in
  `tasks.json`); se partono insieme, gdb può fallire l'attach. J-Link usa un GDB server per core (la
  sonda è condivisa a livello di link).

== Riproducibilità

Tre livelli, tutti verificabili:

1. *tool e SDK*: versione + sha256 in `cmake/SafetyIsland.cmake` e `deps/west.yml`; aggiornare significa
   cambiare uno SHA e ricompilare;
2. *SM*: `make sm-verify` ricostruisce l'SM nel container (`sm/Dockerfile`) con toolchain pinnata, tag
   NXP pinnato, `SOURCE_DATE_EPOCH` = data del commit e percorsi fissi, poi confronta i byte con `sm/`;
3. *documento*: `docs/lib/packages.typ` pinna i package typst (unico posto da toccare) e
   `docs/CMakeLists.txt` pinna typst stesso.

#nota[
  Il primo build di `docs` richiede rete due volte: `si_tool` scarica typst (17 MB) e il compilatore scarica
  i package in `~/.cache/typst/packages`. Per build offline si copia quella directory e si passa
  `-DDOCS_PACKAGE_PATH=<dir>`: `typst` la usa come package path (`--package-path`).
]

== Come è stato validato il virtual safe PLC

Le misure citate nella seconda parte vengono da run reali su questa EVK, registrate in @sec-misure:
10 600 cicli di barrier senza timeout in un run pulito, 3 900 cicli in stato `Data` con `anom 0`, e la
matrice di fault injection con la reazione attesa per ogni guasto. Ogni commit del firmware è stato
preceduto da un `make run` + `make rtt` con quei numeri sotto controllo.
