#ifndef MATRIX_H
#define MATRIX_H

// Define an abstract data type for a matrix
typedef struct mat* matrix_t;

// Initialize a matrix of the given dimension, with all values set to 0.
// Type defines the type of the matrix. There are 3 types, 0, 1 and 2.
void matrix_init(matrix_t* mat, int rows, int cols, int type);

// Free memory allocated to previously initialized matrix
void matrix_free(matrix_t mat);

// Return the number of rows of the matrix
int matrix_rows(matrix_t mat);

// Return the number of columns of the matrix
int matrix_cols(matrix_t mat);

// Retrieve an element at the given row and col
int matrix_get(matrix_t mat, int row, int col);

// Set the matrix value at the given row and col.
// Return the value that was set.
int matrix_set(matrix_t mat, int row, int col, int value);

// Copy this matrix to an output submatrix with row 0 and the given column
// removed.
void matrix_submatrix(matrix_t mat, int col, matrix_t* outmat);

// Useful for debugging
void matrix_print(matrix_t mat);

#endif // MATRIX_H
