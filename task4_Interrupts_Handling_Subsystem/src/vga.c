#include "memory.h"
#include "vga.h"
#include "assert.h"

#define START_ADDRESS 0xb8000
#define SCREEN_SIZE 4000
#define ROWS 25


symbol_t create_symbol(unsigned char c, unsigned char front_color, unsigned char back_color) {
    symbol_t symbol;
    symbol.c = c;
    symbol.front_color = front_color;
    symbol.back_color = back_color;
    return symbol;
}


void vga_clear_screen() {
    memzero((char*) START_ADDRESS, SCREEN_SIZE);
}


void vga_print_char(unsigned char c, size_t x, size_t y) {
    assert(x >= 0 && x <= 80 && y >= 0 && y <= 25)
    symbol_t* address = (symbol_t*) START_ADDRESS;
    int index = y * 80 + x;
    address[index] = create_symbol(c, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}


void vga_scroll_down() {
    int row_size = SCREEN_SIZE / ROWS;
    memmove((void*) START_ADDRESS,(void*) (START_ADDRESS + row_size), SCREEN_SIZE - row_size);
    memzero((void*) (START_ADDRESS + 24*row_size), row_size);
}