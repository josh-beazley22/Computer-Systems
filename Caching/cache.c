#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "cache.h"


void cache_init(cache_t* cache, int size, store_t* store, policy_t policy) {
    cache->PC = 0;
    cache->filled = 0;
    cache->store = store;
    cache->policy = policy;
    cache->size = size;
    cache->entries = calloc(size, sizeof(entry_t));
}

void cache_free(cache_t* cache) {
    cache->store = NULL;
    cache->size = 0;
    free(cache->entries);
}

unsigned char cache_get(cache_t* cache, int memory_location, int* latency) {
    cache->PC += 1;
    int pageno = memory_location / PAGESIZE;
    int offset = memory_location % PAGESIZE;

    if (cache_contains(cache, pageno)) {
        // No reason to change the cache at all.
        *latency = CACHE_LATENCY;
        for (int i = 0; i < cache->size; i++) {
            page_t* page = cache->entries[i].page;
            if (page != NULL && page->pageno == pageno) {
                // For some policies, the usage will change when I access it
                if (cache->policy == MRU || cache->policy == LRU) {
                    cache->entries[i].usage = cache->PC;
                }
                return memory_location;
                // return page->bytes[offset];
            }
        }
    } else {
        // I must edit the cache according to policy
        *latency = STORE_LATENCY;

        unsigned char bytes;
        page_t* cool_page = NULL;

        // Finding bytes from store memory
        for (int i = 0; i < cache->store->numpages; i++) {
            page_t* page = store_get(cache->store, i * PAGESIZE);
            if (page && page->pageno == pageno) {
                bytes = page->bytes[offset];
                cool_page = page;
                break;
            }
        }
        if (cool_page == NULL) {
            printf("Error: Page number %d not found in store.\n", pageno);
            exit(1);
        }
        // If cache has empty space, append.
        if (cache->filled < cache->size) {
            cache->entries[cache->filled].page = cool_page;
            cache->entries[cache->filled].usage = cache->PC;
            cache->filled += 1;
            return memory_location;
        } else {
            // Use policy to remove an element of the cache.
            if (cache->policy == FIFO || cache->policy == LRU) {
                // Search for cache page with lowest usage value
                int min = INT_MAX;
                entry_t* removal_entry = NULL;
                for (int i = 0; i < cache->size; i++) {
                    if (cache->entries[i].usage < min) {
                        min = cache->entries[i].usage;
                        removal_entry = &cache->entries[i];
                    }
                }
                if (removal_entry == NULL) {
                    fprintf(stderr, "No page found for MRU eviction\n");
                    exit(1);
                }
                removal_entry->page = cool_page;
                removal_entry->usage = cache->PC;
                return memory_location;

            } else if (cache->policy == MRU) {
                // Search for cache page with highest usage value
                int max = INT_MIN;
                entry_t* removal_entry = NULL;
                for (int i = 0; i < cache->size; i++) {
                    if (cache->entries[i].usage > max) {
                        max = cache->entries[i].usage;
                        removal_entry = &cache->entries[i];
                    }
                }
                if (removal_entry == NULL) {
                    fprintf(stderr, "No page found for MRU eviction\n");
                    exit(1);
                }
                removal_entry->page = cool_page;
                removal_entry->usage = cache->PC;
                return memory_location;

            } else if (cache->policy == RANDOM) {
                int r = rand() % cache->size;
                cache->entries[r].page = cool_page;
                cache->entries[r].usage = cache->PC;
                return memory_location;

            } else {
                printf("Error! Invalid policy type.\n");
                exit(1);
            }
        }
    }
}

bool cache_contains(cache_t* cache, int page_no) {
    // Iterate through every page in the cache to check if
    // page_no exists in the cache.
    for (int i = 0; i < cache->size; i++) {
        page_t* page = cache->entries[i].page;
        if (page != NULL && page->pageno == page_no) {
            return true;
        }
    }
    return false;
}
