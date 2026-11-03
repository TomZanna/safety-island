Implementa questo diagramma su un PC desktop (Linux) tramite thread nativi (POSIX `pthread` o `std::thread` in C/C++) per validare l'architettura prima del porting su target dual-core reale (es. NXP i.MX943).

La libreria FSoE di rt-labs è scritta in C ANSI pulito, non usa variabili globali e delega il layer di basso livello al Platform Abstraction Layer (PAL), rendendo il test su PC immediato.

---

### 1. Mappatura dell'architettura sui thread PC

* **Thread 1: `A55_comm_thread` (Black Channel Emulation)**
* Gira con cadenza rapida o disaccoppiata (es. ciclo `clock_nanosleep` da 2 ms o 5 ms).


* Gestisce la simulazione del bus EtherCAT / campo: se riceve un frame emesso dai core safe, prepara la risposta FSoE (ad esempio instanziando uno stack slave fittizio con `fsoeslave_sync_with_master()` o facendo da loopback) e aggiorna i buffer RX della SHMEM.




* **Thread 2: `M7_safe_core_thread` (Safety Core 1)**
* Istanza dedicata dello stack master: `fsoemaster_t master_m7`.


* Ciclo periodico rigido (es. 10 ms).


* **Thread 3: `M33_safe_core_thread` (Safety Core 2)**
* Seconda istanza indipendente dello stack master: `fsoemaster_t master_m33`.


* Ciclo sincrono a M7 con barriere di sincronizzazione.





---

### 2. Idee e Meccanismi di Sincronizzazione su PC

#### A. Il "Safe HW Timer Tick" comune (Sincronizzazione M7/M33)

Per emulare il tick hardware sincrono tra i due core safe senza incorrere in drift:

* Usa un `pthread_barrier_t` inizializzato a 2 (o un `std::barrier` in C++20).
* All'inizio di ogni iterazione di 10 ms, entrambi i thread safe attendono la barriera:
```c
pthread_barrier_wait(&safe_tick_barrier);

```


* Per il timing, solo M7 (o un timer OS tramite `timerfd_create` su Linux) scandisce il periodo di 10 ms con `clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ...)` e sblocca la barriera del ciclo.



#### B. Emulazione della SHMEM Mailbox

Sul PC la SHMEM può essere una semplice `struct` in memoria heap o globale, ma trattata rigorosamente con accessi atomici o sequenziali:

```c
typedef struct {
    uint8_t pdu[FSOEMASTER_FRAME_SIZE(SAFE_DATA_SIZE)];
    uint32_t len;
} FsoeSlot;

typedef struct {
    // A55 -> Cores (RX)
    FsoeSlot rx_slots[2];
    atomic_uint rx_active_idx;

    // Cores -> A55 (TX) con 2-phase commit
    FsoeSlot tx_slots[2];
    atomic_uint tx_m7_idx;
    atomic_uint tx_m33_idx;
} ShmemMailbox;

```

#### C. Cross-Checking tra M7 e M33 (IPC inter-core emulata)

Nel target reale useresti registri IPC/MU (Messaging Unit) o shared RAM protetta con MPU. Su PC puoi usare due semplici mailbox lock-free o una seconda barriera pthread:

1. **Passo 1 (Egress PDU Check)**: Quando `fsoeapp_send()` viene invocato dallo stack dentro `sync_with_slave()`, M7 deposita il CRC/Digest locale e M33 fa lo stesso. Dopo un `pthread_barrier_wait()`, ciascuno legge il digest dell'altro. Se uguali, aggiornano rispettivamente `tx_m7_idx` e `tx_m33_idx`.


2. **Passo 2 (Input Check)**: Dopo il ritorno da `fsoemaster_sync_with_slave()`, un altro punto di rendezvous verifica che `inputs_m7` sia identico a `inputs_m33`.



---

### 3. Come collegare il Black Channel (A55) su PC

Per rendere il test significativo, A55 deve scambiare frame reali. Ci sono due strategie eccellenti:

1. **Loopback con Slave Stack rt-labs nello stesso binario**:
Il thread A55 può istanziare internamente un `fsoeslave_t` usando le API slave (`fsoeslave_init`, `fsoeslave_sync_with_master`).


* A55 preleva il frame TX generato da M7/M33 e lo passa a `fsoeslave_sync_with_master()`.


* Lo stack slave genera la PDU di risposta FSoE e A55 la scrive nei buffer RX di SHMEM.


* In questo modo testi l'intero ciclo di vita FSoE (Reset $\rightarrow$ Session $\rightarrow$ Connection $\rightarrow$ Parameter $\rightarrow$ Data) completamente in locale.




2. **Socket UDP locale (Network Black Channel)**:
A55 può inviare e ricevere le PDU su socket UDP `127.0.0.1`. Questo consente di testare il comportamento reale del Black Channel: puoi introdurre un piccolo proxy software che simula perdita pacchetti, jitter, duplicazione o corruzione di bit per verificare che i watchdog e i reset entrino in funzione come da specifica.



---

### 4. Implementazione del PAL (`fsoeport.c`) su PC

Per compilare la libreria rt-labs sul computer basta implementare le funzioni richieste dal manuale (Capitolo 5):

* `fsoeport_memcpy()`, `fsoeport_memset()`, `fsoeport_memcmp()`: mappati direttamente sulle funzioni standard della `<string.h>`.


* `fsoeport_current_time_us()`: implementato tramite `clock_gettime(CLOCK_MONOTONIC)`:


```c
uint32_t fsoeport_current_time_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)((ts.tv_sec * 1000000ULL) + (ts.tv_nsec / 1000ULL));
}

```


* `fsoeport_macros.h`: imposta `FSOEPORT_IS_BIG_ENDIAN` a `0` (x86_64 / ARM standard sono Little Endian) e definisci `FSOEPORT_ASSERT(exp)` con `assert(exp)`.



---

### 5. Vantaggi immediati di questo test preliminare

* **Debugging visuale a stati**: Tramite `fsoemaster_get_state()` e `fsoemaster_state_description()` puoi stampare a terminale l'avanzamento sincronizzato dei due core M7/M33 durante la transizione fino allo stato `DATA`.


* **Validazione delle callback**: Permette di verificare che le callback `fsoeapp_generate_session_id()` generino lo stesso Session ID per entrambi i core (requisito fondamentale 1oo2, Checklist #4).


* **Fault Injection Test**: Puoi forzare volutamente un bit errato in una variabile di M33 durante il cross-check per verificare che il sistema scateni correttamente `fsoemaster_set_reset_request_flag()` e porti le uscite in fail-safe (0).



Vuoi che prepari uno scheletro C essenziale con la struttura dei 3 thread e le barriere di sincronizzazione per iniziare a compilare il prototipo?



# Session ID
per instaurare il session id i due thread safe generano un numero casuale e poi se lo scambiano. In numero finale quindi è la somma dei due numeri

# CRC hardware
il CRC hardware non si fa

# messaging units come black channels
sequence number indipendente da quello di fsoe
digest per proteggere il messaggio nella sua interezza
una qualche connession ID per distinguere i vari destinatari
....


# compile time
l'implementazione deve prendere la libreria come un qualcosa da linkare, quindi facciamo un gcc di ogni componente a se e poi linkiamo insieme
