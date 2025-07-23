#include <stdio.h>
#include "matrix.h"
#include "mat0.h"
#include "measure.h"



int main() {
    long val1 = 0;
    matrix_t mat;
    matrix_init(&mat, 10, 10, 0);
    double slow = measure_mat(mat0_slow, mat, &val1);

    long val2 = 0;
    matrix_init(&mat, 10, 10, 0);
    double fast = measure_mat(mat0_fast, mat, &val2);

    printf("slow=%f, fast=%f\n", slow, fast);
    printf("Speedup=%f\n", slow / fast);
}
