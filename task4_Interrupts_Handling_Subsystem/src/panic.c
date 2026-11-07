#include "panic.h"
#include "print.h"

extern void done();
extern void cli();


void vkernel_panic(const char* fmt, va_list args) {
    cli();
    vprintf(fmt, args);
    done();
}

void kernel_panic(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
    va_end(args);
}