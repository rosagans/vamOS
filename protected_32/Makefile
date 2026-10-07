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

.tmp/boot.bin: src/boot.asm
	$(NASM) -dN=0x40000 src/boot.asm -o .tmp/boot.bin

boot.img: .tmp/boot.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/boot.bin of=boot.img conv=notrunc
	dd if=foo of=boot.img conv=notrunc seek=1

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