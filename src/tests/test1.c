#include "tests/includes/test1.h"


void test1(FILE* out) {
    fprintf(out, "=========Test%" PRIu32 "=========\n", TEST_1_NUMBER);
    fprintf(out, "%s\n", TEST_1_DESCRIPTION);

    // heap
    void* heap = heap_init(HEAP_INITIAL_SIZE);
    if (!heap) {
        fprintf(out, "%s\n", TEST_HEAP_INIT_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_HEAP_INIT_SUCCESS);
        debug_heap(stdout, heap);
    }


    // malloc
    void* queried_memory = _malloc(TEST_1_QUERY_SIZE);
    if (!queried_memory) {
        fprintf(out, "%s\n", TEST_1_MALLOC_FAILURE);
        return ;
    }
    else {
        fprintf(out, "%s\n", TEST_1_MALLOC_SUCCESS);
        debug_heap(stdout, heap);
    }

    _free(queried_memory);

    unmap_pages(heap, HEAP_INITIAL_SIZE);

    fprintf(out, "%s", TEST_HEAP_UNMAP);
    debug_heap(stdout, heap);
}
