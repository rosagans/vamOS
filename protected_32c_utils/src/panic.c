#include "panic.h"
#include "print.h"

void done();
void cli();


void vkernel_panic(const char* fmt, va_list args) {
    vprintf(fmt, args);
    cli();
    done();
}

void kernel_panic(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
    va_end(args);
}