#ifndef MEMORY_ALLOCATOR_TEST1_H
#define MEMORY_ALLOCATOR_TEST1_H
#include "../../mem.h"
#include "./tests_common.h"

#define TEST_1_NUMBER 1
#define TEST_1_QUERY_SIZE 1024
#define TEST_1_DESCRIPTION "Usual memory allocation"

#define TEST_1_MALLOC_FAILURE "Test 1 failed. Memory haven't been allocated."
#define TEST_1_MALLOC_SUCCESS "Test 1 completed. Memory allocated."

void test1(FILE* out);

#endif //MEMORY_ALLOCATOR_TEST1_H
