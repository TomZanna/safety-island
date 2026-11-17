#import "../lib/style.typ": *
= Campo simulato, tempi e footprint

== Il campo

Il ruolo `FIELDBUS` non è un componente di sicurezza: è lì per far girare l'ambiente e per non far scattare
i watchdog. Gira ogni 2 ms e a ogni passo:

1. legge dalla mailbox il frame che entrambi i core hanno pubblicato (`MIN` + interleave);
2. lo consegna allo slave FSoE (`fsoeslave_sync_with_master`);
3. tiene da parte la risposta dello slave;
4. ogni cinque passi (10 ms) pubblica *una* RX e apre il ciclo successivo.

Il processo di campo è un generatore pseudo-casuale *deterministico* (xorshift seedato dal ciclo DWT e dal
ruolo): cambia stato ogni ~500 ms con un bias verso "macchina in marcia" (90% E-stop rilasciato, 80% porta
chiusa) e dà un impulso di reset quando le condizioni di sicurezza sono soddisfatte. Deterministico perché un
test che non si ripete non serve a niente: con lo stesso seed si rivede la stessa sequenza.

#nota[
  Gli ingressi sono "a caso" per scelta: l'obiettivo del banco è esercitare il ciclo completo
  (abilitazione, apertura porta, reset) senza un processo fisico. Il comando `s` da #term("rtt") inietta una
  discrepanza fra i due canali, per verificare il monitoraggio.
]

== Tempi

#si-table(
  columns: (1.8fr, 1.2fr, 2.8fr),
  header: ([Cosa], [Periodo], [Note]),
  rows: (
    ([ciclo safe], [10 ms], [tick da SysTick a 1 kHz; il tempo per il watchdog viene da DWT, per non avere una causa comune]),
    ([bus/fieldbus], [2 ms], [tick separato: lo slave risponde spesso, il watchdog FSoE (100 ms) non scatta mai]),
    ([barrier], [10 ms], [aperta dal fieldbus quando entrambi i core hanno finito; timeout 30 ms]),
    ([cross-check], [–], [timeout 5 ms per singolo scambio, misurato con DWT]),
  ),
)

Nei run puliti il jitter misurato è zero: `slip 0` su oltre 10 000 cicli safe (il contatore conta solo i
ritardi oltre due periodi). Il margine del watchdog è 10×: un ciclo perso non è un problema, dieci sì.

== Footprint

#si-table(
  columns: (1.2fr, 1.2fr, 1.2fr, 1.2fr),
  header: ([App], [text], [data+bss], [stack]),
  rows: (
    ([`m70`], [42 KB (17%)], [10.7 KB (6%)], [4 KB]),
    ([`m71`], [42 KB (17%)], [10.7 KB (6%)], [4 KB]),
    ([`m33s`], [37 KB (15%)], [10.6 KB (6%)], [4 KB]),
  ),
)

Sono i numeri di `--print-memory-usage` sui TCM: c'è spazio per crescere (più connessioni FSoE, più
diagnostica) senza toccare il linker script. Nessuna allocazione dinamica: le strutture (`flow_safe_t`,
`flow_field_t`, gli stack FSoE) sono statiche, e l'unica cosa "grande" sullo stack è il buffer di 200 byte
del log RTT.
