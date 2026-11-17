# Registro delle misure {#misure}

Run fatte su i.MX94 EVK A0 durante lo sviluppo, con l'immagine costruita da
questa repo (SM `lf-6.18.20-2.0.0`, ELE 2.0.6, preset `mx94evk`).

## Validazione del virtual safe PLC {#misure-validazione}

| Passo | Cosa e' stato misurato | Risultato |
|---|---|---|
| Piattaforma | tick 10 ms, tempo DWT, selftest | `slip 0` su oltre 10 000 cicli; selftest `PASS` sui 3 core |
| Memoria | probe OCRAM condivisa + mailbox | `ocram condivisa PASS`; heartbeat incrociati `100/100/100` |
| MU18 | round-trip del doorbell | `mu18 link (ping) PASS`, peer `0x5a5a0000` |
| ICC | scambio full-duplex + barrier | `icc exchange PASS`; 10 600 cicli, `bar 1` solo all'avvio |
| FSoE | due master + slave | `RESET->DATA`, session `0xdd22`/`0xb7d5` identici, `anom 0` su 3 900 cicli |
| Uscite | logica safe + campo | `en 1` (0x00C3) con condizioni soddisfatte, `0x0000` altrimenti |
| Fault injection | `p` `i` `l` `x` `d` | vedi @ref misure-fault fail-safe + riconnessione ogni volta |
| Footprint | `--print-memory-usage` | text 42/42/37 KB, data 10.7 KB, stack 4 KB |

## Matrice di fault injection {#misure-fault}

Comportamento osservato in board. In tutti i casi la macchina va in stato sicuro
e si riconnette da sola.

| Fault | Cosa si vede nei log | Reazione |
|---|---|---|
| `p` | `ANOMALIA i due core hanno generato PDU diversi` su *entrambi* | uscite 0, reset, riconnessione a `Data` |
| `i` | `ANOMALIA SafeInputs divergenti` | uscite 0, sending disabilitato; poi watchdog dello slave e reset |
| `l` | `ANOMALIA SafeOutputs divergenti` | uscite 0 + `reset richiesto`, riconnessione |
| `x` | session diversi (`0x1cc0` vs `0x1dc0`) -> PDU diversi | reset; alla riconnessione i due random tornano coerenti |
| `d` | `reset da slave: WD_EXPIRED`, stato `Data -> Session` | riconnessione completa entro pochi cicli |

## Tempi e footprint {#misure-tempi}

| Cosa | Periodo | Note |
|---|---|---|
| ciclo safe | 10 ms | tick da SysTick a 1 kHz; il tempo per il watchdog viene da DWT, per non avere una causa comune |
| bus/fieldbus | 2 ms | tick separato: lo slave risponde spesso, il watchdog FSoE (100 ms) non scatta mai |
| barrier | 10 ms | aperta dal fieldbus quando entrambi i core hanno finito; timeout 30 ms |
| cross-check | - | timeout 5 ms per singolo scambio, misurato con DWT |

Nei run puliti il jitter misurato e' zero: `slip 0` su oltre 10 000 cicli safe
(il contatore conta solo i ritardi oltre due periodi). Il margine del watchdog
e' 10x: un ciclo perso non e' un problema, dieci si'.

Footprint delle app (numeri di `--print-memory-usage` sui TCM):

| App | text | data+bss | stack |
|---|---|---|---|
| `m70` | 42 KB (17%) | 10.7 KB (6%) | 4 KB |
| `m71` | 42 KB (17%) | 10.7 KB (6%) | 4 KB |
| `m33s` | 37 KB (15%) | 10.6 KB (6%) | 4 KB |

C'e' spazio per crescere (piu' connessioni FSoE, piu' diagnostica) senza toccare
il linker script. Nessuna allocazione dinamica: le strutture (`flow_safe_t`,
`flow_field_t`, gli stack FSoE) sono statiche, e l'unica cosa "grande" sullo
stack e' il buffer di 200 byte del log RTT.
