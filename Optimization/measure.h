#ifndef MEASURE_H
#define MEASURE_H

#include "matrix.h"

// Measure how long the given function takes to execute
double measure(long (*func)(long), long arg, long* result);

// Measure how long the given function takes to execute
double measure_mat(long (*func)(matrix_t mat), matrix_t mat, long* result);

#endif // MEASURE_H
