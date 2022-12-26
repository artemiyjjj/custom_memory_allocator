#define _DEFAULT_SOURCE

#include <assert.h>
#include "mem.h"
#include "mem_internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

void debug_block(struct block_header* b, const char* fmt, ... );
void debug(const char* fmt, ... );

extern inline block_size size_from_capacity( block_capacity cap );
extern inline block_capacity capacity_from_size( block_size sz );

static bool block_is_big_enough( size_t query, struct block_header* block ) { return block->capacity.bytes >= query; }
static size_t pages_count ( size_t mem ) { return mem / getpagesize() + ((mem % getpagesize()) > 0); }
static size_t round_pages ( size_t mem ) { return getpagesize() * pages_count( mem ) ; }

static void block_init( void* restrict addr, block_size block_sz, void* restrict next ) {
  *((struct block_header*)addr) = (struct block_header) {
    .next = next,
    .capacity = capacity_from_size(block_sz),
    .is_free = true
  };
}

static size_t region_actual_size( size_t query ) { return size_max( round_pages( query ), REGION_MIN_SIZE ); }

extern inline bool region_is_invalid( const struct region* r );


static void* map_pages(void const* addr, size_t length, int additional_flags) {
  return mmap( (void*) addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | additional_flags , -1, 0 );
}

/*  аллоцировать регион памяти и инициализировать его блоком */
static struct region alloc_region( void const * addr, size_t query ) {
    // counting region size
    block_capacity region_expected_capacity = {query};
    size_t region_size = region_actual_size(size_from_capacity(region_expected_capacity).bytes);
    // trying to map region EXACTLY starting on provided address
    void* region_addr_from_mmap = map_pages(addr, region_size, MAP_FIXED_NOREPLACE);
    // if failure, then trying to map region somewhere "near" the address
    if (region_addr_from_mmap == MAP_FAILED) {
        region_addr_from_mmap = map_pages(addr, region_size, 0);
        // if this fails too, return uninitialized region
        if (region_addr_from_mmap == MAP_FAILED) { return REGION_INVALID; }
    }

    // region configuring
    struct region region = { .addr = region_addr_from_mmap, .size = region_size, .extends = addr == region_addr_from_mmap };
    // initializing blocks
    block_size initial_block_size = { region_size };
    block_init(region_addr_from_mmap, initial_block_size, NULL);

    return region;
}

static void* block_after( struct block_header const* block );

void* heap_init( size_t initial ) {
  const struct region region = alloc_region( HEAP_START, initial );
  if ( region_is_invalid(&region) ) return NULL;

  return region.addr;
}

#define BLOCK_MIN_CAPACITY 24

typedef struct block_search_result block_search_result;

/*  --- Разделение блоков (если найденный свободный блок слишком большой )--- */

static bool block_splittable( struct block_header* restrict block, size_t query) {
  return block-> is_free && query + offsetof( struct block_header, contents ) + BLOCK_MIN_CAPACITY <= block->capacity.bytes;
}

static bool split_if_too_big( struct block_header* block, size_t query ) {
    if ((block == NULL) || block_splittable(block, query) == false) { return false; }

    // calculate new block's starting address and size
    void* new_block_address = block -> contents + query;
    block_size new_block_size = { block -> capacity.bytes - query};

    // initializing new block
    block_init(new_block_address, new_block_size, block -> next);
    // changing old block's values
    block -> capacity.bytes = query;
    block -> next = new_block_address;

    return true;

}


/*  --- Слияние соседних свободных блоков --- */

static void* block_after( struct block_header const* block ) {
  return  (void*) (block->contents + block->capacity.bytes);
}

static bool blocks_continuous ( struct block_header const* fst, struct block_header const* snd ) {
  return (void*)snd == block_after(fst);
}

static bool mergeable(struct block_header const* restrict fst, struct block_header const* restrict snd) {
  return fst->is_free && snd->is_free && blocks_continuous( fst, snd ) ;
}

static bool try_merge_with_next( struct block_header* block ) {
    // check if block exists and mergable
    if (block == NULL || block -> next == NULL || !mergeable(block, block -> next)) {
        return false;
    }
    // merging
    struct block_header* snd_block = block -> next;
    block_size snd_block_size = size_from_capacity(snd_block -> capacity);

    block -> next = snd_block -> next;
    block -> capacity.bytes += snd_block_size.bytes;

    return true;
}


/*  --- ... ecли размера кучи хватает --- */

struct block_search_result {
  enum {BSR_FOUND_GOOD_BLOCK, BSR_REACHED_END_NOT_FOUND, BSR_CORRUPTED} type;
  struct block_header* block;
};

// find block for query size
static struct block_search_result find_good_or_last  ( struct block_header* restrict block, size_t query ) {
    if (!block) {
        return (struct block_search_result) { .type = BSR_CORRUPTED, .block = block};
    }
    else {
        do {
            while (try_merge_with_next(block))
                continue;

            if (block->is_free && block_is_big_enough(query, block)) {
                return (struct block_search_result) {.type = BSR_FOUND_GOOD_BLOCK, .block = block};
            }
            if (!(block->next)) {
                return (struct block_search_result) {.type = BSR_REACHED_END_NOT_FOUND, .block = block};
            } else block = block->next;

        } while (block);
    }
//    fuck linter
    return (block_search_result) {0};
}

/*  Попробовать выделить память в куче начиная с блока `block` не пытаясь расширить кучу
 Можно переиспользовать как только кучу расширили. */
static struct block_search_result try_memalloc_existing ( size_t query, struct block_header* block ) {
    // try to find sufficient block
    struct block_search_result block_search = find_good_or_last(block, query);
    // if found, optimizing it and returning
    if (block_search.type == BSR_FOUND_GOOD_BLOCK) {
        split_if_too_big(block, query);
        block_search.block -> is_free = false;
        return block_search;
    }
    //otherwise pass result with the last found block or NULL value block
    return block_search;
}


// try to get more memory from mmap
static struct block_header* grow_heap( struct block_header* restrict last_block, size_t query ) {
    // validation
    if (!last_block) {
        return NULL;
    }
    // getting address where we want to allocate new region
    void* expected_address = block_after(last_block);

    // trying to allocate new region at that address
    struct region new_region = alloc_region(expected_address, query);

    // if allocation failed
    if (region_is_invalid(&new_region)) {
        return NULL;
    }
    else {
        // pushing new block to the linked list of blocks in heap
        last_block->next = new_region.addr;
        // try to extend
        if (new_region.extends && try_merge_with_next(last_block)) {
            return last_block;
        }
        else {
            return new_region.addr;
        }
    }
}

/*  Реализует основную логику malloc и возвращает заголовок выделенного блока */
static struct block_header* memalloc( size_t query, struct block_header* first_block) {
    if (!first_block) {
        return NULL;
    }
    struct block_search_result block_search = try_memalloc_existing(query, first_block);

    if (block_search.type == BSR_CORRUPTED) {
        return NULL;
    }

    else if (block_search.type == BSR_FOUND_GOOD_BLOCK) {
        return block_search.block;
    }

    else if (block_search.type == BSR_REACHED_END_NOT_FOUND){
        block_search.block = grow_heap(block_search.block, query);
        if (!block_search.block) {
            return NULL;
        }
        else {
            return try_memalloc_existing(query, block_search.block).block;
        }
    }
    return NULL;
}

void* _malloc( size_t query ) {
  struct block_header* const addr = memalloc( query, (struct block_header*) HEAP_START );
  if (addr) return addr->contents;
  else return NULL;
}

static struct block_header* block_get_header(void* contents) {
  return (struct block_header*) (((uint8_t*)contents)-offsetof(struct block_header, contents));
}

void _free( void* mem ) {
  if (!mem) return ;
  struct block_header* block = block_get_header( mem );
  block -> is_free = true;
  while (try_merge_with_next(block))
      continue;
}
