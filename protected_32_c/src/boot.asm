; nasm -fbin boot.asm -o boot.bin (компиляция в сырой бинарник, чтобы процессор нас понял)
;
; dd if=/dev/zero of=boot.img bs=1024 count=1440 (используя dd собираем chs устройства,
;                                                 в нашем случае floppy disk и заполняем его нулями,
;                                                 размером 1440 КБ)
;
; dd if=boot.bin of=boot.img conv=notrunc (Копируем boot.bin - сырой бинарник - в первый сектор этого файла)
;
; qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA (запуск эмулятора)


SECTORS_TO_READ equ (N + 511) / 512


[BITS 16]


init_stack:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00


init_data:
    mov ds, ax
    mov di, 0x7e0
    mov es, di
    

prep:
    mov si, SECTORS_TO_READ
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
    dec si
    cmp si, 0
    jle to_protected_32

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


to_protected_32:
    cld
    lgdt [gdt_descriptor]

    mov eax, CR0
    or eax, 1
    mov CR0, eax

    jmp 0x8:next


[BITS 32]
[EXTERN kernel_entry]

next:
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kernel_entry



err_1:
    mov si, msg_1
    jmp print_err



err_2:
    mov si, msg_2


print_err:
    mov al, byte [si]
    test al, al
    je done
    mov ah, 0x0E
    xor bh, bh
    int 0x10
    inc si
    jmp print_err

[GLOBAL done]
done:
    jmp done


msg_1: db "Carry flag is set", 0x0A, 0x0D, 0x0

msg_2: db "Max cylinder is 79. Cant access 80", 0x0A, 0x0D, 0x0


gdt_descriptor: 
    dw 0x17
    dd gdt
    
align 8

gdt:
    dq 0                    ; null segment descriptor
    .code_descriptor:           
        dw 0xFFFF           ; first 4 numbers of limit (right 4 numbers from 0xFFFFF)
        dw 0                ; first 2 numbers of base
        db 0                ; third number of base
        db 0b10011010       ; in order: P=1, DPL=00, S=1, Type=0b1010(R enabled)
        db 0b11001111       ; in order: G=1, D=1, fixed=0, AVL=0, high bit of limit = 0b1111
        db 0                ; high bit of base = 0
    
    .data_descriptor:
        dw 0xFFFF           ; least 4 numbers of limit (right 4 numbers from 0xFFFFF)
        dw 0                ; first 2 numbers of base
        db 0                ; third number of base
        db 0b10010010       ; in order: P=1, DPL=00, S=1, Type=0b0010(W enabled)
        db 0b11001111       ; in order: G=1, B=1, fixed=0, AVL=0, high bit of limit = 0b1111
        db 0                ; high bit of base = 0


times 510-($-$$) db 0
dw 0xAA55