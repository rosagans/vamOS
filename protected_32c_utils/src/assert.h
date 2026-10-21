#ifndef ASSERT_H
#define ASSERT_H

#include "panic.h"


    #ifdef DEBUG
        #define my_assert(cond)                                                                                 \
        if (!(cond)) {                                                                                          \
            kernel_panic("Kernel panic: assertion failed: %s at (%s, line: %d)\n", #cond, __FILE__, __LINE__)   \
        }
    #else
        #define my_assert(cond)
    #endif

#endif