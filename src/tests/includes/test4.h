#ifndef MEMORY_ALLOCATOR_TEST4_H
#define MEMORY_ALLOCATOR_TEST4_H

#include "../../mem.h"
#include "./tests_common.h"

#define TEST_4_NUMBER 4
#define TEST_4_QUERY_SIZE_1 8192
#define TEST_4_QUERY_SIZE_2 1024
#define TEST_4_DESCRIPTION "Mapped memory is not enough. Old region has to be extended."

#define TEST_4_MALLOC_FAILURE "Test 4 failed. Memory haven't been allocated."
#define TEST_4_MALLOC_SUCCESS "Memory have been allocated. Heap should be bigger now."
#define TEST_4_MALLOC_FOR_EXTEND "One block now should be as big as heap. Program has to grow heap."

void test4(FILE* out);
#endif //MEMORY_ALLOCATOR_TEST4_H
