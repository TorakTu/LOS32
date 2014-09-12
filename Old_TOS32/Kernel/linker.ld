OUTPUT_FORMAT("binary")

SECTIONS
{
  .text 0x100000 :
  {
      code = .;
      _code = .;
      __code = .;
      *(.text)
      *(.rodata*) /* <---- ELF Cross Compiler or ELF *NIX (eg. Linux) */
      *(.rdata*)  /* <---- COFF/PE MinGW or Cygwin on Windows */
      . = ALIGN(4096);
  }
  .data :
  {
      data = .;
      _data = .;
      __data = .;
     *(.data)
     . = ALIGN(4096);
  }
  .bss :
  {
     bss = .;
     _bss = .;
     __bss = .;
     *(.bss)
     . = ALIGN(4096);
  }
  end = .;
  _end = .;
  __end = .;
}