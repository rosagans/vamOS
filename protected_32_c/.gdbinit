target remote localhost:1234
set architecture i8086
set disassembly-flavor intel
set architecture i8086

# При КАЖДОЙ остановке QEMU принудительно возвращаем 16 бит
define hook-stop
    set architecture i8086
end

layout asm
layout regs
b *0x7c00
b *0x7c40
c
