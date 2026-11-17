# Il processo di sviluppo {#processo}

## Il ciclo {#ciclo}

Il ciclo di lavoro e' volutamente corto, perche' il collo di bottiglia e' sempre
il tempo fra "ho cambiato una riga" e "ho visto cosa succede in board":

```bash
# 1. modifica fw/... (una o piu' app, o fw/common)
make image     # ricompila solo cio' che e' cambiato (~5 s) e rifa' flash.bin
make run       # reset + uuu + boot (~10 s)
make rtt       # guarda i log dei tre core (Ctrl-C per uscire)
```

Da un clone pulito lo stesso ciclo parte con `make usb-setup` (una volta, con
`sudo`) e `make run`: `make clean-all && make run` e' il test che non serva
nessun passaggio manuale.

Se il cambiamento tocca la configurazione dell'SM (memoria, pin, clock) o le
patch SM:

```bash
# 2. modifica sm/patches/*.patch
make sm-update # ricostruisce l'SM nel container Docker, aggiorna sm/*.bin + BUILDINFO
make run       # prova sulla board PRIMA di committare
```

## Regole che tengono in piedi tutto {#regole}

- *prova in board prima del commit*: vale per l'SM (che e' binario) e per il
  firmware. Un commit che non e' stato flashato e' un commit che non sappiamo se
  funziona;
- *l'SDK non si tocca*: se serve un comportamento diverso si passa da
  `fw/common` o dal manifest;
- *le patch SM sono la traccia*: ogni modifica alla config NXP vive in
  `sm/patches/`, numerata, con una riga di spiegazione e lo sha256 in
  `BUILDINFO`;
- *attach sfasati*: i GDB server dei compound partono a 0,5/1 s l'uno dall'altro
  (`Attendi ...` in `tasks.json`); se partono insieme, gdb puo' fallire
  l'attach. J-Link usa un GDB server per core (la sonda e' condivisa a livello
  di link).

## Riproducibilita' {#riproducibilita}

Tre livelli, tutti verificabili:

1. *tool e SDK*: versione + sha256 in `cmake/SafetyIsland.cmake` e
   `deps/west.yml`; aggiornare significa cambiare uno SHA e ricompilare;
2. *SM*: `make sm-verify` ricostruisce l'SM nel container (`sm/Dockerfile`) con
   toolchain pinnata, tag NXP pinnato, `SOURCE_DATE_EPOCH` = data del commit e
   percorsi fissi, poi confronta i byte con `sm/`;
3. *documentazione*: Doxygen 1.18.0 + sha256 in `docs/CMakeLists.txt`, tema
   `doxygen-awesome-css` 2.5.0 committato in `docs/theme/`, e Mermaid/MathJax
   pinnati nel `Doxyfile.in` (`mermaid@11.17.2`, `mathjax@3.2.2`);
4. *qualita'*: versioni minime di clang-format/clang-tidy/cppcheck in
   `quality/CMakeLists.txt`, con warning se l'host e' piu' vecchio (dettagli
   in @ref qualita).

> Il primo `make docs` scarica Doxygen (~115 MB) in `ext/tools/`; Mermaid e
> MathJax sono caricati dal browser, non servono al build. La consultazione
> offline del sito richiede il vendoring dei due runtime: non e' il default.

## Come e' stato validato il virtual safe PLC {#validazione}

Le misure citate nella documentazione vengono da run reali su questa EVK,
registrate in @ref misure 10 600 cicli di barrier senza timeout in un run
pulito, 3 900 cicli in stato `Data` con `anom 0`, e la matrice di fault
injection con la reazione attesa per ogni guasto. Ogni commit del firmware e'
stato preceduto da un `make run` + `make rtt` con quei numeri sotto controllo.
