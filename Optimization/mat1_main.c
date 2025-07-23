#include <stdio.h>
#include "matrix.h"
#include "mat1.h"
#include "measure.h"

int main() {
    matrix_t mat;
    matrix_init(&mat, 10, 20, 1);
    long val1 = 0;
    double slow = measure_mat(mat1_slow, mat, &val1);

    matrix_init(&mat, 10, 20, 1);
    long val2 = 0;
    double fast = measure_mat(mat1_fast, mat, &val2);

    printf("slow=%f, fast=%f\n", slow, fast);
    printf("Speedup=%f\n", slow / fast);
}
