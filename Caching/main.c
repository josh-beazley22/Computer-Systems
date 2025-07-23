#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

void print_usage() {
    printf("Usage: ./cache_sim [-s storesize] [-c cachesize] [-p policy]\n");
}


int main(int argc, char* argv[]) {
    // keep a counter of cache latency
    int total_latency = 0;

    // Default params
    int storesize = 16;
    int cachesize = 4;
    policy_t policy = FIFO;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 >= argc) {
                print_usage();
                return 0;
            }
            storesize = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 >= argc) {
                print_usage();
                return 0;
            }
            cachesize = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-p") == 0) {
            if (i + 1 >= argc) {
                print_usage();
                return 0;
            }
            const char* policy_str = argv[++i];
            // Convert string to enum
            if (strcmp(policy_str, "fifo") == 0) {
                policy = FIFO;
            } else if (strcmp(policy_str, "lru") == 0) {
                policy = LRU;
            } else if (strcmp(policy_str, "mru") == 0) {
                policy = MRU;
            } else if (strcmp(policy_str, "random") == 0) {
                policy = RANDOM;
            } else {
                print_usage();
                return 0;
            }
        } else {
            print_usage();
            return 0;
        }
    }

    // Debug print
    // printf("Store size: %d\n", storesize);
    // printf("Cache size: %d\n", cachesize);
    // printf("Policy: %s\n", policy_str);


    // Initialize store and cache with policy choice.
    store_t* store = malloc(sizeof(store_t));
    store_init(store, storesize);
    cache_t* cache = malloc(sizeof(cache_t));
    cache_init(cache, cachesize, store, policy);

    // read ints from stdin
    int value;
    int latency;
    int sum_of_latency = 0;
    while (scanf("%d", &value) == 1) {
        cache_get(cache, value, &latency);
        sum_of_latency += latency;
    }
    printf("Total latency=%d\n", sum_of_latency);

    // // Debug. Adds values into pages, then retrieves them.
    // for (int i = 0; i < storesize*PAGESIZE; i++) {
    //     if ((ch = getchar()) == EOF) break;
    //     unsigned char byte = (unsigned char) ch;
    //     page_set(&store->pages[i/PAGESIZE], i%PAGESIZE, byte);
    // }
    // page_t* page = store_get(store, 0);
    // unsigned char c = page_get(page, 0);
    // printf("%c\n", c);

    return 0;
}
