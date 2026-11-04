# imx943-flow — prototipo 1oo2 dual-core FSoE su PC

Emulazione su Linux, con thread nativi POSIX, dell'architettura di sicurezza
descritta in `../sequence diag.md` e `../pseudo_code.md`: due core safe
(**M7** e **M33**) che eseguono due istanze indipendenti dello stack FSoE
master di [rt-labs](../example-code/fsoe), si scambiano i cross-check tramite
una **Messaging Unit** emulata e parlano con il campo attraverso il **core
A55** (black channel), che usa una mailbox in **shared memory** e uno stack
FSoE slave emulato in locale.

L'obiettivo è validare l'architettura (stati FSoE, cross-check 1oo2, session
id, fail-safe, watchdog) *prima* del porting sul target i.MX943.

---

## 1. Mappatura thread

| Thread | File | Cadenza | Ruolo |
|---|---|---|---|
| `M7_safe_core_thread` | `cores/m7_core.c` → `safe/safe_core.c` | 10 ms (tick master, `clock_nanosleep` assoluto) | stack master `fsoemaster_t` #1, esegue i 4 passi del diagramma |
| `M33_safe_core_thread` | `cores/m33_core.c` → `safe/safe_core.c` | stesso tick, sbloccato da M7 | stack master `fsoemaster_t` #2, **codice identico** a M7 |
| `A55_comm_thread` | `a55/a55_core.c` | 2 ms (asincrono) | black channel: legge/scrive la SHMEM, esegue lo stack slave, simula il processo di campo |

Componenti di supporto (ognuno compilato a parte):

| File | Contenuto |
|---|---|
| `shmem/shmem_mailbox.c` | mailbox SHMEM emulata (`mmap(MAP_SHARED\|MAP_ANON)`): 3 banchi + seqlock per canale, contatore monotono per scrittore. Il frame TX e' **spezzato**: un core scrive i CRC, l'altro il resto, l'A55 li interleave |
| `mu/mu_channel.c` | Messaging Unit emulata: 5 canali logici, messaggi con channel id, src/dst, sequence number MU (indipendente dal seq FSoE), digest sull'intero messaggio, rendezvous con timeout |
| `safe/safe_tick.c` | "Safe HW Timer Tick" condiviso M7/M33 (M7 = timer hardware) |
| `safe/safe_logic.c` | mini logica safe: 1oo2 con discrepanza, latch, reset manuale, uscita fail-safe = 0 |
| `safe/safe_core.c` | ciclo safe completo (passi 1-4 del diagramma) |
| `glue/fsoe_app_glue.c` | **unica** implementazione delle callback `fsoeapp_*`, dispatch su `app_ref` |
| `app/runtime.c`, `app/main.c` | costruzione del sistema, CLI, report finale |
| `app/fault_inject.c` | fault injection per validare i rami `else` del diagramma |

## 2. Compilazione e link (libreria come artefatto separato)

```
make            # compila ogni .c separatamente, crea build/libfsoe.a, poi linka
make run        # build + esecuzione (2 s di dimostrazione)
make demo       # esecuzione con traccia per ciclo
make check      # matrice di fault injection (6 scenari, PASS/FAIL)
make clean
```

Il Makefile compila:

1. **la libreria FSoE** (`../example-code/fsoe/src/*.c` + `src/port/linux/fsoeport.c`)
   → `build/fsoe/**.o` → `ar rcs build/libfsoe.a`
2. **ogni componente** del prototipo → `build/obj/**.o`
3. link finale: `gcc ... build/obj/**.o build/libfsoe.a -pthread`

Opzioni di build: `CONNECTIONS=2`, `LOG=1` (log interni dello stack),
`SANITIZE=addr` (ASan+UBSan), `SANITIZE=1` (TSan), `DEBUG=1`.

## 3. Esecuzione

```
./build/imx943-flow [opzioni]

  --duration MS      durata, 0 = fino a Ctrl-C (default 2000)
  --cycle-ms MS      ciclo safe M7/M33 (default 10)
  --a55-ms MS        ciclo bus A55 (default 2)
  --seed N           seme RNG (run riproducibili)
  --fault NAME       none|session|pdu|inputs|logic|busdrop
  --fault-cycle N    ciclo in cui iniettare il guasto
  --fault-count N    durata del guasto in cicli
  -v/--verbose       una riga per ciclo safe e per ciclo bus
  -t/--trace         una riga per frame FSoE e per messaggio MU
  -q/--quiet         solo errori
```

Esempio di uscita (fasi salienti):

```
[  0.010] M7   | k=0 connection reset by master, cause: local reset
[  0.020] M33  | k=0 SESSION ID negotiation: rnd(self)=0x45fa + rnd(peer)=0x07f8 = 0x4df2
[  0.020] M7   | k=0 SESSION ID negotiation: rnd(self)=0x07f8 + rnd(peer)=0x45fa = 0x4df2
[  0.020] M7   | k=0 FSoE state: Reset -> Session
[  0.030] M7   | k=0 FSoE state: Session -> Connection
[  0.040] M7   | k=0 FSoE state: Connection -> Parameter
[  0.070] M7   | k=0 FSoE state: Parameter -> Data
[  0.072] A55  | k=0 SafeOutputs received over FSoE and read from SHMEM: DISABLED (w0=0x0000 w1=0x0018)
[  0.222] A55  | k=0 SafeOutputs received over FSoE and read from SHMEM: ENABLED  (w0=0x00c3 w1=0x0061)
[  0.722] A55  | k=0 SafeOutputs received over FSoE and read from SHMEM: DISABLED (w0=0x0000 w1=0x0010)
[  1.172] A55  | k=0 SafeOutputs received over FSoE and read from SHMEM: ENABLED  (w0=0x00c3 w1=0x0061)
```

e il report finale con i contatori di tutti i componenti e il verdetto
`RESULT: PASS/FAIL` (exit code 0/1).

## 4. Ciclo safe (1:1 con il sequence diagram)

```
M7/M33:
 1. flow_safe_tick_wait()                 tick hardware comune
    shmem latch                           ogni core legge il frame RX piu' recente
                                          con il PROPRIO contatore (nessun annuncio)
                                          e ne RICALCOLA il digest
 2. fsoemaster_sync_with_slave()          stack FSoE (inversion of control)
      fsoeapp_recv()  -> frame latched
      fsoeapp_send()  -> MU: scambio del digest del PDU COMPLETO generato
                         se coincide: scrive SOLO LA PROPRIA META' nel proprio
                         banco SHMEM (M7 = CRC, M33 = comando+dati+ConnID),
                         rilegge la meta' scritta e la confronta byte a byte,
                         poi PUBBLICA (tx_count++)
                         altrimenti fail-safe e nessuna pubblicazione
 3. MU: cross-check SafeInputs + status + tx_count/rx_count/rx_digest
                                           se diversi -> SafeInputs=0, fail-safe,
                                           reset e riallineamento a MIN(own,peer)
 4. safe_logic_step() + MU: cross-check del digest delle SafeOutputs
                                           se uguali -> set sending enable
                                           se diversi -> uscite 0, clear enable, reset

A55 (2 ms):
   common = MIN(tx_count_m7, tx_count_m33)   <- ultimo frame scritto da ENTRAMBI
   legge le due META' di quel frame e le INTERLEAVA in un'unica PDU
   consegna al FSoE slave                     <- nessun controllo: solo merge+invio
```

All'avvio, **prima** di creare la SHMEM e i thread, gira un self-test (KAT) dei
meccanismi condivisi dai due canali; se fallisce il sistema non parte.


## 5. SHMEM: frame spezzato fra i due core, assemblato dall'A55

Il frame FSoE che va sul bus **non viene prodotto da un solo core**: e' la
composizione di due contributi indipendenti.

```
 M7  ---- scrive solo i CRC (offset 3+4j) ---------------------\
                                                               >  A55  --PDU--> slave
 M33 ---- scrive comando + safety data + ConnID (il resto) ---/   (interleaving)
```

Perche' e' fatto cosi' (e perche' non e' un CCF aggiunto, ma una difesa):

* **Un guasto singolo non puo' produrre un frame accettato.** I CRC di un core
  devono essere coerenti con i dati dell'altro; se una delle due meta' e'
  sbagliata il frame non e' un FSoE valido e lo **slave** lo scarta con
  `INVALID_CRC` → fail-safe. Il controllo end-to-end e' quindi fatto dal CRC
  standard FSoE, **dentro lo slave**: un componente che non e' nessuno dei due
  core e non e' il black channel.
* **L'A55 non controlla niente** (come da diagramma): prende `common =
  MIN(tx_count_m7, tx_count_m33)` e interleavea le due meta'. Se non sono
  coerenti, se ne accorge lo slave.
* **Nessun coordinatore, nessun annuncio.** Ogni core scrive nel **proprio**
  array di banchi con il **proprio** contatore; la pubblicazione e' gated dal
  cross-check MU, quindi i contatori avanzano insieme per costruzione.
* **Indice monotono, `mod N` solo per indirizzare** (`N = 3`, triple buffer):
  con N=2 `MIN()` si romperebbe appena un core resta indietro di 2. Con N=3 lo
  scarto tollerato e' 2; lo scarto reale e' 0/1 perche' la pubblicazione e'
  gated dal cross-check (e una deriva e' comunque un fault rilevato).
* **Il digest non si eredita da nessuno**: il core ricalcola sempre il digest
  sul frame che ha latchato/scritto (`rx_digest`, read-back della propria
  meta'), e il cross-check del PDU usa il digest del frame **completo**
  calcolato dal core stesso.
* **Come e quando ci si riallinea**: nella stessa chiamata in cui si rileva un
  disallineamento dei contatori, entrambi applicano `MIN(proprio, peer)`.
  `MIN()` e' commutativa → stessa decisione, stesso ciclo, nessun comando
  scambiato. Si converge al ribasso cosi' il valore comune non supera mai un
  indice che entrambi hanno scritto.
* **Direzione RX (A55 → core)**: c'e' **un solo scrittore** (l'A55); ogni core
  legge il frame piu' recente con il proprio `rx_count` e ne ricalcola il
  digest. Se i due core leggono frame diversi, `rx_index`/`rx_digest`
  differiscono → guasto → fail-safe.

Non si usa il numero del ciclo safe come indice: deriverebbe entrambi i core da
una **sorgente comune** (il tick), quindi un guasto su quella sorgente li
farebbe sbagliare *insieme* (common cause).

## 6. Altre scelte implementative

* **Session ID** (`pseudo_code.md` §"Session ID"): ogni core genera un numero
  casuale proprio, lo scambia sulla MU e il Session ID usato e' la **somma**
  dei due. Loggato da entrambi i core: gli addendi sono scambiati, la somma
  coincide.
* **CRC hardware**: non emulato, come richiesto. L'integrita' e' garantita da un
  **digest FNV-1a 32 bit** su: messaggi MU (intero messaggio, header incluso),
  slot SHMEM (con seqlock), cross-check PDU/input/output. Il CRC SR-A dello
  stack FSoE resta quello della libreria.
* **Barriera M7/M33**: invece di `pthread_barrier_t` (senza `timedwait`
  affidabile) si usa mutex + condvar su `CLOCK_MONOTONIC`: stessa semantica,
  ma con timeout -> nessun deadlock se un core e' in ritardo.
* **Rendezvous MU**: `mu_exchange()` (pubblica + attende il peer) con timeout;
  se il peer non risponde la cross-check fallisce e si va in fail-safe.
* **Black channel**: opzione 1 del pseudo-code, slave stack rt-labs nello
  stesso processo (loopback deterministico). Il punto di innesto per
  l'opzione 2 (socket UDP + proxy che introduce perdite/jitter) e' isolato in
  `a55/a55_core.c` (`a55_read_shmem` / `a55_publish_slave_frame`).
* **Callback globali**: `fsoeapp_send/recv/generate_session_id/...` sono uniche
  nel processo e fanno dispatch su `app_ref` (`flow_app_ctx_t`). Sul target
  ogni core ha il proprio binario, quindi il dispatch e' un no-op.
* **Self-test di avvio (KAT)** — `app/self_test.c`, eseguito all'inizio di
  `flow_runtime_init()` **prima** di allocare la SHMEM e di creare i thread:
  - vettori di riferimento del digest FNV-1a (il verificatore usato da tutti i
    cross-check e dai messaggi MU): un digest guasto/bloccato si scopre prima
    di iniziare, non dopo;
  - round-trip split/assemble su un frame da 11 byte e uno da 6, verifica che
    le due meta' contengano esattamente i byte attesi e che l'assemblaggio sia
    sensibile a **entrambe** le meta' (se no, la tabella degli offset CRC e'
    sbagliata);
  - sequenza nota della logica safe (E-stop → reset → ENABLE → porta aperta →
    stop → discrepanza → fault latched).
  Se un KAT fallisce il processo **non parte** (exit code 1). Sul target lo
  stesso schema va esteso a MPU/TRDC, ECC, TRNG e clock (fault injection
  hardware), come prevede la fase di init di un sistema safety.

## 7. Fault injection

| `--fault` | Cosa corrompe | Reazione attesa |
|---|---|---|
| `session` | il Session ID finale di M33 | PDU divergenti -> EGRESS CROSS-CHECK FAILED -> reset |
| `pdu` | il digest del PDU completo generato da M33 | EGRESS CROSS-CHECK FAILED -> reset |
| `inputs` | le SafeInputs decodificate da M33 | INPUTS CROSS-CHECK FAILED -> uscite 0, sending disabled (nessun reset, come da diagramma) |
| `logic` | il digest delle SafeOutputs di M33 | divergent SafeOutputs -> uscite 0 + reset |
| `busdrop` | frame scartati dall'A55 (bus cycle) | watchdog FSoE (100 ms) -> `WD_EXPIRED` -> reset e riconnessione |
| `stall` | M33 non pubblica la sua meta': `tx_count` diverge | SHMEM COUNTER MISMATCH -> fail-safe -> riallineamento a MIN() -> reset -> riconnessione |
| `frame` | M33 corrompe la **meta' dati** in SHMEM *dopo* il proprio check (i CRC restano validi) | il frame assemblato non e' un FSoE valido -> lo **slave** risponde `INVALID_CRC` -> reset: e' la prova che il controllo end-to-end e' del CRC FSoE nello slave |

`./check.sh` esegue la matrice completa e verifica sia il verdetto sia la
traccia di detection attesa.

## 8. Mini logica safe

`flow_safe_logic_step()` e' una funzione **pura** `(SafeInputs, stato) -> (SafeOutputs, stato)`:
nessun tempo, nessun I/O, nessun random - e' questo che rende confrontabili i
digest dei due core.

* 2 canali di ingresso (`ch_a`, `ch_b`): bit0 = E-stop OK, bit1 = porta chiusa,
  bit2 = pulsante di reset;
* valutazione **1oo2** con monitoraggio di **discrepanza** (3 cicli) e fault
  **latchato**;
* uscita `enable` = `0x00C3` (ENABLE) oppure `0x0000` (**fail-safe = 0**),
  più una word di diagnostica;
* reset manuale su fronte di salita, accettato solo se le condizioni di
  sicurezza sono soddisfatte.

Lo scenario di campo simulato dall'A55 (`flow_a55_field_scenario`) produce:
E-stop premuto -> rilascio + reset (ENABLE) -> apertura porta (safe stop) ->
chiusura + reset (ENABLE).

## 9. Limiti noti / note di porting

* Nessun CRC hardware, nessuna MPU/ECC, nessun lockstep: il prototipo valida
  la *logica* dell'architettura, non il silicato.
* Lo stack FSoE e' quello di `example-code/fsoe` (v2.0.0), non modificato.
* TSan non e' eseguibile in questo container (richiede la disabilitazione
  dell'ASLR, bloccata dal sandbox). `make SANITIZE=addr` (ASan+UBSan) gira
  pulito; in piu' il seqlock della SHMEM e' per definizione una corsa "benigna"
  che richiederebbe annotazioni TSan.
* L'A55 non e' una funzione di sicurezza e **non esegue controlli**: si limita a
  `MIN()` + lettura delle due meta' + interleaving + invio. La coerenza del
  frame assemblato e' garantita dal CRC FSoE verificato dallo slave; sul target
  la regione condivisa va comunque protetta con MPU/ECC/TRDC.
* Sul target: `shmem_mailbox.c` va puntato alla regione fisica condivisa,
  `mu_channel.c` ai registri MU, `safe_tick.c` al timer hardware safe, e il
  PAL (`fsoeport.c`) al port della piattaforma.

