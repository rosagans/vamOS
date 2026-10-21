#include <stddef.h>


void* memmove(void* dst, const void* src, size_t n) {
    char* d = (char*) dst;
    const char* s = (const char*) src;

    if (s > d) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i-- > 0; ) {
            d[i] = s[i];
        }
    }
    return dst;
}

void* memzero(void* dst, size_t n) {
    char* d = (char*) dst;
    for (size_t i = 0; i < n; i++) {
        d[i] = 0;
    }
    return dst;
}