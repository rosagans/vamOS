#ifndef PRINT_H
#define PRINT_H
#include <stdarg.h>

void init_printer();
void vprintf(const char* fmt, va_list args);
void printf(const char* fmt, ...);


#endif