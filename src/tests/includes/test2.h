#ifndef MEMORY_ALLOCATOR_TEST2_H
#define MEMORY_ALLOCATOR_TEST2_H

#include "mem.h"
#include "tests_common.h"

#define TEST_2_NUMBER 2
#define TEST_2_QUERY_SIZE_1 1024
#define TEST_2_QUERY_SIZE_2 1024
#define TEST_2_DESCRIPTION "Free one block of few existing"

#define TEST_2_MALLOC_FAILURE "Test 2 failed. Memory haven't been allocated."
#define TEST_2_MALLOC_SUCCESS "Memory have been allocated."
#define TEST_2_FREE "After freeing one block"

void test2(FILE* out);

#endif //MEMORY_ALLOCATOR_TEST2_H
