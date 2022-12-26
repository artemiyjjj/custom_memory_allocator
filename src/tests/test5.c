#include "./includes/test5.h"

void test5(FILE* out) {
    fprintf(out, "=========Test%" PRIu32 "=========\n", TEST_5_NUMBER);
    fprintf(out, "%s\n", TEST_5_DESCRIPTION);

    void* heap_1 = heap_init(HEAP_INITIAL_SIZE);
    if (!heap_1) {
        fprintf(out, "%s\n", TEST_HEAP_INIT_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_HEAP_INIT_SUCCESS);
        debug_heap(out, heap_1);
    }
    void* heap_2 = heap_init(HEAP_INITIAL_SIZE);
    if (!heap_2) {
        fprintf(out, "%s\n", TEST_HEAP_INIT_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_HEAP_INIT_SUCCESS);
        debug_heap(out, heap_2);
    }

    void* queried_memory_1 = _malloc(TEST_5_QUERY_SIZE_1);
    if (!queried_memory_1) {
        fprintf(out, "%s\n", TEST_5_MALLOC_FAILURE);
        return;
    }
    else {
        fprintf(out, "%s\n", TEST_5_MALLOC_FOR_EXTEND);
    }
    fprintf(out, TEST_5_HEAP_1);
    debug_heap(out, heap_1);

    fprintf(out, TEST_5_HEAP_2);
    debug_heap(out, heap_2);

    void* queried_memory_2 = _malloc(TEST_5_QUERY_SIZE_2);
    if (!queried_memory_2) {
        fprintf(out, "%s\n", TEST_5_MALLOC_FAILURE);
        return;
    }
    else {
        fprintf(out, "%s\n", TEST_5_MALLOC_SUCCESS);
    }

    fprintf(out, TEST_5_HEAP_1);
    debug_heap(out, heap_1);

    fprintf(out, TEST_5_HEAP_2);
    debug_heap(out, heap_2);

    unmap_pages(heap_1, HEAP_INITIAL_SIZE);
    unmap_pages(heap_2, HEAP_INITIAL_SIZE);
    fprintf(out, "%s\n", TEST_HEAP_UNMAP);

    fprintf(out, TEST_5_HEAP_1);
    debug_heap(out, heap_1);

    fprintf(out, TEST_5_HEAP_2);
    debug_heap(out, heap_2);
}

