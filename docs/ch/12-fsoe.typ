#import "../lib/style.typ": *
= Lo stack FSoE e la logica safe

== Gli stati

Lo stack di rt-labs è compilato come libreria statica (`libfsoe-<app>.a`) con un port minimo
(`fw/common/flow/port/fsoeport.c`): tempo da `DWT->CYCCNT`, `memcpy`/`memset` da newlib-nano, e
`FSOEPORT_ASSERT` mappato su `flow_platform_fatal` (un'asserzione violata non è un `printf`: è lo stato
sicuro).

La progressione è quella standard, e nei log si vede identica sui due master:

#si-table(
  columns: (1.3fr, 3.2fr),
  header: ([Stato], [Cosa succede]),
  rows: (
    ([`Reset`], [il master chiede la connessione con un frame di reset; lo slave risponde]),
    ([`Session`], [si negozia il Session ID (vedi sotto)]),
    ([`Connection`], [si stabilisce la connessione (Connection ID)]),
    ([`Parameter`], [si scambiano watchdog e parametri applicativi; lo slave dichiara di poter mandare dati]),
    ([`Data`], [dati di processo in entrambe le direzioni; è lo stato in cui la macchina può lavorare]),
  ),
)

Il *Session ID* è l'unico valore che i due core devono *coordinare* invece di calcolare: ciascuno genera un
numero casuale con il proprio RNG, lo scambia, e il Session ID finale è la *somma dei due*. Se i due core
ottengono lo stesso valore, anche i frame che generano sono identici — ed è quello che rende possibile il
cross-check del PDU in uscita. Dal log reale:

```text
[A] SESSION ID: 0xe818 + 0x12bf = 0xfad7
[B] SESSION ID: 0x12bf + 0xe818 = 0xfad7
```

Il *watchdog* FSoE è a 100 ms, dieci volte il ciclo safe: se lo slave smette di rispondere (o i frame
vengono scartati) il master va in `WD_EXPIRED` e riparte da `Reset` — comportamento verificato scartando
60 frame di proposito.

== La logica safe

La funzione di sicurezza è *pura* (`flow_safe_logic.c`): stessi ingressi, stesso stato, stesse uscite, su
entrambi i core. Non ha tempo, I/O né casualità: è la condizione perché i due risultati siano confrontabili.

#si-table(
  columns: (1.6fr, 2.4fr, 2.2fr),
  header: ([Ingresso], [Significato], [Effetto]),
  rows: (
    ([`ch_a`/`ch_b` bit0], [E-stop rilasciato], [necessario per abilitare]),
    ([`ch_a`/`ch_b` bit1], [porta chiusa], [necessario per abilitare]),
    ([`ch_a` bit2], [pulsante di reset], [fronte di salita: abilita e azzera il latch]),
  ),
)

- valutazione *1oo2* sui due canali con monitoraggio di discrepanza: tre cicli consecutivi diversi
  producono un fault *latchato*, che si azzera solo con un reset a condizioni di sicurezza soddisfatte;
- qualsiasi perdita delle condizioni toglie *subito* l'uscita;
- uscita `enable = 0x00C3` quando abilitata, `0x0000` in fail-safe; la word `status` porta i bit di
  diagnostica (abilitato, fault latched, discrepanza, E-stop aperto, porta aperta, canale A/B ok).

== Cosa fa il singolo core a ogni ciclo

`flow_safe_step()` esegue i quattro passi del diagramma, con il numero di ciclo preso dalla barrier:

1. *latch* dei frame RX dalla mailbox (la copia in RAM locale evita di leggere la memoria condivisa due
   volte con contenuti diversi);
2. *sync* con lo stack: `fsoemaster_sync_with_slave()`; dentro `fsoeapp_send()` il core fa il
   *cross-check del PDU in uscita* e pubblica la propria metà del frame;
3. *cross-check delle SafeInputs* decodificate (più stato e frame type della connessione);
4. *logica safe* e *cross-check delle SafeOutputs*.

Se uno qualunque dei cross-check non torna: uscite a zero, `send_enable` togliato, anomalia contata e (dove
previsto) richiesta di reset. La differenza fra i casi è voluta: una divergenza di ingressi porta al
fail-safe *senza* reset, una divergenza di uscite o di PDU chiede anche il reset.
