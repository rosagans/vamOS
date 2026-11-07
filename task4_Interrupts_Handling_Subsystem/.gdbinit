set architecture i8086
set disassembly-flavor intel

file .tmp/os.elf

target remote localhost:1234

break init_interrupts
continue
layout src
