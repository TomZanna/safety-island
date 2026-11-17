#import "../lib/style.typ": *
= Sommario

Questo documento descrive la repo `safety-island` (i.MX94 EVK, silicio A0) e il *virtual safe PLC* che ci
gira sopra: due core safe in configurazione 1oo2 che eseguono lo stack #term("fsoe") di rt-labs, un terzo
core che fa da campo simulato (il ruolo che sul prodotto finale sarà dell'A55 con EtherCAT) e un System
Manager NXP che possiede boot, clock, pin e memoria.

L'obiettivo non è (ancora) un prodotto certificato: è un banco di prova *reale*, su silicio, in cui
l'architettura del diagramma di sequenza (`sequence diag.md` di `imx-flow`) è verificabile a ogni modifica
con un ciclo di pochi secondi: compila, carica, guarda i log, inietta un guasto, guarda la reazione.

== Stato attuale

#si-table(
  columns: (2.6fr, 1.2fr, 3.4fr),
  header: ([Componente], [Stato], [Come è stato verificato]),
  rows: (
    ([Immagine di boot (ELE + SM + 3 app)], [ok], [uuu in SDP, `make run` su EVK]),
    ([SM con OCRAM condivisa (patch 0001/0002)], [ok], [build Docker riproducibile + selftest su tre core]),
    ([Piattaforma: tick, tempo DWT, log RTT], [ok], [`slip 0` su >10 000 cicli]),
    ([Mailbox a frame spezzato], [ok], [heartbeat incrociati e merge `MIN` in board]),
    ([Cross-check 1oo2 via MU18 + OCRAM], [ok], [exchange senza errori, fault injection]),
    ([Barrier di ciclo a 3], [ok], [10 600 cicli senza timeout in run pulito]),
    ([Ciclo FSoE completo (2 master + slave)], [ok], [`RESET → … → DATA`, session id identico]),
    ([Logica safe + campo simulato], [ok], [uscite abilitate/disabilitate, discrepanza]),
    ([Fault injection da RTT], [ok], [`p`, `i`, `l`, `x`, `d` con reazione attesa]),
    ([Driver Linux lato A55], [no], [fuori scope: qui il campo è m33s]),
  ),
)

== Come leggere questo documento

La *prima parte* descrive la repo e il processo di sviluppo: quali componenti ci sono, cosa è committato e
cosa si scarica, come si costruisce l'immagine, come si parla con la board (SDP, SWD, seriale, RTT) e
quali regole rendono il tutto riproducibile. Serve a chi deve mettere mano all'ambiente.

La *seconda parte* descrive il virtual safe PLC: ruoli e mappatura sui core, mappa di memoria, i due
protocolli (mailbox e cross-check), il rendezvous di ciclo, lo stack FSoE, la logica safe, i tempi e la
diagnosi. Serve a chi deve mettere mano al codice.

Le *appendici* hanno i dettagli che non vuoi leggere due volte: layout delle strutture, registri MU18,
mappa dei file, comandi rapidi e il registro delle misure fatte in board.
