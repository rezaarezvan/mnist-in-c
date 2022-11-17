#ifndef ACT_H
	#define ACT_H
    #include "../matrix/matrix.h"

    double sigmoid(double input);
    Matrix* sigmoidPrime(Matrix* m);
    Matrix* softmax(Matrix* m);

#endif