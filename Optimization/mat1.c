#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "measure.h"

long mat1_slow(matrix_t mat) {
    long sum = 0;
    for (int i = 0; i < matrix_rows(mat); i++) {
        int row_result = 0;
        for (int j = 0; j < matrix_cols(mat); j++) {
            row_result += matrix_get(mat, i, 0) + matrix_get(mat, i, j);
        }
        printf("%d %d\n", i, row_result);
        sum += row_result;
    }
    return sum;
}

long mat1_fast(matrix_t mat) {
    long sum = 0;
    for (int i = 0; i < matrix_rows(mat); i++) {
        int row_result = 0;
        int first_val = matrix_get(mat, i, 0);
        for (int j = 0; j < matrix_cols(mat); j++) {
            row_result += first_val + matrix_get(mat, i, j);
        }
        printf("%d %d\n", i, row_result);
        sum += row_result;
    }
    return sum;
}
