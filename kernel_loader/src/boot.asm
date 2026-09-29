; nasm -fbin boot.asm -o boot.bin (компиляция в сырой бинарник, чтобы процессор нас понял)
;
; dd if=/dev/zero of=boot.img bs=1024 count=1440 (используя dd собираем chs устройства,
;                                                 в нашем случае floppy disk и заполняем его нулями,
;                                                 размером 1440 КБ)
;
; dd if=boot.bin of=boot.img conv=notrunc (Копируем boot.bin - сырой бинарник - в первый сектор этого файла)
;
; qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA (запуск эмулятора)



[BITS 16]


init_stack:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00


init_data:
    mov ax, 0x7c0
    mov ds, ax
    mov di, 0x7e0
    mov es, di
    

prep:
    mov esi, N
    xor ch, ch
    mov cl, 2
    xor dh, dh
    xor bx, bx


print_loop:
    mov ax, 0x0201
    int 0x13
    jc err_1


update_es:
    add di, 32              ; 32 * 16 = 512
    mov es, di


update_chs:
    sub esi, 512
    cmp esi, 0
    jle done

    inc cl
    cmp cl, 19
    jne print_loop

    mov cl, 1
    xor dh, 1
    jnz print_loop

    inc ch
    cmp ch, 80
    je err_2
    jmp print_loop





err_1:
    mov bx, msg_1
    jmp print_err



err_2:
    mov bx, msg_2


print_err:
    mov al, byte [bx]
    test al, al
    je done
    mov ah, 0x0E
    xor bh, bh
    int 0x10
    inc bx
    jmp print_err


done:
    jmp done


msg_1: db "Carry flag is set", 0x0A, 0x0D, 0x0

msg_2: db "Max cylinder is 79. Cant access 80", 0x0A, 0x0D, 0x0



times 510-($-$$) db 0
dw 0xAA55