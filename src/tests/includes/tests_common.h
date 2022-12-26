#ifndef MEMORY_ALLOCATOR_CONSTS_H
#define MEMORY_ALLOCATOR_CONSTS_H

#include <inttypes.h>
#include "mem.h"
#include "mem_internals.h"
#include <stdint.h>


#define HEAP_INITIAL_SIZE 8192

#define TEST_HEAP_INIT_FAILURE "Failed to initialize heap"
#define TEST_HEAP_INIT_SUCCESS "Successfully initialized heap"
#define TEST_HEAP_UNMAP "Successfully unmapped pages"

inline void unmap_pages(void* addr, size_t size) {
    munmap(addr, size_from_capacity((block_capacity){.bytes = size}).bytes);
}

#endif //MEMORY_ALLOCATOR_CONSTS_H
