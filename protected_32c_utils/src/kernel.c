#include "vga.h"
#include <stddef.h>
#include "assert.h"
#include <limits.h>
#include "allocator.h"
#include "print.h"


cursor_t CURSOR = (cursor_t) {
    .x = 0,
    .y = 0
};


void done();
void cli();

void kernel_entry() {

    vga_clear_screen();
    for (size_t i = 1; i < INT_MAX; i++) {
        void* ptr = malloc_undead(i, i);
        printf("%x\n", ptr);
    }
}