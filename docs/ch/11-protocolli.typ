#import "../lib/style.typ": *
= I protocolli: mailbox, cross-check, barrier

Il 1oo2 ha bisogno di tre cose che non sono FSoE e che vanno protette a mano: il passaggio dei frame fra
i core e il campo (mailbox), lo scambio dei risultati fra i due core (cross-check) e la sincronizzazione
del ciclo (barrier). Nessuna delle tre è un canale di sicurezza certificato: sono i *mezzi*, mentre la
garanzia end-to-end resta il CRC FSoE.

== La mailbox: frame spezzato

La mailbox contiene, per ogni connessione, due canali TX (uno per core) e uno RX (del fieldbus). Ogni
canale ha tre banchi con un seqlock, così chi legge non prende mai uno slot a metà scrittura, e ogni slot
ha un CRC-16 sui soli metadati:

#si-listing(
  caption: [Slot della mailbox: `len`/`flags`/`fc` sono coperti dal CRC-16, la PDU no (la protegge FSoE)],
  src: "/fw/common/flow/flow_shm_api.h",
  lines: (69, 78),
)

Il punto interessante è la *divisione del frame*: `SAFE_A` scrive solo i byte CRC della PDU, `SAFE_B`
solo i byte dati (comando, dati di processo, Connection ID). Nessuno dei due ha da solo un frame #term("pdu")
valido: il fieldbus prende il frame con `MIN(count_A, count_B)` — l'ultimo che *entrambi* hanno pubblicato —
e interleave le due metà. Se una metà è sbagliata il frame assemblato non è FSoE-valido e lo slave lo
scarta con `INVALID_CRC`: è il controllo end-to-end fatto da un componente che non è nessuno dei due core.

#nota[
  Con tre banchi il banco scelto da `MIN` non è mai quello in scrittura, anche se un core è una o due
  pubblicazioni indietro. L'RX invece è *una per ciclo safe*: il fieldbus pubblica la risposta dello slave
  una sola volta, al confine del ciclo, così i due core leggono la stessa RX (e lo stesso `cycle`).
]

== Il cross-check: messaggio protetto + doorbell MU18 <sec-crosscheck>

I messaggi fra i due core portano decisioni di sicurezza, quindi hanno un protocollo proprio: id e versione,
routing (sorgente, destinazione, canale, tag), *sequence number indipendente da quello FSoE* e CRC-32 su
tutto il messaggio.

#si-listing(
  caption: [Messaggio di cross-check: 72 byte, con CRC-32 sull'intero messaggio],
  src: "/fw/common/flow/flow_mu.h",
  lines: (33, 49),
)

Sopra ci sono quattro tipi di payload: negoziazione del Session ID, cross-check del PDU in uscita, delle
SafeInputs decodificate e delle SafeOutputs calcolate. Il trasporto è l'ICC, e qui c'è il vincolo che ha
guidato tutto: *la #term("mu") ha 4 registri TR e 4 RR da 32 bit*, cioè 16 byte per direzione, mentre il
messaggio è di 72. Quindi:

- il messaggio intero sta in uno slot in OCRAM (`flow_icc_slot_t`, 88 byte con seq/lunghezza/ciclo/CRC);
- la MU18 porta solo il *doorbell*, 16 byte: `TR0 = {seq, tag, channel}`, `TR1 = CRC-32 del payload`;
- chi riceve *ricalcola* il CRC del payload letto in OCRAM e lo confronta con quello arrivato via MU:
  due mezzi fisici indipendenti devono dire la stessa cosa, altrimenti è anomalia.

#si-listing(
  caption: [Slot dell'ICC: l'intero messaggio MU (72 B) più seq, ciclo e CRC-32],
  src: "/fw/common/flow/flow_icc.h",
  lines: (35, 43),
)

== La barrier di ciclo <sec-barrier>

I due core safe devono eseguire lo *stesso* ciclo: i messaggi contengono il numero di ciclo e
`flow_mu_verify()` lo confronta. La barrier è a tre partecipanti e ha una regola sola, che evita ogni
race: *il numero di ciclo lo scrive solo il fieldbus*.

#si-listing(
  caption: [Il gate d'avvio della barrier: il ciclo non avanza finché entrambi i core non hanno chiuso il primo],
  src: "/fw/common/flow/flow_icc.c",
  lines: (178, 196),
)

Il protocollo completo:

#si-table(
  columns: (1.5fr, 2.2fr, 2.6fr),
  header: ([Chi], [Quando], [Cosa fa]),
  rows: (
    ([core safe], [a ogni tick di 10 ms], [attende che `cycle` cambi (`flow_icc_cycle_wait`), esegue i 4 passi, poi scrive `ready[me] = cycle`]),
    ([fieldbus], [ogni 5 bus cycle = 10 ms], [pubblica la RX del ciclo, attende `ready[A]` e `ready[B]`, poi `cycle++`]),
    ([fieldbus], [se un core è fermo], [dopo 30 ms apre comunque il ciclo: il vivo lo rileva dal cross-check e va in fail-safe]),
  ),
)

#nota[
  La barrier è anche il motivo per cui il ciclo dei due core è *deterministico*: senza, uno dei due
  potrebbe eseguire due cicli mentre l'altro ne esegue uno, e ogni cross-check fallirebbe per un motivo che
  non è un guasto. Il gate d'avvio esiste per lo stesso motivo: all'inizio il fieldbus non deve "correre"
  mentre i core sono ancora nel boot.
]
