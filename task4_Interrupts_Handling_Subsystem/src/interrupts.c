#include "interrupts.h"
#include "allocator.h"
#include "memory.h"
#include "panic.h"


static tramplin* generate_tramplins();
static void* generate_int_descriptors(tramplin* tramplins);
static int contains(int arr[], int size, int target);
static int generate_push_u8(char* address, u8 value);
static int generate_jmp(char* address, void(*func_ptr)(void));
extern void lidt(void* idtr);
extern void collect_context();


void universal_handler(context* ctx) {
    const char* panic_template = 
        "=== KERNEL PANIC ===\n"
        "unhandled interrupt #%d at 0x%x:0x%x\n"
        "\n"
        "Registers:\n"
        "   EAX: 0x%x\n"
        "   EBX: 0x%x\n"
        "   ECX: 0x%x\n"
        "   EDX: 0x%x\n"
        "   ESP: 0x%x\n"
        "   EBP: 0x%x\n"
        "   ESI: 0x%x\n"
        "   EDI: 0x%x\n"
        "   DS : 0x%x\n"
        "   ES : 0x%x\n"
        "   FS : 0x%x\n"
        "   GS : 0x%x\n"
        "\n"
        "Error code:\n"
        "   common_error_code, value: %x\n"
        "\n"
        "EFLAGS:\n"
        "   value: %x";

    kernel_panic(panic_template,    
        ctx->vector, ctx->cs, ctx->eip,                      
        ctx->eax, ctx->ebx, ctx->ecx, ctx->edx,
        ctx->esp, ctx->ebp, ctx->esi, ctx->edi,
        ctx->ds, ctx->es, ctx->fs, ctx->gs,
        ctx->error_code,
        ctx->eflags);
}


void init_interrupts() {
    tramplin* tramplins = generate_tramplins();
    void* idt = generate_int_descriptors(tramplins);
    idtr_reg* idtr = malloc_undead(6, 1);
    idtr->limit = 256 * 8 - 1;
    idtr->addr = (u32) idt;
    lidt(idtr);
}

static tramplin* generate_tramplins() {
    int has_error_code[8] = {8, 10, 11, 12, 13, 14, 17, 21};

    tramplin* tramplins = malloc_undead(17 * 256, 1);

    for (int i = 0; i < 256; i++) {
        u8* cur_ptr = (u8*) &tramplins[i];
        if (!contains(has_error_code, 8, i)) {
            cur_ptr += generate_push_u8(cur_ptr, 0);        // Пуш фейкового error code
        }  
        cur_ptr += generate_push_u8(cur_ptr, i);
        cur_ptr += generate_jmp(cur_ptr, &collect_context);
    }
    return tramplins;
}

static void* generate_int_descriptors(tramplin* tramplins) {
    interrupt_descriptor* idt = malloc_undead(8 * 256, 8);
    for (int i = 0; i < 256; i++) {
        u32 tramplin_addr = (u32) &tramplins[i];
        u16 tramplin_addr_high = (u16) (tramplin_addr >> 16);
        u16 tramplin_addr_low = (u16) tramplin_addr;

        interrupt_descriptor* cur_int_desc = &idt[i];

        cur_int_desc->offset_15_00           = tramplin_addr_low;
        cur_int_desc->segment_selector       = 0x8;
        cur_int_desc->reserved               = 0;
        cur_int_desc->zero3b                 = 0;
        cur_int_desc->type                   = 0b110;
        cur_int_desc->D                      = 1;
        cur_int_desc->zero1b                 = 0;
        cur_int_desc->DPL                    = 0;
        cur_int_desc->P                      = 1;
        cur_int_desc->offset_31_16           = tramplin_addr_high;
    }
    return (void*) idt;
}


static int contains(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return 1;
        }
    }
    return 0;
}


static int generate_push_u8(char* address, u8 value) {
    // 5-ти байтовый опкод для пуша unsigned char
    address[0] = 0x68;
    address[1] = value;
    address[2] = 0;
    address[3] = 0;
    address[4] = 0;
    return 5;
}

static int generate_jmp(char* address, void(*func_ptr)(void)) {
    unsigned int func_raw_addr = (unsigned int)func_ptr;
    unsigned int jmp_src_addr  = (unsigned int)address;
    unsigned int offset = func_raw_addr - (jmp_src_addr + 5);
    address[0] = 0xE9; // Опкод для относительного прыжка
    memmove(&address[1], &offset, 4);
    return 5;
}