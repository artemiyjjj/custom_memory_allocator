#ifndef MEMORY_ALLOCATOR_TEXT3_H
#define MEMORY_ALLOCATOR_TEXT3_H

#include "../../mem.h"
#include "./tests_common.h"

#define TEST_3_NUMBER 3
#define TEST_3_QUERY_SIZE_1 1024
#define TEST_3_QUERY_SIZE_3 1024
#define TEST_3_DESCRIPTION "Free two blocks of few existing"

#define TEST_3_MALLOC_FAILURE "Test 3 failed. Memory haven't been allocated."
#define TEST_3_MALLOC_SUCCESS "Memory have been allocated."
#define TEST_3_FREE_1 "After freeing one block"
#define TEST_3_FREE_2 "After freeing another block"


void test3(FILE* out);

#endif //MEMORY_ALLOCATOR_TEXT3_H
