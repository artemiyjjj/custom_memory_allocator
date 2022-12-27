#ifndef MEMORY_ALLOCATOR_TEST5_H
#define MEMORY_ALLOCATOR_TEST5_H

#include "../../mem.h"
#include "./tests_common.h"

#define TEST_5_NUMBER 5
#define TEST_5_QUERY_SIZE_1 8192
#define TEST_5_QUERY_SIZE_2 1024
#define TEST_5_DESCRIPTION "Mapped memory is not enough. Old region has to be extended, but it can't grow right after itself because of different mapped addresses."

#define TEST_5_MALLOC_FAILURE "Test 5 failed. Memory haven't been allocated."
#define TEST_5_MALLOC_SUCCESS "Memory have been allocated. Heap should be bigger now."
#define TEST_5_MALLOC_FOR_EXTEND "The block now should be as big as heap. Program has to grow heap."
#define TEST_5_HEAP_1 "Heap 1"
#define TEST_5_HEAP_2 "Heap 2"

void test5(FILE* out);

#endif //MEMORY_ALLOCATOR_TEST5_H
