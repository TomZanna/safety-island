/*
 * abi_layout.c
 *
 * Prints the shared mailbox geometry.  tools/abi_check.sh compares it with
 * the expected values and, when available, repeats the check with -m32 to
 * prove that the packed structures have the same layout on 32-bit safety
 * cores and on the 64-bit A55.
 */

#include "flow_shm_api.h"

#include <stddef.h>
#include <stdio.h>

int main (void)
{
   printf ("header=%u slot=%u channel=%u shm=%u tx=%u rx=%u\n",
      (unsigned)sizeof (flow_shm_header_t),
      (unsigned)sizeof (flow_shm_slot_t),
      (unsigned)sizeof (flow_shm_channel_t),
      (unsigned)sizeof (flow_shm_t),
      (unsigned)offsetof (flow_shm_t, tx),
      (unsigned)offsetof (flow_shm_t, rx));

   return 0;
}
