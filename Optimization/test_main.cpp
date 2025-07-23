#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <stdio.h>

extern "C" {
#include "../matrix.h"
#include "../oddinacci.h"
#include "../mat0.h"
#include "../mat1.h"
#include "../mat2.h"
#include "../measure.h"
}

#define ODDINACCI_ITERS 40


TEST_CASE("Oddinacci", "[Test]") {
    long val1 = 0;
    double slow = measure(oddinacci, ODDINACCI_ITERS, &val1);

    long val2 = 0;
    double fast = measure(oddinacci_fast, ODDINACCI_ITERS, &val2);

    printf("slow=%lf, fast=%lf\n", slow, fast);
    printf("factor=%f\n", (double) slow / fast);

    REQUIRE(val1 == val2);
    REQUIRE(slow / fast > 100);
}

TEST_CASE("Matrix0", "[Mat0]") {
    matrix_t mat;
    matrix_init(&mat, 10, 10, 0);

    long val1 = 0;
    double slow = measure_mat(mat0_slow, mat, &val1);

    long val2 = 0;
    double fast = measure_mat(mat0_fast, mat, &val2);

    printf("slow=%lf, fast=%lf\n", slow, fast);
    printf("factor=%f\n", (double) slow / fast);

    REQUIRE(val1 == val2);
    REQUIRE(slow / fast > 10);
}

TEST_CASE("Matrix1", "[Mat1]") {
    matrix_t mat;
    matrix_init(&mat, 10, 10, 1);

    long val1 = 0;
    double slow = measure_mat(mat1_slow, mat, &val1);

    long val2 = 0;
    double fast = measure_mat(mat1_fast, mat, &val2);

    printf("slow=%f, fast=%f\n", slow, fast);
    printf("factor=%f\n", slow / fast);

    REQUIRE(val1 == val2);
    REQUIRE(slow / fast > 1.5);
}

TEST_CASE("Matrix2", "[Mat2]") {
    matrix_t mat;
    matrix_init(&mat, 4, 4, 2);

    printf("Calculating slow determinant: ");
    long val1 = 0;
    double slow = measure_mat(mat2_slow, mat, &val1);
    printf("\n");

    printf("Calculating fast determinant: ");
    long val2 = 0;
    double fast = measure_mat(mat2_fast, mat, &val2);
    printf("\n");

    printf("slow=%f, fast=%f\n", slow, fast);
    printf("factor=%f\n", slow / fast);

    REQUIRE(val1 == val2);
    REQUIRE(slow / fast > 2.0);
}

/*
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
