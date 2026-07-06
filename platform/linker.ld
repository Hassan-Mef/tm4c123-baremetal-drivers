/*=============================================================================
 * FILENAME    : linker.ld
 * DESCRIPTION : Linker script for the TM4C123GH6PM microcontroller.
 * AUTHOR      : Hassan
 *===========================================================================*/

ENTRY(Reset_Handler)

_stack_top = ORIGIN(SRAM) + LENGTH(SRAM);

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}


SECTIONS
{
     /*-------------------------------------------------------------
     * Interrupt Vector Table
     *------------------------------------------------------------*/
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    /*-------------------------------------------------------------
     * Program Code and Read-Only Data
     *------------------------------------------------------------*/
    .text :
    {
        KEEP(*(.text.Reset_Handler))

        *(.text)
        *(.text*)

        *(.rodata)
        *(.rodata*)
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        _sdata = .;

        *(.data)
        *(.data*)

        _edata = .;
    } > SRAM AT > FLASH

    .bss :
    {
        _sbss = .;

        *(.bss)
        *(.bss*)

        *(COMMON)

        /* End of .bss */
        _ebss = .;

    } > SRAM
}