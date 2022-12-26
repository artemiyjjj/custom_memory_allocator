#include "./includes/test4.h"

void test4(FILE* out) {
    fprintf(out, "=========Test%" PRIu32 "=========\n", TEST_4_NUMBER);
    fprintf(out, "%s\n", TEST_4_DESCRIPTION);

    void* heap = heap_init(HEAP_INITIAL_SIZE);
    if (!heap) {
        fprintf(out, "%s\n", TEST_HEAP_INIT_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_HEAP_INIT_SUCCESS);
        debug_heap(out, heap);
    }

    void* queried_memory_1 = _malloc(TEST_4_QUERY_SIZE_1);
    if (!queried_memory_1) {
        fprintf(out, "%s\n", TEST_4_MALLOC_FAILURE);
        return;
    }
    else {
        fprintf(out, "%s\n", TEST_4_MALLOC_FOR_EXTEND);
    }
    debug_heap(out, heap);

    void* queried_memory_2 = _malloc(TEST_4_QUERY_SIZE_2);
    if (!queried_memory_2) {
        fprintf(out, "%s\n", TEST_4_MALLOC_FAILURE);
        return;
    }
    else {
        fprintf(out, "%s\n", TEST_4_MALLOC_SUCCESS);
    }
    debug_heap(out, heap);

    unmap_pages(heap, HEAP_INITIAL_SIZE);
    fprintf(out, "%s\n", TEST_HEAP_UNMAP);
    debug_heap(out, heap);
}
