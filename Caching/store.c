#include <stdio.h>
#include <stdlib.h>
#include "store.h"

// Initialize a memory store with the given number of pages
void store_init(store_t* store, int numpages) {
    store->numpages = numpages;
    store->pages = malloc(numpages * sizeof(page_t));
    if (store->pages == NULL) {
        // Memory declaration failure.
        printf("Error declaring memory.");
        return;
    }

    for (int i=0; i < numpages; i++) {
        page_init(&store->pages[i], i, 0);
    }
}

// Free memory allocated for the store
void store_free(store_t* store) {
    store->pages = NULL;
    store->numpages = 0;
    for (int i = 0; i < store->numpages; i++) {
        free(&store->pages[i]);
    }
    free(store);
}

// Return the page corresponding to the given memory location
page_t* store_get(store_t* store, int location) {
    int pageno = location / PAGESIZE;
    return &store->pages[pageno];
}
