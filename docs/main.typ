// Documento tecnico di safety-island.
//   make docs        -> build/<preset>/docs/safety-island.pdf
//   make docs-live   -> typst watch
// Cover e front matter qui; contenuti nei capitoli ch/ e nelle appendici ap/.
#import "lib/style.typ": *
#show: document-setup
#show: glossary-setup

// ---------------------------------------------------------------- glossario --
#let entries = (
  (key: "ahab", short: "AHAB", long: "Advanced High Assurance Boot", description: [Contenitore firmato dell'immagine di boot NXP.]),
  (key: "bcu", short: "BCU", long: "Board Control Utility", description: [Utility NXP sull'FTDI della EVK: reset e boot mode forzato.]),
  (key: "ele", short: "ELE", long: "EdgeLock Enclave", description: [Sottosistema di sicurezza NXP che esegue il firmware di boot firmato.]),
  (key: "fsoe", short: "FSoE", long: "Functional Safety over EtherCAT", description: [Protocollo di sicurezza end-to-end (ETG.5100) che viaggia su un black channel.]),
  (key: "icc", short: "ICC", long: "Inter-Core Channel", description: [Canale di cross-check fra i due core safe: payload in OCRAM + doorbell MU18.]),
  (key: "lm", short: "LM", long: "Logical Machine", description: [Partizione logica dell'SM: una per agente (AP, M33S, M7_0, M7_1).]),
  (key: "mu", short: "MU", long: "Messaging Unit", description: [Periferica di scambio messaggi fra due lati cablati (A/B).]),
  (key: "ocram", short: "OCRAM", long: "On-Chip RAM", description: [RAM interna visibile a più core: qui ci stanno RTT e mailbox.]),
  (key: "pdu", short: "PDU", long: "Protocol Data Unit", description: [Il frame FSoE: comando, dati di processo, parole CRC, Connection ID.]),
  (key: "rtt", short: "RTT", long: "Real-Time Transfer", description: [Log/input SEGGER via debug probe, senza periferiche.]),
  (key: "scl", short: "SCL", long: "Safety Communication Layer", description: [Livello di sicurezza dello stack FSoE: stati, CRC, watchdog.]),
  (key: "scmi", short: "SCMI", long: "System Control and Management Interface", description: [Protocollo con cui l'SM concede clock, pin e power ai core.]),
  (key: "sdp", short: "SDP", long: "Serial Download Protocol", description: [Modalità di boot della ROM che accetta un'immagine via USB (uuu).]),
  (key: "swd", short: "SWD", long: "Serial Wire Debug", description: [Interfaccia di debug a due fili: è quella usata dal J-Link.]),
  (key: "sm", short: "SM", long: "System Manager", description: [Firmware NXP sul M33_0 che possiede boot, clock, pin e memoria.]),
  (key: "tcm", short: "TCM", long: "Tightly Coupled Memory", description: [RAM locale di ogni core secondario, dove girano codice e dati.]),
  (key: "trdc", short: "TRDC", long: "Trusted Resource Domain Controller", description: [Filtro hardware che decide quale LM può accedere a cosa.]),
)
#glossary-register(entries)

// ------------------------------------------------------------------ cover ----
#page(numbering: none, header: none)[
  #v(4em)
  #align(center)[
    #text(size: 30pt, weight: "light", tracking: 0.18em)[SAFETY ISLAND]
    #v(0.3em)
    #line(length: 2.5cm, stroke: 0.5pt + rule-col)
    #v(0.4em)
    #text(size: 12pt, fill: muted, tracking: 0.1em)[i.MX94 · repo, processo di sviluppo e virtual safe PLC FSoE]
  ]
  #v(1fr)
  #align(center)[
    #text(size: 9pt, fill: muted)[
      companion di `README.md` e `AGENTS.md`: qui il *perché*, là il *come si usa*
    ]
  ]
  #v(2em)
]

// ------------------------------------------------------------ front matter ---
#outline(title: [Indice], indent: auto, depth: 3)

#pagebreak()
#outline(title: [Lista dei listati], target: figure.where(kind: raw))
#v(1em)
#nota[
  I listati sono presi *dai sorgenti* (`codly.new(path(...))`): se cambia il codice citato, il PDF si
  rigenera al successivo `make docs` (quei file sono nei `DEPENDS` di `docs/CMakeLists.txt`).
]

// ---------------------------------------------------------------- capitoli ---
#include "ch/00-sommario.typ"
#include "ch/01-repo.typ"
#include "ch/02-build-immagine.typ"
#include "ch/03-board.typ"
#include "ch/04-processo.typ"
#include "ch/10-architettura.typ"
#include "ch/11-protocolli.typ"
#include "ch/12-fsoe.typ"
#include "ch/13-campo-timing.typ"
#include "ch/14-diagnosi.typ"
#include "ch/15-limiti.typ"

// -------------------------------------------------------------- appendici ----
#include "ap/appendici.typ"

// ---------------------------------------------------------------- glossario --
#pagebreak()
= Glossario
#v(0.5em)
#glossary-print(entries)
