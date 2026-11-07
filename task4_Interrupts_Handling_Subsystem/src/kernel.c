#include "vga.h"
#include <stddef.h>
#include "assert.h"
#include <limits.h>
#include "allocator.h"
#include "print.h"
#include "interrupts.h"


extern void done();
extern void cli();
extern void test_de();
extern void test_int();
extern void test_io();


void kernel_entry() {

    assert(sizeof(interrupt_descriptor) == 8)

    init_interrupts();

    vga_clear_screen();
    // test_int();
    // test_de();
    test_io();
}