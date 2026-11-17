# Documentazione tecnica

Questa e' la documentazione tecnica della repo `safety-island` (i.MX94 EVK,
silicio A0) e del *virtual safe PLC* che ci gira sopra: due core safe in
configurazione 1oo2 che eseguono lo stack FSoE di rt-labs, un terzo core che fa
da campo simulato (il ruolo che sul prodotto finale sara' dell'A55 con
EtherCAT) e un System Manager NXP che possiede boot, clock, pin e memoria.

L'obiettivo non e' (ancora) un prodotto certificato: e' un banco di prova
*reale*, su silicio, in cui l'architettura del diagramma di sequenza e'
verificabile a ogni modifica con un ciclo di pochi secondi: compila, carica,
guarda i log, inietta un guasto, guarda la reazione.

La documentazione vive insieme al codice: le API, l'ABI, i protocolli e gli
invarianti stanno nei commenti Doxygen dei file in `fw/`; qui restano
l'architettura, il setup e le misure. I comandi d'uso stanno nel
`README.md` della repo.

## Stato attuale {#stato}

| Componente | Stato | Come e' stato verificato |
|---|---|---|
| Immagine di boot (ELE + SM + 3 app) | ok | uuu in SDP, `make run` su EVK |
| SM con OCRAM condivisa (patch 0001/0002) | ok | build Docker riproducibile + selftest su tre core |
| Piattaforma: tick, tempo DWT, log RTT | ok | `slip 0` su oltre 10 000 cicli |
| Mailbox a frame spezzato | ok | heartbeat incrociati e merge `MIN` in board |
| Cross-check 1oo2 via MU18 + OCRAM | ok | exchange senza errori, fault injection |
| Barrier di ciclo a 3 | ok | 10 600 cicli senza timeout in run pulito |
| Ciclo FSoE completo (2 master + slave) | ok | `RESET -> ... -> DATA`, session id identico |
| Logica safe + campo simulato | ok | uscite abilitate/disabilitate, discrepanza |
| Fault injection da RTT | ok | `p`, `i`, `l`, `x`, `d` con reazione attesa |
| Driver Linux lato A55 | no | fuori scope: qui il campo e' m33s |

## Mappa della documentazione {#mappa}

| Pagina | Contenuto |
|---|---|
| @ref repo | albero della repo, componenti e script |
| @ref build | superbuild, tool pinnati, immagine `flash.bin` |
| @ref board | SDP, SWD, seriale, RTT, reset e debug |
| @ref processo | ciclo di sviluppo, regole e riproducibilita' |
| @ref architettura | 1oo2, ruoli, vincolo MU18, mappa di memoria |
| @ref diagnosi | log, fault injection e falsi allarmi |
| @ref limiti | limiti noti e lavoro futuro |
| @ref misure | registro delle misure e footprint |
| @ref glossario | acronimi |
| @ref qualita | clang-format, clang-tidy e MISRA (non certificato) |

I dettagli di API e protocolli sono nei gruppi Doxygen:

- @ref flow_config, @ref flow_shm, @ref flow_mu, @ref flow_icc, @ref flow_mu18
- @ref flow_safe, @ref flow_field, @ref flow_platform, @ref flow_selftest
- @ref board, @ref rtt

## Come si genera {#genera}

```bash
make docs          # build/<preset>/docs/html/index.html
make docs-serve    # build + server locale su http://127.0.0.1:8000/
```

Doxygen e' pinnato a **1.18.0** e viene scaricato in `ext/tools/` al primo
`make docs` (sha256 verificato); il tema e' `doxygen-awesome-css` 2.5.0,
committato in `docs/theme/` e disattivabile con `-DDOCS_AWESOME_CSS=OFF`. I diagrammi usano
Mermaid e le formule MathJax, entrambi caricati da CDN nel browser: non servono
ne' Graphviz ne' LaTeX.
