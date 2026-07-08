/* Define the end of stack, 0x20005000 in this case */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Define memory of the device */
MEMORY
{
  FLASH ( rx ) : ORIGIN = 0x00000000 , LENGTH = 256K
  RAM ( rxw ) : ORIGIN = 0x20000000 , LENGTH = 32K
}

/* Define different sections of memory and where to store them */
SECTIONS
{
  .text :
  {
    *(.isr_vector)
    *(.text)
    *(.rodata)
	. = ALIGN(4);
	_etext = .;
  } > FLASH

  /* Used by the startup file to initialize data */
  _sidata = LOADADDR(.data);
  .data :
  {
    _sdata = .;
	*(.data)
	. = ALIGN(4);
	_edata = .;
  } > RAM AT> FLASH

  .bss :
  {
    _sbss = .;
    *(.bss)
	*(COMMON)
	. = ALIGN(4);
	_ebss = .;
  } > RAM
}

