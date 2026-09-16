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
    mov si, [N_val]
    mov ah, 0x2
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    xor bx, bx


print_loop:
    mov ah, 0x2
    int 0x13
    jc err_1


update_es:
    add di, 32              ; 32 * 16 = 512
    mov es, di


update_chs:
    sub si, 512
    cmp si, 0
    jbe done

    cmp cl, 18
    jne update_chs.inc_sector
    mov cl, 1

    test dh, dh
    jz update_chs.inc_header
    xor dh, dh

    cmp ch, 79
    je err_2
    inc ch
    jmp print_loop

    .inc_header:
        inc dh
        jmp print_loop

    .inc_sector:
        inc cl
        jmp print_loop






err_1:
    mov bx, msg_1
    .loop:
        mov al, byte [bx]
        cmp al, 0
        je done
        mov ah, 0x0E
        int 0x10
        inc bx
        jmp err_1.loop



err_2:
    mov bx, msg_2
    .loop:
        mov al, byte [bx]
        cmp al, 0
        je done
        mov ah, 0x0E
        int 0x10
        inc bx
        jmp err_2.loop



done:
    jmp done


msg_1: db "Carry flag is set", 0x0A, 0x0D, 0x0

msg_2: db "Max cylinder is 79. Cant access 80", 0x0A, 0x0D, 0x0

%ifdef N
    N_val: dw N
%else
    N_val: dw 512   
%endif


times 510-($-$$) db 0
dw 0xAA55