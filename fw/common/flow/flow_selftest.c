#include "flow_selftest.h"

#include "flow_config.h"
#include "flow_crc.h"
#include "flow_mem.h"
#include "flow_platform.h"
#include "flow_shm.h"

/* Aree di prova dentro la finestra OCRAM condivisa, fuori dalle strutture: un
 * write/read qui valida i permessi della patch SM 0002 (TRDC/DID) senza toccare
 * la mailbox. */
#define PROBE_SHM_ADDR (FLOW_SHM_BASE_ADDR + 0xF00u)
#define PROBE_ICC_ADDR (FLOW_ICC_BASE_ADDR + 0x300u)
#define PROBE_PATTERN 0xA5A5C3C3u

static bool s_failed;

void flow_selftest_check(const char *name, bool ok)
{
    flow_platform_log("selftest", "%-24s %s", name, ok ? "PASS" : "FAIL");
}

static void check(const char *name, bool ok)
{
    flow_selftest_check(name, ok);
    if (!ok) {
        s_failed = true;
    }
}

/* CRC-32/ISO-HDLC e CRC-16/CCITT-FALSE sui vettori noti "123456789". */
static void test_crc(void)
{
    static const char vector[] = "123456789";

    check("crc32 vector", flow_crc32(vector, 9u) == 0xCBF43926u);
    check("crc16 vector", flow_crc16(vector, 9u) == 0x29B1u);
}

/* Split/assemble del frame FSoE: nessuna delle due meta' e' un frame valido, e
 * l'assemblaggio deve restituire esattamente l'originale. */
static void test_split(void)
{
    const flow_u8 frame11[FLOW_PDU_MAX] = {0x35u, 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u, 0x88u, 0xFEu,
        0xFFu};
    const flow_u8 frame6[FLOW_PDU_MIN] = {0x04u, 0x5Au, 0x12u, 0x34u, 0xFEu, 0xFFu};
    flow_u8 crc_part[FLOW_PDU_MAX];
    flow_u8 data_part[FLOW_PDU_MAX];
    flow_u8 merged[FLOW_PDU_MAX];
    bool ok;

    flow_shm_split(crc_part, frame11, 11u, true);
    flow_shm_split(data_part, frame11, 11u, false);

    /* Byte CRC di un frame da 11 byte: 3, 4, 7, 8 (ETG.5100). */
    ok = (flow_memcmp(crc_part, frame11, 11u) != 0) && (flow_memcmp(data_part, frame11, 11u) != 0) &&
        (crc_part[0] == 0u) && (crc_part[1] == 0u) && (crc_part[2] == 0u) && (crc_part[3] == frame11[3]) &&
        (crc_part[4] == frame11[4]) && (crc_part[7] == frame11[7]) && (crc_part[8] == frame11[8]) &&
        (crc_part[9] == 0u) && (crc_part[10] == 0u) && (data_part[9] == frame11[9]) && (data_part[10] == frame11[10]);

    flow_shm_assemble(merged, crc_part, data_part, 11u);
    ok = ok && (flow_memcmp(merged, frame11, 11u) == 0);

    /* Una corruzione nella propria meta' deve essere vista, una nell'altra no. */
    crc_part[3] ^= 0xFFu;
    ok = ok && !flow_shm_part_matches(crc_part, frame11, 11u, true);
    ok = ok && flow_shm_part_matches(data_part, frame11, 11u, false);
    check("split/assemble 11B", ok);

    /* Frame minimo (un byte di dati): CRC nei byte 2 e 3. */
    flow_shm_split(crc_part, frame6, 6u, true);
    flow_shm_split(data_part, frame6, 6u, false);
    flow_shm_assemble(merged, crc_part, data_part, 6u);
    ok = (crc_part[2] == frame6[2]) && (crc_part[3] == frame6[3]) && (crc_part[0] == 0u) &&
        (flow_memcmp(merged, frame6, 6u) == 0);
    check("split/assemble 6B", ok);
}

/* Finestra OCRAM condivisa: se il TRDC/DID non la concede, il core prende un
 * fault qui (e non a meta' del ciclo FSoE). */
static void test_ocram(void)
{
    volatile flow_u32 *shm_probe = (volatile flow_u32 *)PROBE_SHM_ADDR;
    volatile flow_u32 *icc_probe = (volatile flow_u32 *)PROBE_ICC_ADDR;
    bool ok;

    *shm_probe = PROBE_PATTERN;
    *icc_probe = ~PROBE_PATTERN;
    ok = (*shm_probe == PROBE_PATTERN) && (*icc_probe == ~PROBE_PATTERN);

    /* Le due finestre devono essere aree distinte. */
    *shm_probe = 0x1234ABCDu;
    *icc_probe = 0x5678EF01u;
    ok = ok && (*shm_probe == 0x1234ABCDu) && (*icc_probe == 0x5678EF01u) && (PROBE_SHM_ADDR != PROBE_ICC_ADDR);

    *shm_probe = 0u;
    *icc_probe = 0u;

    check("ocram condivisa", ok);
}

bool flow_selftest_run(flow_core_id_t self)
{
    s_failed = false;

    flow_platform_log("selftest", "ruolo %s: PDU max %u B, mailbox %u B, versione %u",
        flow_role_log_tag(self), (unsigned)FLOW_PDU_MAX, (unsigned)sizeof(flow_shm_t),
        (unsigned)FLOW_SHM_VERSION);

    test_crc();
    test_split();
    test_ocram();

    flow_platform_log("selftest", "RESULT: %s", s_failed ? "FAIL" : "PASS");

    return !s_failed;
}
