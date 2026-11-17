#import "../lib/style.typ": *
= Limiti e lavoro futuro

Questo è un banco di prova, non un prodotto certificato. I limiti sono voluti e vanno tenuti presenti
quando si legge il codice.

== Sicurezza

#si-table(
  columns: (1.7fr, 3.2fr),
  header: ([Limite], [Cosa manca / cosa fare]),
  rows: (
    ([RNG di banco], [il Session ID usa DWT+SysTick+ruolo (`FLOW_BENCH_RNG`). In produzione serve il TRNG dell'ELE con health test; con il flag a 0 la funzione è fatale, mai un ripiego pseudo-casuale]),
    ([MPU/ECC/TRDC], [le finestre OCRAM sono condivise per configurazione dell'SM, ma i core non impostano regioni MPU non-cache né gestiscono l'ECC: va fatto prima di fidarsi della memoria]),
    ([Diversità dei core], [il 1oo2 è su due M7 identici: nessuna diversità hardware. Un 1oo2 "vero" vorrebbe core diversi (M7+M33) o un lockstep, e questo cambia il canale di cross-check (non c'è MU fra M33S e M7) (§ @fig-architettura)]),
    ([Watchdog esterni], [WDOG5/6/7 non sono abilitati: `flow_platform_watchdog_kick()` è un no-op]),
    ([FMEDA, WCET, MISRA], [non fatti: mancano analisi dei guasti, misure di worst case e qualificazione degli strumenti]),
    ([Catena FSoE completa], [manca la parte di campo reale (EtherCAT/A55) e la certificazione dello slave]),
  ),
)

== Firmware e integrazione

- *A55/Linux*: qui il ruolo A55 è il M33_1. Il driver Linux di `imx-flow` (`flow_a55_drv.c`, ioctl) non è
  stato portato: se serve, va allineato all'ABI versione 2 (heartbeat per tre ruoli).
- *Una sola connessione FSoE* (`FLOW_NUM_CONNECTIONS = 1`): le strutture e i canali sono già indicizzati
  per connessione, ma il campo e i comandi RTT assumono `k = 0`.
- *Nessuna periferica*: le app non usano UART, GPIO o timer hardware oltre a SysTick/DWT. Tutto ciò che
  tocca pin o clock deve passare dall'SM (#term("scmi")), e questo va progettato caso per caso.
- *Memoria condivisa*: le due finestre sono dimensionate per il caso attuale (16 KB + 1 KB). Crescere
  significa una nuova patch SM e `make sm-update`.

== Cosa sarebbe il passo successivo

Nell'ordine in cui lo faremmo:

1. *robustezza*: halt di un core per più di 30 s (l'altro deve restare in fail-safe e riprendersi), run di
   un'ora con contatori sotto controllo, misure di WCET e stack;
2. *memoria*: MPU non-cache, ECC, verifica del TRDC sui tre core;
3. *entropia vera*: TRNG ELE + health test, poi `FLOW_BENCH_RNG=0` nel build di produzione;
4. *campo reale*: EtherCAT sull'A55 con lo slave sul drive, il driver Linux allineato all'ABI;
5. *processo*: analisi dei guasti e qualificazione, che è un lavoro diverso da "far girare il codice".

#nota[
  La scelta di mettere il 1oo2 sui due M7 (e non su M7+M33) è documentata in @fig-architettura: non è una
  preferenza, è il vincolo delle MU. Se in futuro servisse diversità, il punto da cambiare è il trasporto del
  cross-check (oggi MU18 + ICC), non la logica di sicurezza.
]
