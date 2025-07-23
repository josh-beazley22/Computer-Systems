
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "VM.h"

int* make_random_array16() {
    int* elems = malloc(16 * sizeof(int));
    if (elems == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    // Fill with 0 to 15
    for (int i = 0; i < 16; i++) {
        elems[i] = i;
    }

    // Shuffle using Fisher–Yates
    for (int i = 15; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = elems[i];
        elems[i] = elems[j];
        elems[j] = temp;
    }

    return elems;
}

int load_page_table(PageTable* ptab, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }

    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        int temp;
        if (fscanf(fp, "%d", &temp) != 1 || temp < 0 || temp > 15) {
            fprintf(stderr, "Invalid or missing physical page number at index %d\n", i);
            fclose(fp);
            return 1;
        }
        ptab->entries[i].valid = 1;
        ptab->entries[i].physical_page_no = (unsigned char)temp;
    }

    fclose(fp);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <page_table_file>\n", argv[0]);
        return 1;
    }

    PageTable ptab;

    if (load_page_table(&ptab, argv[1]) != 0) {
        return 1;
    }

    // Optional: print loaded page table
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        printf("Page %2d -> Physical %2d (valid=%d)\n", i, ptab.entries[i].physical_page_no, ptab.entries[i].valid);
    }

    // init the cache with pages 0, 1, 2, 3 of the same order.
    TLB cache;
    for (unsigned short i = 0; i < TLB_SIZE; i++) {
        cache.entries[i].valid = 1;
        cache.entries[i].order = i;
        cache.entries[i].virtual_page_no = i;
        cache.entries[i].physical_page_no = ptab.entries[i].physical_page_no;
    }
    printf("=============\n");
    for (int i = 0; i < TLB_SIZE; i++) {
        printf("Page %2d -> Physical %2d (valid=%d)\n", cache.entries[i].virtual_page_no, cache.entries[i].physical_page_no, cache.entries[i].valid);
    }

    // TODO: read list of 16 bit ints. Parse and add to cache. Implement LRU. return physical address.
    int addresses[] = {0x123f, 0x2a13, 0x7fff, 0xe124, 0x7e21};
    int len = sizeof(addresses) / sizeof(addresses[0]);
    for (int i = 0; i < len; i++) {
        int addy = addresses[i];
        unsigned short virt_addy = (addy & 0xf000) >> 12;
        // check if cache contains this physical address

        int cache_hit = -1;
        for (int c = 0; c < TLB_SIZE; c++) {
            if (cache.entries[c].virtual_page_no == virt_addy) {
                cache_hit = c;
            }
        }
        if (cache_hit != -1) {
            unsigned short phys_addy = cache.entries[cache_hit].physical_page_no;
            addy = (phys_addy << 12) | (addy & 0x0fff);
            printf("Cache hit! address = 0x%04x\n", addy);

            // update order of access
            int order = cache.entries[cache_hit].order;
            for (int abba = 0; abba < TLB_SIZE; abba++) {
                if (cache.entries[abba].order < order) {
                    cache.entries[abba].order += 1;
                }
            }
            cache.entries[cache_hit].order = 0;
        } else {
            // cache miss
            // find the page in memory
            unsigned short phys_addy = ptab.entries[virt_addy].physical_page_no;
            addy = (phys_addy << 12) | (addy & 0x0fff);
            printf("Cache miss. address = 0x%04x\n", addy);
            
            // add the page to cache. Kick out LRU
            int LRU;
            for (int abba = 0; abba < TLB_SIZE; abba++) {
                if (cache.entries[abba].order == 3) {
                    LRU = abba;
                }
                cache.entries[abba].order += 1;
            }
            cache.entries[LRU].physical_page_no = phys_addy;
            cache.entries[LRU].virtual_page_no = virt_addy;
            cache.entries[LRU].order = 0;
            cache.entries[LRU].valid = 1;
        }
    }

    return 0;
}
