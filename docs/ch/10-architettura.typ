#import "../lib/style.typ": *
= Architettura del virtual safe PLC

Il *virtual safe PLC* è un 1oo2: due core indipendenti eseguono lo stesso codice di sicurezza e si
scambiano il risultato; una divergenza porta le uscite a zero. Il campo (lo slave #term("fsoe") e gli
ingressi) è simulato da un terzo core, che nella architettura originale sarebbe l'A55 con EtherCAT e che
qui non è un componente di sicurezza.

== Ruoli e mappatura

#si-table(
  columns: (1fr, 1.3fr, 1.5fr, 2.9fr),
  header: ([App], [Core], [`FLOW_CORE_ID`], [Ruolo]),
  rows: (
    ([`fw/m70`], [M7_0 @ 800 MHz], [0 — `SAFE_A`], [master FSoE #1, pubblica la *metà CRC* del frame, lato *B* della MU18]),
    ([`fw/m71`], [M7_1 @ 800 MHz], [1 — `SAFE_B`], [master FSoE #2, pubblica la *metà dati*, lato *A* della MU18]),
    ([`fw/m33s`], [M33_1 @ 266 MHz], [2 — `FIELDBUS`], [ruolo A55: unisce le due metà, esegue lo slave FSoE, genera il campo]),
  ),
)

La scelta non è arbitraria: la @fig-architettura mostra il vincolo hardware. La MU18 è l'*unica* MU che
collega due core secondari fra loro, e i suoi lati sono cablati: A su M7_1, B su M7_0. Il M33_1 non ha
nessuna MU verso gli M7 (le sue vanno verso l'SM e l'AP). Quindi un 1oo2 con cross-check su MU può stare
solo sui due M7, e il campo deve stare sul M33_1, che di MU non ha bisogno perché parla solo attraverso la
memoria condivisa.

#figure(
  caption: [Architettura: due core safe in 1oo2, un core di campo, due finestre OCRAM condivise e il solo link MU18 disponibile.],
  cetz-canvas({
    import cetz-draw: *
    let core(x, y, name, sub, fill: rgb("#eef4fb")) = content(
      (x, y),
      box(stroke: 0.7pt + accent, fill: fill, inset: (x: 7pt, y: 5pt), radius: 3pt, align(center, text(size: 8.5pt)[*#name* #linebreak() #sub])),
    )
    core(-4.2, 0, "M7_0", "SAFE_A · metà CRC")
    core(-0.2, 0, "M7_1", "SAFE_B · metà dati", fill: rgb("#eaf6ee"))
    core(4.0, 0, "M33_1", "FIELDBUS · slave + campo", fill: rgb("#fdf3e7"))

    // MU18 fra i due M7
    line((-2.8, 0.55), (-1.6, 0.55), mark: (end: ">"), stroke: 0.8pt)
    line((-1.6, 0.25), (-2.8, 0.25), mark: (end: ">"), stroke: 0.8pt)
    content((-2.2, 1.0), text(size: 7.5pt, fill: muted)[MU18 (16 B)])

    // OCRAM
    content((0, -1.9), box(stroke: 0.7pt + luma(150), fill: luma(248), inset: 6pt, radius: 3pt, text(size: 8pt)[
      OCRAM condivisa: mailbox `0x204D4000` · ICC `0x204D5000`
    ]))
    line((-4.2, -0.75), (-0.8, -1.6), stroke: 0.6pt + luma(120), mark: (end: ">"))
    line((-0.2, -0.75), (0, -1.6), stroke: 0.6pt + luma(120), mark: (end: ">"))
    line((2.6, -0.75), (0.8, -1.6), stroke: 0.6pt + luma(120), mark: (end: ">"))
    line((4.0, -0.75), (2.6, -1.6), stroke: 0.6pt + luma(120), mark: (end: ">"))
  }),
) <fig-architettura>

== Mappa di memoria

Tutto ciò che è condiviso sta in OCRAM: la DDR non è inizializzata da `flash.bin`, quindi non è
utilizzabile dai core secondari. Le due finestre sono concesse alle tre #term("lm") dalla patch SM 0002
(la prima dalla 0001), e la loro scrittura è la prova che il #term("trdc") è configurato bene.

#si-table(
  columns: (1.4fr, 1fr, 2.4fr),
  header: ([Indirizzo], [Dimensione], [Uso]),
  rows: (
    ([`0x204D0000`], [16 KB], [control block #term("rtt") condiviso, un canale per core (patch SM 0001)]),
    ([`0x204D4000`], [16 KB], [mailbox FSoE: `flow_shm_t`, 328 B usati (patch SM 0002)]),
    ([`0x204D5000`], [1 KB], [canale di cross-check `flow_icc_t`: 8 slot da 88 B + barrier]),
    ([`0x204D5400`…], [~11 KB], [riserva per crescita e guardia]),
  ),
)

Al boot ogni core scrive e rilegge due word nelle due finestre (`flow_selftest`): se il TRDC non concede
l'area, il fault arriva subito e non a metà ciclo.

#nota[
  Nel `flow_shm_t` l'header è pubblicato con il `magic` *per ultimo* (store release) e `flow_shm_validate()`
  richiede anche `state = READY`: chi legge non vede mai una mailbox a metà inizializzazione. Il ruolo
  `FIELDBUS` è l'unico che la inizializza, i due core safe la aspettano con un timeout (2 s) e vanno in
  fail-safe se non arriva.
]
