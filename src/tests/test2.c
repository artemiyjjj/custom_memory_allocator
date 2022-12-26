#include "./includes/test2.h"

void test2(FILE* out) {
    fprintf(out, "=========Test%" PRIu32 "=========\n", TEST_2_NUMBER);
    fprintf(out, "%s\n", TEST_2_DESCRIPTION);

    void* heap = heap_init(HEAP_INITIAL_SIZE);
    if (!heap) {
        fprintf(out, "%s\n", TEST_HEAP_INIT_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_HEAP_INIT_SUCCESS);
        debug_heap(out, heap);
    }

    void* queried_memory_1 = _malloc(TEST_2_QUERY_SIZE_1);
    void* queried_memory_2 = _malloc(TEST_2_QUERY_SIZE_2);

    if (!queried_memory_1 || !queried_memory_2) {
        fprintf(out, "%s\n", TEST_2_MALLOC_FAILURE);
        return;
    }
    else {
        fprintf(out, "%s\n", TEST_2_MALLOC_SUCCESS);
    }
    debug_heap(out, heap);

    _free(queried_memory_1);
    fprintf(out, "%s\n", TEST_2_FREE);
    debug_heap(out, heap);

    unmap_pages(heap, HEAP_INITIAL_SIZE);
    fprintf(out, "%s\n", TEST_HEAP_UNMAP);
    debug_heap(out, heap);
}
