#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "matrix.h"
#include "measure.h"

long determinant(matrix_t mat) {
    // These output statements help see progress in the 
    // calculation
    puts(".");
    fflush(stdout);

    assert(matrix_rows(mat) == matrix_cols(mat));
    if (matrix_rows(mat) == 1) {
        return matrix_get(mat, 0, 0);
    }

    long sum = 0;
    for (int i = 0; i < matrix_cols(mat); i++) {
        matrix_t submat;
        matrix_submatrix(mat, i, &submat);
        long subvalue = matrix_get(mat, 0, i) * determinant(submat);
        if (i % 2 == 0) {
            sum += subvalue;
        } else {
            sum -= subvalue;
        }
        matrix_free(submat);
    }
    return sum;
}

long mat2_slow(matrix_t mat) {
    return determinant(mat);           // calculate the determinant
}

long fast_determinant(matrix_t mat) {
    // These output statements help see progress in the 
    // calculation
    puts("-");
    fflush(stdout);

    // Using Doolittle's Algorithm
    matrix_t L;
    matrix_t U;
    int n = matrix_rows(mat);
    int m = matrix_cols(mat);
    assert(n == m);

    matrix_init(&L, n, m, 2);
    matrix_init(&U, n, m, 2);

    for (int i=0; i < n; i++) {
        // Upper Triangle
        for (int k = i; k < n; k++) {
            int sum = 0;
            for (int j=0; j < i; j++) {
                sum += (matrix_get(L, i, j) * matrix_get(U, j, k));
            }
            int value = matrix_get(mat, i, k) - sum;
            matrix_set(U, i, k, value);
        }
        // Lower Triangle
        for (int k = i; k < n; k++) {
            if (i == k) {
                matrix_set(L, i, i, 1);
            } else {
                int sum = 0;
                for (int j = 0; j < i; j++) {
                    sum += (matrix_get(L, k, j) * matrix_get(U, j, i));
                }
                int value = (int) (matrix_get(mat, k, i) - sum) / matrix_get(U, i, i);
                matrix_set(L, k, i, value);
            }
        }
    }
    // Calculate determinant
    // Product of diagonal elements for the upper triangular matrix (lower tri is all 1 on diagonal).
    int det = 1;
    for (int i = 0; i < n; i++) {
        det *= matrix_get(U, i, i);
    }
    return det;
}

long mat2_fast(matrix_t mat) {
    return fast_determinant(mat);
}
