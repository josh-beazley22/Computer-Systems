#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "mat2.h"
#include "measure.h"

int main() {
    matrix_t mat;
    matrix_init(&mat, 4, 4, 2);

    long val1 = 0;
    double slow = measure_mat(mat2_slow, mat, &val1);
    printf("mat1 det=%ld\n",  val1);

    matrix_init(&mat, 4, 4, 2);
    long val2 = 0;
    double fast = measure_mat(mat2_fast, mat, &val2);
    printf("mat2 det=%ld\n",  val2);

    printf("slow=%f, fast=%f\n", slow, fast);
    printf("Speedup=%f\n", slow / fast);
}
