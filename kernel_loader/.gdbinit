target remote localhost:1234
set architecture i8086
set disassembly-flavor intel
layout asm
layout regs
b *0x7c00
c
define skipint
  tb *($pc + 2)
  c
end