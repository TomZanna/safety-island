/*
 * startup.c
 *
 * Minimal Cortex-M startup.  TODO(target): vector table, .data copy, .bss
 * clear, FPU setup, stack limits and the real reset handler.  This file only
 * makes the link step of the target build meaningful.
 */

extern int main (void);

void _start (void)
{
   (void)main ();

   for (;;)
   {
   }
}

void Default_Handler (void)
{
   for (;;)
   {
   }
}
