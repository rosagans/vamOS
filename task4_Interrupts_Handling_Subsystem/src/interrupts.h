#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;


#pragma pack(push, 1)

typedef struct {
    u16     offset_15_00        : 16;
    u16     segment_selector    : 16;
    u8      reserved            :  5;
    u8      zero3b              :  3;
    u8      type                :  3;
    u8      D                   :  1;
    u8      zero1b              :  1;
    u8      DPL                 :  2;
    u8      P                   :  1;
    u16     offset_31_16        : 16;
} interrupt_descriptor;

typedef struct {
    u32     edi;
    u32     esi;
    u32     ebp;
    u32     esp;
    u32     ebx;
    u32     edx;
    u32     ecx;
    u32     eax;

    u32     gs;
    u32     fs;
    u32     es;
    u32     ds;

    u32     vector;

    u32     error_code;
    u32     eip;
    u32     cs;
    u32     eflags;
} context;

typedef struct {
    u8      raw_data[17];
} tramplin;

typedef struct {
    u16     limit;
    u32     addr;
} idtr_reg;

#pragma pack(pop)

void universal_handler(context* ctx);
void init_interrupts();


#endif