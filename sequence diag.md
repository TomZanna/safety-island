```mermaid
sequenceDiagram
    autonumber
    participant a55 as A55 (Black Channel / Comm Engine)
    participant shm as SHMEM Mailbox (Rx/Tx Buffers)
    participant m7 as M7 (Safety Core 1)
    participant m33 as M33 (Safety Core 2)

    Note over a55,shm: Asynchronous A55 Task: Cyclic fieldbus exchange & continuous SHMEM updates

    rect rgb(255, 245, 230)
        Note over m7,m33: 1. Deterministic Safe Tick (Hardware Timer / Sync Tick)
        m7->>m7: HW Safe Timer Tick (e.g., 10ms)
        m33->>m33: HW Safe Timer Tick (e.g., 10ms)

        Note over m7,m33: Local Latch of Raw Received Frames from SHMEM
        m7->>shm: Atomic copy raw RX frames -> Local RAM
        m33->>shm: Atomic copy raw RX frames -> Local RAM
    end

    rect rgb(240, 245, 255)
        Note over m7,m33: 2. FSoE Stack Synchronization (Inversion of Control)
        Note over m7,m33: Pass SafeOutputs computed in previous cycle (or fail-safe 0 at boot)

        loop For each FSoE connection k (1..N)
            m7->>m7: fsoemaster_sync_with_slave(m7_inst[k], SafeOutputs[k], &inputs_m7[k], &status_m7[k])
            m33->>m33: fsoemaster_sync_with_slave(m33_inst[k], SafeOutputs[k], &inputs_m33[k], &status_m33[k])

            Note over m7,m33: Stack invokes fsoeapp_recv() -> provides latched raw frame
            Note over m7,m33: Stack invokes fsoeapp_send() -> generates TX frame for connection k

            m7->>m33: Exchange generated PDU Digest/CRC
            m33->>m7: Exchange generated PDU Digest/CRC

            alt Identical generated PDUs (Cross-check OK)
                m7->>shm: Stage/Write PDU[k] into SHMEM TX buffer
                m33->>shm: Validate / atomic commit of TX slot
            else Divergent PDUs (State or encoding mismatch)
                Note over m7,m33: fsoemaster_set_reset_request_flag()
            end
        end
    end

    rect rgb(235, 255, 235)
        Note over m7,m33: 3. Cross-Checking Decoded SafeInputs
        m7->>m33: Exchange inputs_m7[1..N] and status_m7[1..N]
        m33->>m7: Exchange inputs_m33[1..N] and status_m33[1..N]

        alt Data & Status Match (status.is_process_data_received == true)
            Note over m7,m33: SafeInputs[1..N] = Valid received process data
        else Mismatch or Communication Error
            Note over m7,m33: SafeInputs[1..N] = 0 (Fail-Safe)
            Note over m7,m33: fsoemaster_clear_process_data_sending_enable_flag()
        end
    end

    rect rgb(245, 255, 240)
        Note over m7,m33: 4. Safe Logic Execution (Compute SafeOutputs for cycle k+1)
        m7->>m7: Compute safe logic(SafeInputs) -> SafeOutputs_m7[1..M]
        m33->>m33: Compute safe logic(SafeInputs) -> SafeOutputs_m33[1..M]

        m7->>m33: Exchange logic output digests
        m33->>m7: Exchange logic output digests

        alt Safe Logic Digests Match
            m7->>m7: fsoemaster_set_process_data_sending_enable_flag()
            m33->>m33: fsoemaster_set_process_data_sending_enable_flag()
            Note over m7,m33: SafeOutputs stored for next fsoemaster_sync_with_slave()
        else Logic Divergence
            Note over m7,m33: SafeOutputs[1..M] = 0
            m7->>m7: fsoemaster_clear_process_data_sending_enable_flag()
            m33->>m33: fsoemaster_clear_process_data_sending_enable_flag()
            m7->>m7: fsoemaster_set_reset_request_flag()
            m33->>m33: fsoemaster_set_reset_request_flag()
        end
    end