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
| `shmem/shmem_mailbox.c` | mailbox SHMEM emulata (`mmap(MAP_SHARED\|MAP_ANON)`), banchi ping-pong + seqlock + 2-phase commit |
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
    shmem.rx_latch()                      copia dei frame RX in RAM locale
    (M7 pubblica su MU il banco letto, M33 legge lo *stesso* banco)
 2. fsoemaster_sync_with_slave()          stack FSoE (inversion of control)
      fsoeapp_recv()  -> frame latched
      fsoeapp_send()  -> MU: scambio digest PDU generata (+ digest RX)
                         se uguali: M7 stage nella SHMEM, M33 valida e committa
                         se diversi: set_reset_request_flag()
 3. MU: cross-check SafeInputs + status    se diversi -> SafeInputs=0, clear sending enable
 4. safe_logic_step() + MU: cross-check del digest delle SafeOutputs
                                           se uguali -> set sending enable
                                           se diversi -> uscite 0, clear enable, reset
```


## 5. Scelte implementative (e scostamenti motivati)

* **Session ID** (`pseudo_code.md` §"Session ID"): ogni core genera un numero
  casuale proprio, lo scambia sulla MU e il Session ID usato è la **somma**
  dei due. Loggato da entrambi i core: gli addendi sono scambiati, la somma
  coincide.
* **CRC hardware**: non emulato, come richiesto. L'integrità è garantita da un
  **digest FNV-1a 32 bit** su: messaggi MU (intero messaggio, header incluso),
  slot SHMEM (con seqlock), cross-check PDU/input/output. Il CRC SR-A dello
  stack FSoE resta quello della libreria.
* **Barriera M7/M33**: invece di `pthread_barrier_t` (senza `timedwait`
  affidabile) si usa mutex + condvar su `CLOCK_MONOTONIC`: stessa semantica,
  ma con timeout → nessun deadlock se un core è in ritardo.
* **Rendezvous MU**: `mu_exchange()` (pubblica + attende il peer) con timeout;
  se il peer non risponde la cross-check fallisce e si va in fail-safe.
* **SHMEM**: 2 banchi per canale, seqlock per ogni banco.
  - A55→core: l'A55 pubblica solo quando nessun core è dentro un ciclo
    (`cores_active == 0`), così M7 e M33 leggono la stessa istantanea;
  - core→A55: **M7 prepara** il banco inattivo, **M33 valida il digest e
    committa** (flip del banco attivo). Solo dopo il commit l'A55 lo vede.
* **Black channel**: opzione 1 del pseudo-code, slave stack rt-labs nello
  stesso processo (loopback deterministico). Il punto di innesto per
  l'opzione 2 (socket UDP + proxy che introduce perdite/jitter) è isolato in
  `a55/a55_core.c` (`a55_read_shmem` / `a55_publish_slave_frame`).
* **Callback globali**: `fsoeapp_send/recv/generate_session_id/...` sono uniche
  nel processo e fanno dispatch su `app_ref` (`flow_app_ctx_t`). Sul target
  ogni core ha il proprio binario, quindi il dispatch è un no-op.

## 6. Fault injection

| `--fault` | Cosa corrompe | Reazione attesa |
|---|---|---|
| `session` | il Session ID finale di M33 | PDU divergenti -> EGRESS CROSS-CHECK FAILED -> reset |
| `pdu` | il digest della PDU generata da M33 | EGRESS CROSS-CHECK FAILED -> reset |
| `inputs` | le SafeInputs decodificate da M33 | INPUTS CROSS-CHECK FAILED -> uscite 0, sending disabled (nessun reset, come da diagramma) |
| `logic` | il digest delle SafeOutputs di M33 | divergent SafeOutputs -> uscite 0 + reset |
| `busdrop` | frame scartati dall'A55 (bus cycle) | watchdog FSoE (100 ms) -> `WD_EXPIRED` -> reset e riconnessione |

`./check.sh` esegue la matrice completa e verifica sia il verdetto sia la
traccia di detection attesa.

## 7. Mini logica safe

`flow_safe_logic_step()` è una funzione **pura** `(SafeInputs, stato) -> (SafeOutputs, stato)`:
nessun tempo, nessun I/O, nessun random — è questo che rende confrontabili i
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

## 8. Limiti noti / note di porting

* Nessun CRC hardware, nessuna MPU/ECC, nessun lockstep: il prototipo valida
  la *logica* dell'architettura, non il silicato.
* Lo stack FSoE è quello di `example-code/fsoe` (v2.0.0), non modificato.
* TSan non è eseguibile in questo container (richiede la disabilitazione
  dell'ASLR, bloccata dal sandbox). `make SANITIZE=addr` (ASan+UBSan) gira
  pulito; in più il seqlock della SHMEM è per definizione una corsa "benigna"
  che richiederebbe annotazioni TSan.
* Sul target: `shmem_mailbox.c` va puntato alla regione fisica condivisa,
  `mu_channel.c` ai registri MU, `safe_tick.c` al timer hardware safe, e il
  PAL (`fsoeport.c`) al port della piattaforma.

