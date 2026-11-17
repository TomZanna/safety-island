# Glossario {#glossario}

| Acronimo | Significato | Nota |
|---|---|---|
| AHAB | Advanced High Assurance Boot | contenitore firmato dell'immagine di boot NXP |
| BCU | Board Control Utility | utility NXP sull'FTDI della EVK: reset e boot mode forzato |
| ELE | EdgeLock Enclave | sottosistema di sicurezza NXP che esegue il firmware di boot firmato |
| FSoE | Functional Safety over EtherCAT | protocollo di sicurezza end-to-end (ETG.5100) su black channel |
| ICC | Inter-Core Channel | canale di cross-check fra i due core safe: payload in OCRAM + doorbell MU18 |
| LM | Logical Machine | partizione logica dell'SM: una per agente (AP, M33S, M7_0, M7_1) |
| MU | Messaging Unit | periferica di scambio messaggi fra due lati cablati (A/B) |
| OCRAM | On-Chip RAM | RAM interna visibile a piu' core: qui stanno RTT e mailbox |
| PDU | Protocol Data Unit | il frame FSoE: comando, dati di processo, parole CRC, Connection ID |
| RTT | Real-Time Transfer | log/input SEGGER via debug probe, senza periferiche |
| SCL | Safety Communication Layer | livello di sicurezza dello stack FSoE: stati, CRC, watchdog |
| SCMI | System Control and Management Interface | protocollo con cui l'SM concede clock, pin e power ai core |
| SDP | Serial Download Protocol | modalita' di boot della ROM che accetta un'immagine via USB (uuu) |
| SWD | Serial Wire Debug | interfaccia di debug a due fili usata dal J-Link |
| SM | System Manager | firmware NXP sul M33_0 che possiede boot, clock, pin e memoria |
| TCM | Tightly Coupled Memory | RAM locale di ogni core secondario, dove girano codice e dati |
| TRDC | Trusted Resource Domain Controller | filtro hardware che decide quale LM puo' accedere a cosa |
