#include "activations.h"

#include <math.h>
#include "../matrix/ops.h"

double sigmoid(double input) {
	return 1.0 / (1 + exp(-1 * input));
}

matrix_t* sigmoidPrime(matrix_t* m) {
	matrix_t* ones = matrix_create(m->rows, m->cols);
	matrix_fill(ones, 1);
	matrix_t* subtracted = subtract(ones, m);
	matrix_t* multiplied = multiply(m, subtracted);
	matrix_free(ones);
	matrix_free(subtracted);
	return multiplied;
}

matrix_t* softmax(matrix_t* m) {
	double total = 0;
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			total += exp(m->entries[i][j]);
		}
	}
	matrix_t* mat = matrix_create(m->rows, m->cols);
	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			mat->entries[i][j] = exp(m->entries[i][j]) / total;
		}
	}
	return mat;
}