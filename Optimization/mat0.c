#include <stdio.h>
#include "matrix.h"


long mat0_slow(matrix_t mat) {
    long sum = 0;
    for (int i = 0; i < matrix_rows(mat); i++) {
        for (int j = 0; j < matrix_cols(mat); j++) {
            int value = matrix_get(mat, i, j);
            printf("%d ", value);
            sum += value;
        }
        printf("\n");
    }
    return sum;
}

long mat0_fast(matrix_t mat) {
    long sum = 0;
    int rows = matrix_rows(mat);
    int cols = matrix_cols(mat);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int value = matrix_get(mat, i, j);
            printf("%d ", value);
            sum += value;
        }
        printf("\n");
    }
    return sum;
}
