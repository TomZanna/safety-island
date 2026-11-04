/*
 * linker.ld
 *
 * Placeholder memory map for the bare metal target build.  TODO(target):
 * replace with the real i.MX943 memory map, MPU regions, reserved SHMEM
 * carve-out and stack/heap sizes.
 */

ENTRY (_start)

MEMORY
{
   FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 1M
   RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
}

SECTIONS
{
   .text :
   {
      *(.text*)
      *(.rodata*)
   } > FLASH

   .data :
   {
      *(.data*)
   } > RAM

   .bss (NOLOAD) :
   {
      *(.bss*)
      *(COMMON)
   } > RAM

   _estack = ORIGIN (RAM) + LENGTH (RAM);
}
