/**
 * @file rtt_shared.c
 * @brief SEGGER RTT con un solo control block condiviso da tutti i core
 * secondari, un canale per core.
 * @ingroup rtt
 *
 */
#include "rtt_shared.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "fsl_device_registers.h"

/* Strutture come in SEGGER_RTT.h (lette cosi' dal J-Link) */
typedef struct
{
  const char* sName;
  char* pBuffer;
  unsigned SizeOfBuffer;
  unsigned WrOff;
  volatile unsigned RdOff;
  unsigned Flags;
} rtt_up_t;

typedef struct
{
  const char* sName;
  char* pBuffer;
  unsigned SizeOfBuffer;
  volatile unsigned WrOff;
  unsigned RdOff;
  unsigned Flags;
} rtt_down_t;

typedef struct
{
  char acID[16];
  int MaxNumUpBuffers;
  int MaxNumDownBuffers;
  rtt_up_t aUp[RTT_NUM_CHANNELS];
  rtt_down_t aDown[RTT_NUM_CHANNELS];
} rtt_cb_t;

/* Layout fisso dell'area condivisa, uguale per tutti i core (i puntatori devono
 * valere per chiunque legga: niente stringhe o buffer nella TCM del singolo
 * core) */
typedef struct
{
  rtt_cb_t cb; /* 0x0000 */
  char names[RTT_NUM_CHANNELS][8]; /* nomi dei canali */
  char down[RTT_NUM_CHANNELS][RTT_DOWN_BUFFER_SIZE]; /* input */
  char up[RTT_NUM_CHANNELS][RTT_UP_BUFFER_SIZE]; /* output */
} rtt_area_t;

_Static_assert(sizeof(rtt_area_t) <= RTT_SHARED_SIZE,
               "area RTT condivisa troppo piccola");

#define AREA ((volatile rtt_area_t*)RTT_SHARED_BASE)
#define RTT_MODE_NO_BLOCK_SKIP 0U

static const char* const s_names[RTT_NUM_CHANNELS] = {"m33s", "m70", "m71"};

void rtt_init(void)
{
  rtt_area_t* a = (rtt_area_t*)RTT_SHARED_BASE;
  const uint32_t ch = RTT_CHANNEL;

  /* Solo i descrittori di questo core: un core che riparte non tocca i canali
   * degli altri */
  strncpy(a->names[ch], s_names[ch], sizeof(a->names[ch]));
  a->cb.aUp[ch].sName = a->names[ch];
  a->cb.aUp[ch].pBuffer = a->up[ch];
  a->cb.aUp[ch].SizeOfBuffer = RTT_UP_BUFFER_SIZE;
  a->cb.aUp[ch].WrOff = 0U;
  a->cb.aUp[ch].RdOff = 0U;
  a->cb.aUp[ch].Flags = RTT_MODE_NO_BLOCK_SKIP;
  a->cb.aDown[ch].sName = a->names[ch];
  a->cb.aDown[ch].pBuffer = a->down[ch];
  a->cb.aDown[ch].SizeOfBuffer = RTT_DOWN_BUFFER_SIZE;
  a->cb.aDown[ch].WrOff = 0U;
  a->cb.aDown[ch].RdOff = 0U;
  a->cb.aDown[ch].Flags = RTT_MODE_NO_BLOCK_SKIP;

  /* Header comune, identico da ogni core. Come SEGGER: l'ID per ultimo (il
   * J-Link cerca l'ID). */
  a->cb.MaxNumUpBuffers = RTT_NUM_CHANNELS;
  a->cb.MaxNumDownBuffers = RTT_NUM_CHANNELS;
  __DMB();
  memcpy(&a->cb.acID[7], "RTT\0\0\0\0\0", 9);
  __DMB();
  memcpy(&a->cb.acID[0], "SEGGER ", 7);
  __DMB();
}

void rtt_write(const char* data, size_t n)
{
  volatile rtt_up_t* up = &AREA->cb.aUp[RTT_CHANNEL];
  char* buf = up->pBuffer;
  unsigned size = up->SizeOfBuffer;
  unsigned wr = up->WrOff;
  unsigned rd = up->RdOff;
  unsigned avail = (rd > wr) ? (rd - wr - 1U) : (size - (wr - rd) - 1U);

  if ((buf == NULL) || (n == 0U) || (n > avail)) {
    return; /* no-block-skip: il messaggio intero si perde */
  }
  size_t first = size - wr;
  if (first > n) {
    first = n;
  }
  memcpy(&buf[wr], data, first);
  memcpy(&buf[0], &data[first], n - first);
  wr += (unsigned)n;
  if (wr >= size) {
    wr -= size;
  }
  __DMB(); /* i dati prima del nuovo WrOff */
  up->WrOff = wr;
}

int rtt_printf(const char* fmt, ...)
{
  char line[256];
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(line, sizeof(line), fmt, ap);
  va_end(ap);
  if (n > 0) {
    rtt_write(line, (n < (int)sizeof(line)) ? (size_t)n : sizeof(line) - 1U);
  }
  return n;
}

int rtt_getkey(void)
{
  volatile rtt_down_t* down = &AREA->cb.aDown[RTT_CHANNEL];
  unsigned rd = down->RdOff;

  if ((down->pBuffer == NULL) || (rd == down->WrOff)) {
    return -1;
  }
  int c = (unsigned char)down->pBuffer[rd];
  rd++;
  if (rd >= down->SizeOfBuffer) {
    rd = 0U;
  }
  __DMB();
  down->RdOff = rd;
  return c;
}
