# La repo e i suoi componenti {#repo}

La repo e' un *superbuild*: non compila niente direttamente, orchestra tool
esterni (toolchain, `west`, `mkimage`, `uuu`, `J-Link`, Doxygen) e i
sottoprogetti del firmware. Tutto cio' che e' scaricato finisce in `ext/` o
`build/`, entrambi gitignored: nel repo restano solo sorgenti, script,
configurazioni e i binari dell'SM (che sono la "sorgente di verita'" del boot).

## Albero {#albero}

```text
safety-island/
  CMakeLists.txt        superbuild: deps -> ele -> sm -> fw -> image -> board -> docs
  CMakePresets.json     preset mx94evk | mx94alt | docker
  Makefile              scorciatoia: make <target> = cmake --build --preset
  deps/                 west.yml: 4 repo SDK NXP pinnati per SHA (in ext/sdk)
  ele/                  firmware ELE NXP: URL + sha256, scaricato (EULA)
  sm/                   System Manager: binari committati + patches/ + Dockerfile
  fw/                   firmware: app (m70, m71, m33s) + ruoli (safe/, fieldbus/) + common
  image/                mkimage -> flash.bin (AHAB + SM + app)
  board/                target make: run, reset, uart, rtt, debug, usb-*
  scripts/              script bash/python usati dai target
  docs/                 pagine Markdown + Doxyfile + tema (questo sito)
  ext/                  toolchain, SDK, strumenti, venv (gitignored)
  build/<preset>/       uscite (gitignored): flash.bin, elf/bin delle app, HTML
```

## Componenti, uno per uno {#componenti}

| Componente | Cosa fa | Committato? | Come si produce |
|---|---|---|---|
| `deps/` | pinna toolchain Arm e SDK MCUXpresso minimo | solo `west.yml` | `make deps` |
| `ele/` | fornisce il firmware ELE (`mx943a0-ahab-container.img`) | no (EULA NXP) | `make ele` |
| `sm/` | binari del System Manager + patch locali + Dockerfile | si (`.bin`, `SHA256SUMS`) | `make sm-update` / `sm-verify` |
| `fw/` | le tre app, i due ruoli (`safe/`, `fieldbus/`) e il codice comune | si | `make fw` / `make image` |
| `image/` | impacchetta AHAB + SM + app in `flash.bin` | si (solo CMake) | `make image` |
| `board/` | target che parlano con la board | si | (nessuna uscita) |
| `scripts/` | implementazione dei target: USB, uuu, RTT, gdb, SM in Docker | si | - |
| `docs/` | questo sito | si | `make docs` |

> La regola d'oro: *l'SDK non si copia e non si modifica* dentro `ext/`. Se
> serve un comportamento diverso, si interviene da `fw/common` (per esempio
> `SystemInitHook`, linker script) o si aggiorna il manifest `deps/west.yml`
> cambiando gli SHA.

## Gli script che contano {#script}

Ogni script spiega nella propria intestazione i dettagli operativi. Qui solo la
mappa:

| Script | Ruolo |
|---|---|
| `scripts/common.sh` | funzioni condivise; default delle variabili, `die`, `info` |
| `scripts/deps.sh` | crea il venv (`west` + `pylink`) e fa `west update` |
| `scripts/fetch-ele.sh` | scarica il pacchetto ELE, verifica lo sha256, estrae il container |
| `scripts/sm-docker.sh` | ricostruisce l'SM nel container e confronta/copia i binari |
| `scripts/flash.sh` | reset + `uuu SDPS: boot`, con fallback BCU |
| `scripts/reset.sh` | J-Link (`r0`/`r1`) -> BCU -> monitor SM, in quest'ordine |
| `scripts/uart.sh` | console seriale del monitor SM (`picocom`) |
| `scripts/rtt.py` | lettura/scrittura RTT via `pylink`, una connessione per terminale |
| `scripts/debug.sh` | GDB server + gdb su un core, `GDB_BATCH=1` per l'attach non interattivo |
| `scripts/usb-*.sh` | USB/IP in WSL: setup, attach, stato |

I comandi e le variabili d'uso sono nel `README.md`; il perche' dei target e'
in @ref build, @ref board e @ref processo.
