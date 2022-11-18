#ifndef ACT_H
	#define ACT_H
    #include "../matrix/matrix.h"

    double sigmoid(double input);
    matrix_t* sigmoidPrime(matrix_t* m);
    matrix_t* softmax(matrix_t* m);

#endif