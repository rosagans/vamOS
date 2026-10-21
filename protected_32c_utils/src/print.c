#include "vga.h"
#include "print.h"


#define VGA_WIDTH 80
#define VGA_HEIGHT 25


extern cursor_t CURSOR;

static void print_hex(unsigned int value);
static void print_dec(int value);


void init_printer() {
    vga_clear_screen();
    CURSOR.x = 1;
    CURSOR.y = 1;
}

static void putchar(char c) {
    if (c == '\n') {
        CURSOR.x = 0;
        CURSOR.y++;
    } else if (c == '\r') {
        CURSOR.x = 0;
    } else {
        vga_print_char(c, CURSOR.x, CURSOR.y);
        CURSOR.x++;
    }

    if (CURSOR.x >= VGA_WIDTH) {
        CURSOR.x = 0;
        CURSOR.y++;
    }
    
    if (CURSOR.y >= VGA_HEIGHT) {
        vga_scroll_down();
        CURSOR.y = VGA_HEIGHT - 1;
    }
}


void vprintf(const char* fmt, va_list args) {
    for (size_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] != '%') {
            putchar(fmt[i]);
            continue;
        }

        i++;
        switch (fmt[i]) {
            case 'c': {
                char c = (char) va_arg(args, int);      // char is promoted to int in ...
                putchar(c);
                break;
            }
            case 's': {
                char* str = (char*) va_arg(args, char*);
                while (*str) {
                    putchar(*str++);
                }
                break;
            }
            case 'd': {
                int val = va_arg(args, int);
                print_dec(val);
                break;
            }
            case 'x': {
                unsigned int val = va_arg(args, unsigned int);
                print_hex(val);
                break;
            }
            default: {
                putchar('%');
                putchar(fmt[i]);
                break;
            }
        }
    }
}


static void print_dec(int value) {
    if (value == 0) {
        putchar('0');
        return;
    }

    char buffer[10];            // 10 цифр хватит на int
    unsigned int uvalue;
    int i = 0;

    if (value < 0) {
        putchar('-');
        uvalue = (unsigned int) (-value);
    } else {
        uvalue = (unsigned int) (value);
    }
    while (uvalue > 0) {
        buffer[i++] = (char) ('0' + (uvalue % 10));
        uvalue /= 10;
    }
    while (i > 0) {
        putchar(buffer[--i]);
    }
}


static void print_hex(unsigned int value) {
    if (value == 0) {
        putchar('0');
        return;
    }

    char buffer[8];         // 8 цифр должно хватить
    int i = 0;
    char hex_digits[] = "0123456789abcdef";

    while (value > 0) {
        buffer[i++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    while (i > 0) {
        putchar(buffer[--i]);
    }
}


void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}