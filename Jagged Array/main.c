#include "jagged.h"

int main(int argc, char** argv) {
    jagged_t jagged;
    jagged_init(&jagged, 8);

    // jagged_bins(&jagged);
    jagged_add(&jagged, 1, 5);
    jagged_add(&jagged, 1, 7);

    jagged_add(&jagged, 4, 3);
    jagged_add(&jagged, 1, 13);
    jagged_add(&jagged, 7, 6);
    jagged_add(&jagged, 7, 9);

    jagged_remove(&jagged, 2, 3);

    jagged_pack(&jagged);
    // jagged_unpack(&jagged);
    // jagged_pack(&jagged);
    // jagged_unpack(&jagged);

    jagged_print(&jagged);

    jagged_free(&jagged);
}
