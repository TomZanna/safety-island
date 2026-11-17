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
| `misra-check` | fallisce su finding nuovi o deviazioni obsolete |
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

I blocchi Mermaid e le tabelle Doxygen nei commenti sono delicati: se
clang-format li rovina, si usano guardie `// clang-format off` / `on` intorno
al blocco (`ReflowComments` non si tocca senza motivo).

## Commenti Doxygen {#commenti}

Le API, l'ABI, i protocolli e gli invarianti stanno nei commenti Doxygen dei
file in `fw/`; l'architettura e il perche' delle scelte restano in `docs/`.
Poche regole, per non scivolare ne' nei telegrammi ne' nella prosa:

1. `@brief` = una frase con il verbo in testa, entro le 80 colonne.
2. Il corpo dice il *perche'*: ordine delle scritture, scrittore unico,
   timeout, costanti. Se una frase ripete il nome della funzione, si toglie.
3. `@param` solo quando aggiunge informazione (unita', range, "solo FIELDBUS",
   "ignorato"), nell'ordine della firma.
4. Un fatto, un posto: l'header ha contratto e invarianti, il `.c` ha
   l'implementazione. La testa del `.c` non ricopia il `@file` dell'header.
5. Liste solo per enumerazioni (bit di stato, campi di un log, mappa di
   memoria); il ragionamento va in prosa.
6. Riferimenti con `@ref`, italiano con traslitterazione ASCII (`e'`, `meta'`),
   nessuna riga ` *` vuota prima di `*/`.
7. Tabelle e diagrammi Mermaid restano nei commenti solo con le guardie
   `// clang-format off` / `on`; se diventano contenuto documentale, il posto
   e' un file in `docs/` (esempio: la tabella dei log RTT, `@ref rtt_log`).

`make docs` gira con `WARN_AS_ERROR=FAIL_ON_WARNINGS`: un `@param` sbagliato o
un `@ref` inesistente ferma il build, ed e' voluto.

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
  --template='{file}:{line}:{column}: {severity}: {id}: {message}' \
  --output-file=build/<preset>/reports/misra.txt
```

Un solo passaggio, **senza** `--suppressions-list`: il report contiene tutti i
finding, anche quelli gia' accettati. Poi `scripts/misra_check.py` confronta il
report con la baseline `quality/misra-suppressions.txt`:

| Situazione | Esito di `misra-check` |
|---|---|
| finding non in baseline | regressione: esce 1, stampa `file:linea` e la riga di codice |
| riga di baseline senza finding | deviazione **obsoleta** (il codice e' cambiato): esce 1 |
| insiemi uguali | verde, con il riepilogo per regola |

Il confronto nei due sensi evita che la baseline "derivi" in silenzio: se il
codice cambia, la riga va rivalutata, non copiata.

- `--platform=unix32` approssima i 32 bit del Cortex-M; se i finding sono
  troppo sensibili a `sizeof`, si aggiunge un platform XML ARM.
- `--project` e' relativo alla radice del repo: report e baseline usano path
  relativi (`fw/...`), leggibili e confrontabili.
- `quality/misra-suppressions.txt` e' la baseline, raggruppata **per regola**:
  ogni blocco dice cosa chiede la regola (con la categoria: Mandatory,
  Required, Advisory), perche' la deviazione e' accettata e come e' limitata.
  Il formato delle righe resta `<id>:<file>:<line>` (lo impone cppcheck);
  vietato sopprimere interi file o categorie.
- La baseline accetta solo cio' che non si puo' correggere: codice vendored
  (`fw/common/fsoe/`), vincoli architetturali (es. 15.5, single exit contro
  fail-safe) e limiti del checker (es. 17.3 con i builtin `__atomic_*`).
- `misra` e' report-only; `misra-check` e' il gate. In fase di
  certificazione serviranno un checker qualificato e la tool qualification.

## Cosa non e' coperto

- MISRA non e' certificato; `clang-tidy` non ha check MISRA (`cert-*` e'
  CERT C, non MISRA).
- Non si analizzano i **sorgenti** di `fw/common/fsoe/`, l'SDK, `sm/`, `ele/`,
  script e CMake. Gli **header** della libreria FSoE vengono pero' inclusi dai
  nostri TU: i finding che nascono li' (union, macro non usate) sono riportati
  e accettati riga per riga in baseline, perche' la libreria non si modifica
  (AGENTS.md).
- L'addon implementa anche regole introdotte dagli amendment (es. 21.16), che
  non compaiono nell'edizione 2012 base: per quelle il riferimento va cercato
  nella documentazione MISRA aggiornata, non nel PDF base.
- Il lint e' statico sul codice C; non sostituisce i test in board, il
  fault injection e le misure di WCET/stack.
- La formattazione dipende dalla versione di clang-format: il warning di
  versione esiste proprio per rendere visibile la differenza.

Due limiti pratici, voluti:

- `clang-tidy` gira con `-Wno-everything` per non inondare il report con i
  warning di SDK/newlib; i warning del compilatore restano quelli di
  `-Wall -Wextra` nel build firmware.
- cppcheck non conosce i builtin atomics (`__ATOMIC_*`) usati da
  `flow_icc.c`/`flow_shm.c`: `misra-config` e' soppresso e il report MISRA
  puo' avere falsi negativi su quelle espressioni. Gli stessi builtin fanno
  scattare 17.3 (falsa dichiarazione implicita): le righe sono in baseline con
  il motivo, il build reale con `-Wall -Wextra` e' pulito.
