# Diagnosi e fault injection {#diagnosi}

## I log {#log}

Ogni secondo ogni ruolo stampa una riga sul proprio canale RTT. Le righe dei due
core safe sono volutamente identiche: se un contatore differisce, si vede
subito.

```text
[m70/A] cyc 3900 Data anom 0 icc p11700 r11700 to0 crc0 sq0 cy0 db0 mu to0/0 sess 0xb7d5 hb 3900/3900/3900
[m71/B] cyc 3900 Data anom 0 icc p11700 r11700 to0 crc0 sq0 cy0 db0 mu to0/0 sess 0xb7d5 hb 3900/3900/3900
[m33s/fld] bus 19000 Data out 1 en 1 ok 18705 drop 0 a55 0 hb 3900/3900/3801
```

Il formato e il significato dei campi sono documentati in @ref rtt_log, accanto
alla telemetria (`fw/safe/safe_main.c` e `fw/fieldbus/fieldbus_main.c`).

## I comandi {#comandi}

La tabella d'uso dei tasti e' nel `README.md`; l'effetto di ogni handler sta nel
commento vicino al parser (`safe_main.c` e `fieldbus_main.c`). Qui non si
ricopia: il README e' l'unico posto per l'uso, il codice per l'effetto.

## Due cose che sembrano guasti e non lo sono {#falsi-guasti}

### L'attach del debugger {#attach}

Aprendo `make rtt` (o un attach SWD) il J-Link *ferma il core* per qualche
millisecondo: il core piu' veloce scade il timeout della barrier, l'altro il
watchdog FSoE, e si vede un `barrier timeout` seguito da `WD_EXPIRED` e da una
riconnessione. E' il comportamento corretto -- un core che si ferma *deve*
portare il sistema in stato sicuro -- e sparisce se non ci si attacca. Nei run
puliti `anom` resta 0.

### Il primo `INVALID_CRC` {#invalid-crc}

Nei primi cicli dopo il boot lo slave rifiuta i frame del master con
`INVALID_CRC`: il master sta ancora chiedendo la connessione con frame che lo
slave non ha ancora accettato. Sono due righe per master, una volta sola, e non
hanno niente a che vedere con il cross-check (che resta a zero).

La matrice completa dei guasti e le reazioni osservate sono in @ref misure.
