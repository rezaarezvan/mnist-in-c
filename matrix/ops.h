#ifndef OPS_H
	#define OPS_H

    #include "matrix.h"

    matrix_t* multiply(matrix_t* m1, matrix_t* m2);
    matrix_t* add(matrix_t* m1, matrix_t* m2);
    matrix_t* subtract(matrix_t* m1, matrix_t* m2);
    matrix_t* dot(matrix_t* m1, matrix_t* m2);
    matrix_t* apply(double (*func)(double), matrix_t* m);
    matrix_t* scale(double n, matrix_t* m);
    matrix_t* addScalar(double n, matrix_t* m);
    matrix_t* transpose(matrix_t* m);

#endif