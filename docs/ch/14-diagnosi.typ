#import "../lib/style.typ": *
= Diagnosi e fault injection

== I log

Ogni secondo ogni ruolo stampa una riga sul proprio canale #term("rtt"). Le righe dei due core safe sono
volutamente identiche: se un contatore differisce, si vede subito.

```text
[m70/A] cyc 3900 Data anom 0 icc p11700 r11700 to0 crc0 sq0 cy0 db0 mu to0/0 sess 0xb7d5 hb 3900/3900/3900
[m71/B] cyc 3900 Data anom 0 icc p11700 r11700 to0 crc0 sq0 cy0 db0 mu to0/0 sess 0xb7d5 hb 3900/3900/3900
[m33s/fld] bus 19000 Data out 1 en 1 ok 18705 drop 0 a55 0 hb 3900/3900/3801
```

#si-table(
  columns: (1.2fr, 3.2fr),
  header: ([Campo], [Significato]),
  rows: (
    ([`cyc` / `bus`], [cicli safe eseguiti / cicli da 2 ms del fieldbus]),
    ([`Reset…Data`], [stato FSoE corrente]),
    ([`anom`], [anomalie di cross-check cumulate (deve restare 0 in run pulito)]),
    ([`icc p/r`], [messaggi pubblicati/ricevuti sul canale inter-core]),
    ([`to/crc/sq/cy/db`], [timeout, CRC errati, seq incoerente, ciclo diverso, doorbell inatteso]),
    ([`mu to`], [timeout del doorbell MU18 (tx/rx)]),
    ([`sess`], [Session ID negoziato (deve essere uguale sui due core)]),
    ([`hb`], [heartbeat dei tre ruoli, letti dalla mailbox]),
    ([`out/en/ok/drop/a55`], [fieldbus: uscite valide, abilitate, frame buoni, frame scartati, frame invalidi assemblati]),
  ),
)

== I comandi

#si-table(
  columns: (1.1fr, 1.6fr, 3.2fr),
  header: ([Canale], [Tasto], [Effetto]),
  rows: (
    ([m70/m71], [`p`], [corrompe il PDU in uscita di questo core → i due PDU divergono]),
    ([m70/m71], [`i`], [corrompe le SafeInputs decodificate → divergenza sugli ingressi]),
    ([m70/m71], [`l`], [corrompe le SafeOutputs calcolate → divergenza sulla logica]),
    ([m70/m71], [`x`], [corrompe il random del Session ID alla prossima negoziazione]),
    ([m70/m71], [`?`], [stampa lo stato completo]),
    ([m33s], [`s`], [attiva/disattiva una discrepanza fra i due canali di campo]),
    ([m33s], [`d`], [scarta 60 frame (~120 ms) → fa scadere il watchdog FSoE]),
    ([m33s], [`?`], [stampa lo stato del fieldbus]),
  ),
)

== La matrice di fault injection <sec-fault>

Comportamento osservato in board (@sec-misure). In tutti i casi la macchina va in stato sicuro e si
riconnette da sola.

#si-table(
  columns: (0.7fr, 2.4fr, 2.4fr),
  header: ([Fault], [Cosa si vede nei log], [Reazione]),
  rows: (
    ([`p`], [`ANOMALIA i due core hanno generato PDU diversi` su *entrambi*], [uscite 0, reset, riconnessione a `Data`]),
    ([`i`], [`ANOMALIA SafeInputs divergenti`], [uscite 0, sending disabilitato; poi watchdog dello slave e reset]),
    ([`l`], [`ANOMALIA SafeOutputs divergenti`], [uscite 0 + `reset richiesto`, riconnessione]),
    ([`x`], [session diversi (`0x1cc0` vs `0x1dc0`) → PDU diversi], [reset; alla riconnessione i due random tornano coerenti]),
    ([`d`], [`reset da slave: WD_EXPIRED`, stato `Data → Session`], [riconnessione completa entro pochi cicli]),
  ),
)

== Due cose che sembrano guasti e non lo sono

=== L'attach del debugger <sec-attach>

Aprendo `make rtt` (o un attach #term("swd")) il J-Link *ferma il core* per qualche millisecondo: il core
più veloce scade il timeout della barrier, l'altro il watchdog FSoE, e si vede un `barrier timeout` seguito
da `WD_EXPIRED` e da una riconnessione. È il comportamento corretto — un core che si ferma *deve* portare il
sistema in stato sicuro — e sparisce se non ci si attacca. Nei run puliti `anom` resta 0.

=== Il primo `INVALID_CRC`

Nei primi cicli dopo il boot lo slave rifiuta i frame del master con `INVALID_CRC`: il master sta ancora
chiedendo la connessione con frame che lo slave non ha ancora accettato. Sono due righe per master, una volta
sola, e non hanno niente a che vedere con il cross-check (che resta a zero).
