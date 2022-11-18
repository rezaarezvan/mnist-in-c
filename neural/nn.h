#pragma once

#include "../matrix/matrix.h"
#include "../util/img.h"

typedef struct {
	int input;
	int hidden;
	int output;
	double learning_rate;
	matrix_t* hidden_weights;
	matrix_t* output_weights;
} neural_network_t;

neural_network_t* network_create(int input, int hidden, int output, double lr);
void network_train(neural_network_t* net, matrix_t* input_data, matrix_t* output_data);
void network_train_batch_imgs(neural_network_t* net, img_t** imgs, int batch_size);
matrix_t* network_predict_img(neural_network_t* net, img_t* img);
double network_predict_imgs(neural_network_t* net, img_t** imgs, int n);
matrix_t* network_predict(neural_network_t* net, matrix_t* input_data);
void network_save(neural_network_t* net, char* file_string);
neural_network_t* network_load(char* file_string);
void network_print(neural_network_t* net);
void network_free(neural_network_t* net);