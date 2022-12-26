#include "tests/includes/test3.h"

void test3(FILE* out) {
    fprintf(out, "=========Test%" PRIu32 "=========\n", TEST_3_NUMBER);
    fprintf(out, "%s\n", TEST_3_DESCRIPTION);

    void* heap = heap_init(HEAP_INITIAL_SIZE);
    if (!heap) {
        fprintf(out, "%s\n", TEST_HEAP_INIT_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_HEAP_INIT_SUCCESS);
        debug_heap(out, heap);
    }

    void* queried_memory_1 = _malloc(TEST_3_QUERY_SIZE_1);
    void* queried_memory_2 = _malloc(TEST_3_QUERY_SIZE_3);
    void* queried_memory_3 = _malloc(TEST_3_QUERY_SIZE_3);
    void* queried_memory_4 = _malloc(TEST_3_QUERY_SIZE_3);

    if (!queried_memory_1 || !queried_memory_2 || !queried_memory_3 || !queried_memory_4) {
        fprintf(out, "%s\n", TEST_3_MALLOC_FAILURE);
        return;
    }
    else {
        fprintf(out, "%s\n", TEST_3_MALLOC_SUCCESS);
    }
    debug_heap(out, heap);

    _free(queried_memory_1);
    fprintf(out, "%s\n", TEST_3_FREE_1);
    debug_heap(out, heap);

    _free(queried_memory_3);
    fprintf(out, "%s\n", TEST_3_FREE_2);
    debug_heap(out, heap);

    unmap_pages(heap, HEAP_INITIAL_SIZE);
    fprintf(out, "%s\n", TEST_HEAP_UNMAP);
    debug_heap(out, heap);
}
