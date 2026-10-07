# target remote localhost:1234
# set architecture i8086
# set disassembly-flavor intel
# set architecture i8086

# # При КАЖДОЙ остановке QEMU принудительно возвращаем 16 бит
# define hook-stop
#     set architecture i8086
# end

# layout asm
# layout regs
# b *0x7c00
# b *0x7c40
# c

# 1. ЖЕСТКО отключаем запуск XML-опроса у QEMU
set remote tdesc-packet off

# 2. Выставляем 16-битный режим ДО подключения
set architecture i8086
set disassembly-flavor intel

# 3. Подключаемся к QEMU
target remote localhost:1234

# 4. Ставим брейкпоинты
b *0x7c00
b *0x7c40
c

# 5. Автоматически печатаем 5 инструкций на каждом шаге
display/5i $pc