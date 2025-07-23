#ifndef STORE_H_
#define STORE_H_

#include "page.h"

#define STORE_LATENCY 10

// A store is a set of pages
typedef struct {
    int numpages;
    page_t* pages;
} store_t;

// Initialize a memory store with the given number of pages
void store_init(store_t* store, int numpages);

// Free the memory allocated for the store
void store_free(store_t* store);

// Return the page corresponding to the given memory location
page_t* store_get(store_t* store, int location);

#endif  // STORE_H_
