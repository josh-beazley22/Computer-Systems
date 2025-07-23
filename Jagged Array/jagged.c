#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jagged.h"

// Initialize a jagged array object with the given number of bins.
// The array is initially unpacked.
void jagged_init(jagged_t* jagged, int bins) {
    jagged->size = 0;
    jagged->number_of_bins = bins;

    // Allocate memory
    jagged->bins = (entry_t**) malloc(bins * sizeof(entry_t*));

    // Initialize each bin to NULL
    for (int i = 0; i < bins; i++) {
        jagged->bins[i] = NULL;
    }

    // Initialize packed representation to NULL
    jagged->packed_values = NULL;
    jagged->offsets = NULL;
}

void jagged_free(jagged_t* jagged) {
    if (jagged->bins != NULL) {
        // free memory from jagged->bins
        for (int i=0; i < jagged->number_of_bins; i++) {
            if (jagged->bins[i] == NULL) {
                free(jagged->bins[i]);
            } else {
                // free all memory in the linked list
                entry_t* current = jagged->bins[i];
                while (current != NULL) {
                    entry_t* store = current->next;
                    free(current);
                    current = store;
                }
            }
        }
        free(jagged->bins);
    }
    if (jagged->packed_values != NULL) {
        free(jagged->packed_values);
        free(jagged->offsets);
    }
}

// Return the number of elements in the jagged array
int jagged_size(jagged_t* jagged) {
    return jagged->size;
}

// Return the number of bins
int jagged_bins(jagged_t* jagged) {
    return jagged->number_of_bins;
}

// Return the number of slots in the given bin
int jagged_slots(jagged_t* jagged, int bin) {
    if (jagged->bins == NULL) {
        return 0;
    } else {
        entry_t* current = jagged->bins[bin];
        int count = 0;
        while (current != NULL) {
            count += 1;
            current = current->next;
        }
        return count;
    }
}

// Return the element stored at the given bin and slot number.
// Success is set to 0 if the element was found, or -1 otherwise.
// If success is -1, 0 is returned.
int jagged_element(jagged_t* jagged, int bin, int slot, int* success) {
    if (bin < 0 || bin >= jagged->number_of_bins) {
        *success = -1;
        return 0;
    }
    entry_t* current = jagged->bins[bin];
    if (current == NULL) {
        *success = -1;
        return 0;
    }
    for (int i=0; i < slot; i++) {
        current = current->next;
        if (current == NULL) {
            *success = -1;
            return 0;
        }
    }
    *success = 0;
    return current->value;
}

// Add an element to the bin. Return 0 is the element was
// added, or -1 if the representation is packed
int jagged_add(jagged_t* jagged, int bin, int element) {
    // return -1 if jagged is packed
    if (jagged->bins == NULL) {
        return -1;
    }
    // Check if bin index is valid
    if (bin < 0 || bin >= jagged->number_of_bins) {
        return -1;
    }
    // Initialize new_item
    entry_t* new_item = (entry_t*) malloc(sizeof(entry_t));
    if (new_item == NULL) {
        printf("Memory allocation failed.\n");
        return -1;  // Memory allocation failed
    }
    new_item->value = element;
    new_item->next = NULL;

    // Find where to append new_item
    entry_t** current = &jagged->bins[bin];
    while (*current != NULL) {
        current = &(*current)->next;
    }
    *current = new_item;


    // jagged_add is successful
    jagged->size += 1;
    return 0;
}

// Remove the element from the given bin and slot. Return 0 on success,
// or -1 if the representation was packed or element not found.
int jagged_remove(jagged_t* jagged, int bin, int slot) {
    // Return -1 if jagged is packed
    if (jagged->bins == NULL) {
        return -1;
    }
    // Check if bin index is valid
    if (bin < 0 || bin >= jagged->size) {
        return -1;
    }
    // Check if bin is empty
    if (jagged->bins[bin] == NULL) {
        return -1;
    }
    // edge case
    if (slot == 0) {
        entry_t* store = jagged->bins[bin]->next;
        free(jagged->bins[bin]);
        jagged->bins[bin] = store;
        jagged->size -= 1;
        return 0;
    }
    // Iterate until I reach the node before what I would like to remove.
    entry_t* current = jagged->bins[bin];
    for (int i=0; i < slot-1; i++) {
        // Bin is empty
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }
    // current is the node BEFORE what I wish to remove.
    if (current->next == NULL) {
        // this slot does not exist.
        return -1;
    }
    entry_t* store = current->next;
    current->next = current->next->next;
    free(store);
    jagged->size -= 1;
    return 0;
}

// Unpack the jagged array. Return 0 if successful or -1 if the array is
// already unpacked.
int jagged_unpack(jagged_t* jagged) {
    // Array is already unpacked
    if (jagged->bins != NULL) {
        return -1;
    }
    // declare memory for the jagged array.
    jagged->bins = calloc(jagged->number_of_bins, sizeof(entry_t*));

    for (int i=0; i < jagged->number_of_bins; i++) {
        int a;
        int b;
        // last bin
        if (i+1 == jagged->number_of_bins) {
            a = jagged->offsets[i];
            b = jagged->size;
        } else {
            a = jagged->offsets[i];
            b = jagged->offsets[i+1];
        }
        // no values in this bin.
        if (a == b) {
            continue;
        }

        entry_t** current = &jagged->bins[i];
        for (int binz=a; binz < b; binz++) {
            entry_t* new_item = malloc(sizeof(entry_t));
            if (new_item == NULL) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            new_item->value = jagged->packed_values[binz];
            new_item->next = NULL;

            *current = new_item;
            current = &(*current)->next;
        }
    }
    free(jagged->packed_values);
    jagged->packed_values = NULL;
    free(jagged->offsets);
    jagged->offsets = NULL;
    return 0;
}

// Pack the jagged array. Return 0 if successful or -1 if the array is already
// packed.
int jagged_pack(jagged_t* jagged) {
    if (jagged->bins == NULL) {
        // Array is already packed.
        return -1;
    }
    jagged->offsets = (int*) malloc(jagged->number_of_bins * sizeof(int));
    jagged->packed_values = (int*) malloc(jagged->size * sizeof(int));

    int offset_idx = 0;
    for (int i=0; i < jagged->number_of_bins; i++) {
        jagged->offsets[i] = offset_idx;
        entry_t* current = jagged->bins[i];
        while (current != NULL) {
            jagged->packed_values[offset_idx] = current->value;
            current = current->next;
            offset_idx += 1;
        }
    }
    // free memory from jagged->bins
    for (int i=0; i < jagged->number_of_bins; i++) {
        if (jagged->bins[i] == NULL) {
            free(jagged->bins[i]);
        } else {
            // free all memory in the linked list
            entry_t* current = jagged->bins[i];
            while (current != NULL) {
                entry_t* store = current->next;
                free(current);
                current = store;
            }
        }
    }
    free(jagged->bins);
    jagged->bins = NULL;

    return 0;
}

// Print a jagged array out. Useful for debugging
void jagged_print(jagged_t* jagged) {
    printf("%d\n", (jagged)->size);
    printf("%d\n", (jagged)->number_of_bins);
    printf("========\n");

    // Print unpacked jagged array.
    if (jagged->bins != NULL) {
        for (int i=0; i < jagged->number_of_bins; i++) {
            printf(".");
            entry_t* current = jagged->bins[i];
            while (current != NULL) {
                printf("%d ", current->value);
                current = current->next;
            }
            printf("\n");
        }
    } else {  // Print packed jagged array.
        for (int i=0; i < jagged->size; i++) {
            printf("%d ", jagged->packed_values[i]);
        }
        printf("\n");
        for (int i=0; i < jagged->number_of_bins; i++) {
            printf("%d ", jagged->offsets[i]);
        }
        printf("\n");
    }
}
