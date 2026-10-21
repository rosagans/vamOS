#include "allocator.h"
#include "memory.h"
#include <stdint.h>
#include "panic.h"


#ifndef KERNEL_SIZE
    #error "KERNEL_SIZE must be defined by the build system!"
#endif


#define ARENA_START (0x00007C00 + KERNEL_SIZE)
#define ARENA_MAX   0x00080000


static uintptr_t current_heap = ARENA_START;


void* malloc_undead(size_t size, size_t alignment) {\
    // выравнивание работает только, когда alignment - степень двойки
    // uintptr_t aligned_addr = (current_heap + (alignment - 1)) & ~(alignment - 1);
    uintptr_t aligned_addr = (current_heap - 1) + alignment - ((current_heap - 1) % alignment);

    if (aligned_addr + size > ARENA_MAX) {
        kernel_panic("Undead allocator is out of memory: attempted to allocate %d bytes, exceed 0x80000 limit", size);
    }

    current_heap = aligned_addr + size;
    return (void*) aligned_addr;
}

void* calloc_undead(size_t size, size_t alignment) {
    void* ptr = malloc_undead(size, alignment);
    memzero(ptr, size);
    return ptr;
}