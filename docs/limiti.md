# Limiti e lavoro futuro {#limiti}

Questo e' un banco di prova, non un prodotto certificato. I limiti sono voluti
e vanno tenuti presenti quando si legge il codice.

## Sicurezza {#limiti-sicurezza}

| Limite | Cosa manca / cosa fare |
|---|---|
| RNG di banco | il Session ID usa DWT+SysTick+ruolo (`FLOW_BENCH_RNG`). In produzione serve il TRNG dell'ELE con health test; con il flag a 0 la funzione e' fatale, mai un ripiego pseudo-casuale |
| MPU/ECC/TRDC | le finestre OCRAM sono condivise per configurazione dell'SM, ma i core non impostano regioni MPU non-cache ne' gestiscono l'ECC: va fatto prima di fidarsi della memoria |
| Diversita' dei core | il 1oo2 e' su due M7 identici: nessuna diversita' hardware. Un 1oo2 "vero" vorrebbe core diversi (M7+M33) o un lockstep, e questo cambia il canale di cross-check (non c'e' MU fra M33S e M7): vedi @ref architettura |
| Watchdog esterni | WDOG5/6/7 non sono abilitati: `flow_platform_watchdog_kick()` e' un no-op |
| FMEDA, WCET, MISRA | non fatti: mancano analisi dei guasti, misure di worst case e qualificazione degli strumenti |
| Catena FSoE completa | manca la parte di campo reale (EtherCAT/A55) e la certificazione dello slave |

## Firmware e integrazione {#limiti-fw}

- *A55/Linux*: qui il ruolo A55 e' il M33_1. Il driver Linux di `imx-flow`
  (`flow_a55_drv.c`, ioctl) non e' stato portato: se serve, va allineato
  all'ABI versione 2 (heartbeat per tre ruoli).
- *Una sola connessione FSoE* (`FLOW_NUM_CONNECTIONS = 1`): le strutture e i
  canali sono gia' indicizzati per connessione, ma il campo e i comandi RTT
  assumono `k = 0`.
- *Nessuna periferica*: le app non usano UART, GPIO o timer hardware oltre a
  SysTick/DWT. Tutto cio' che tocca pin o clock deve passare dall'SM (SCMI), e
  questo va progettato caso per caso.
- *Memoria condivisa*: le due finestre sono dimensionate per il caso attuale
  (16 KB + 1 KB). Crescere significa una nuova patch SM e `make sm-update`.

## Cosa sarebbe il passo successivo {#prossimi-passi}

Nell'ordine in cui lo faremmo:

1. *robustezza*: halt di un core per piu' di 30 s (l'altro deve restare in
   fail-safe e riprendersi), run di un'ora con contatori sotto controllo,
   misure di WCET e stack;
2. *memoria*: MPU non-cache, ECC, verifica del TRDC sui tre core;
3. *entropia vera*: TRNG ELE + health test, poi `FLOW_BENCH_RNG=0` nel build di
   produzione;
4. *campo reale*: EtherCAT sull'A55 con lo slave sul drive, il driver Linux
   allineato all'ABI;
5. *processo*: analisi dei guasti e qualificazione, che e' un lavoro diverso da
   "far girare il codice".

> La scelta di mettere il 1oo2 sui due M7 (e non su M7+M33) e' documentata in
> @ref architettura non e' una preferenza, e' il vincolo delle MU. Se in futuro
> servisse diversita', il punto da cambiare e' il trasporto del cross-check
> (oggi MU18 + ICC), non la logica di sicurezza.
