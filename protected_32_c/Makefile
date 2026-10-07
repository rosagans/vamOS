#all:
#	nasm -fbin -dN=0x201 boot.asm -o boot.bin
#	dd if=/dev/zero of=boot.img bs=1024 count=1440
#	dd if=boot.bin of=boot.img conv=notrunc
#	dd if=foo of=boot.img conv=notrunc seek=1
#	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA


# =============================================================================
# Variables

# Build tools
NASM = nasm -f bin 


# =============================================================================
# Tasks

all: clean build test

.tmp/os.bin: .tmp/kernel.o .tmp/boot.o
	ld .tmp/boot.o .tmp/kernel.o -T link.ld -m elf_i386 -o .tmp/os.elf
	objcopy -I elf32-i386 -O binary .tmp/os.elf .tmp/os.bin

.tmp/boot.o: src/boot.asm
	nasm -felf -dN=0x1000 src/boot.asm -o .tmp/boot.o


.tmp/kernel.o:
	gcc -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector -c src/kernel.c -o .tmp/kernel.o


boot.img: .tmp/os.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/os.bin of=boot.img conv=notrunc


build: boot.img


clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp


test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA


debug: clean build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -s -S &
	gdb -x .gdbinit


.PHONY: all build clean test debug