# Formattazione, lint e MISRA {#qualita}

Gli strumenti di qualita' sono **opzionali**: `make image`, `make run` e
`make docs` non li richiedono. Servono solo ai target `format`, `lint`,
`misra` e `check`.

## Prerequisiti

| Tool | Versione minima | A cosa serve | Installazione Ubuntu |
|---|---|---|---|
| `clang-format` | 19.1.7 | formattazione C/H | `sudo apt install clang-format-19` |
| `clang-tidy` | 19.1.7 | lint statico e `clang-analyzer` | `sudo apt install clang-tidy-19` |
| `cppcheck` | 2.19.0 | MISRA C:2012 (non certificato) | `sudo apt install cppcheck` |

Il build usa il binario della distro: **non c'e' uno sha256**, perche' le
build cambiano tra distribuzioni. `quality/CMakeLists.txt` e' l'unico posto
dove sono dichiarate le versioni minime:

- se un tool manca, il target che lo usa esce subito con il comando di
  installazione (`make check` fa il precheck di tutti e tre);
- se la versione e' sotto la minima, stampa un warning e prosegue;
- se e' piu' recente, prosegue e stampa la versione usata.

## Target

| Target | Cosa fa |
|---|---|
| `format` | applica `clang-format -i` ai `.c/.h` di `fw/` (esclusa `fw/common/fsoe/`) |
| `format-check` | `clang-format --dry-run --Werror`: fallisce se serve formattare |
| `lint` | `clang-tidy` sul `compile_commands.json` delle app |
| `lint-fix` | applica i fix di clang-tidy; da rivedere a mano |
| `misra` | genera `build/<preset>/reports/misra.txt` (report, non fallisce) |
| `misra-check` | fallisce sulle violazioni MISRA non soppresse |
| `check` | precheck tool + `format-check` + `lint` + report MISRA |
| `check-misra` | come `check`, ma anche `misra-check` |

Il `compile_commands.json` unico e' prodotto da `make fw`: `lint`, `misra`,
`check` e `check-misra` lo rigenerano passando da `fw`. `format` non ne ha
bisogno.

## Formattazione

La fonte di verita' dello stile e' `.clang-format` nella radice. Scope:
`fw/**/*.c`, `fw/**/*.h`, esclusi `fw/common/fsoe/`, `ext/` e `build/`.

Il primo `make format` e' volutamente un commit meccanico: clang-format
normalizza indentazione, puntatori, include e commenti. Prima di committare:

```bash
make format
make fw            # i commenti non cambiano il codice, ma gli include si'
make image
make docs          # deve restare a 0 warning
```

I blocchi `\mermaid` e le tabelle Doxygen nei commenti sono delicati: se
clang-format li rovina, si usano guardie `// clang-format off` / `on` intorno
al blocco (`ReflowComments` non si tocca senza motivo).

## clang-tidy

Il profilo in `.clang-tidy` e' volutamente **C**, non `Checks: "*"`: include
`bugprone-*`, `cert-*`, `clang-analyzer-*` e `misc-*`, ed esclude le famiglie
C++ e il naming. Il filtro header e' limitato a `fw/`; la libreria FSoE
vendored e il SDK non producono finding.

`scripts/lint.py`:

- legge il `compile_commands.json`;
- tiene una entry per sorgente (le tre app compilano gli stessi file con
  define diverse);
- passa a clang-tidy `--target=arm-none-eabi`, il toolchain GCC in uso e
  `-Wno-unused-command-line-argument`;
- scrive il report in `build/<preset>/reports/clang-tidy.txt`;
- con `--no-werror` (solo per triage) non trasforma i warning in errore.

Le eccezioni sono `NOLINT`/`NOLINTNEXTLINE` con il nome del check e un motivo.
Non si sopprimono categorie o file interi.

## MISRA con cppcheck

`cppcheck --addon=misra` implementa una parte di MISRA C:2012 (con amendment
1 e 2). **Non e' un checker certificato** e non distribuisce i testi delle
regole: stampa solo l'ID (`misra-c2012-x.y`). Serve per il triage e per
intercettare regressioni, non per dichiarare conformita'.

Comando (dentro `scripts/misra.sh`):

```bash
cppcheck --project=build/<preset>/compile_commands.json \
  --file-filter='fw/*' -i fw/common/fsoe -i ext -i build \
  --platform=unix32 --std=c11 --enable=style --addon=misra \
  --suppress=missingIncludeSystem --inline-suppr \
  --suppressions-list=quality/misra-suppressions.txt \
  --error-exitcode=1 --output-file=build/<preset>/reports/misra.txt
```

- `--platform=unix32` approssima i 32 bit del Cortex-M; se i finding sono
  troppo sensibili a `sizeof`, si aggiunge un platform XML ARM.
- `quality/misra-suppressions.txt` e' la baseline: ogni riga ha un motivo e
  un riferimento alla review. Vietato sopprimere interi file o categorie.
- `misra` e' report-only; `misra-check` e' il gate. In fase di
  certificazione serviranno un checker qualificato e la tool qualification.

## Cosa non e' coperto

- MISRA non e' certificato; `clang-tidy` non ha check MISRA (`cert-*` e'
  CERT C, non MISRA).
- Non si analizzano `fw/common/fsoe/`, SDK, `sm/`, `ele/`, script e CMake.
- Il lint e' statico sul codice C; non sostituisce i test in board, il
  fault injection e le misure di WCET/stack.
- La formattazione dipende dalla versione di clang-format: il warning di
  versione esiste proprio per rendere visibile la differenza.
