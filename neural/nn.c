#include "nn.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../matrix/ops.h"
#include "../neural/activations.h"

#define MAXCHAR 1000

// 784, 300, 10
neural_network_t* network_create(int input, int hidden, int output, double lr) {
	neural_network_t* net = malloc(sizeof(neural_network_t));
	net->input = input;
	net->hidden = hidden;
	net->output = output;
	net->learning_rate = lr;
	matrix_t* hidden_layer = matrix_create(hidden, input);
	matrix_t* output_layer = matrix_create(output, hidden);
	matrix_randomize(hidden_layer, hidden);
	matrix_randomize(output_layer, output);
	net->hidden_weights = hidden_layer;
	net->output_weights = output_layer;
	return net;
}

void network_train(neural_network_t* net, matrix_t* input, matrix_t* output) {
	// Feed forward
	matrix_t* hidden_inputs	= dot(net->hidden_weights, input);
	matrix_t* hidden_outputs = apply(sigmoid, hidden_inputs);
	matrix_t* final_inputs = dot(net->output_weights, hidden_outputs);
	matrix_t* final_outputs = apply(sigmoid, final_inputs);

	// Find errors
	matrix_t* output_errors = subtract(output, final_outputs);
	matrix_t* hidden_errors = dot(transpose(net->output_weights), output_errors);

	// Backpropogate
	// output_weights = add(
	//		 output_weights, 
	//     scale(
	// 			  net->lr, 
	//			  dot(
	// 		 			multiply(
	// 						output_errors, 
	//				  	sigmoidPrime(final_outputs)
	//					), 
	//					transpose(hidden_outputs)
	// 				)
	//		 )
	// )
	matrix_t* sigmoid_primed_mat = sigmoidPrime(final_outputs);
	matrix_t* multiplied_mat = multiply(output_errors, sigmoid_primed_mat);
	matrix_t* transposed_mat = transpose(hidden_outputs);
	matrix_t* dot_mat = dot(multiplied_mat, transposed_mat);
	matrix_t* scaled_mat = scale(net->learning_rate, dot_mat);
	matrix_t* added_mat = add(net->output_weights, scaled_mat);
	matrix_free(net->output_weights); // Free the old weights before replacing
	net->output_weights = added_mat;

	matrix_free(sigmoid_primed_mat);
	matrix_free(multiplied_mat);
	matrix_free(transposed_mat);
	matrix_free(dot_mat);
	matrix_free(scaled_mat);

	// hidden_weights = add(
	// 	 net->hidden_weights,
	// 	 scale (
	//			net->learning_rate
	//    	dot (
	//				multiply(
	//					hidden_errors,
	//					sigmoidPrime(hidden_outputs)	
	//				)
	//				transpose(inputs)
	//      )
	// 	 )
	// )
	// Reusing variables after freeing memory
	sigmoid_primed_mat = sigmoidPrime(hidden_outputs);
	multiplied_mat = multiply(hidden_errors, sigmoid_primed_mat);
	transposed_mat = transpose(input);
	dot_mat = dot(multiplied_mat, transposed_mat);
	scaled_mat = scale(net->learning_rate, dot_mat);
	added_mat = add(net->hidden_weights, scaled_mat);
	matrix_free(net->hidden_weights); // Free the old hidden_weights before replacement
	net->hidden_weights = added_mat; 

	matrix_free(sigmoid_primed_mat);
	matrix_free(multiplied_mat);
	matrix_free(transposed_mat);
	matrix_free(dot_mat);
	matrix_free(scaled_mat);

	// Free matrices
	matrix_free(hidden_inputs);
	matrix_free(hidden_outputs);
	matrix_free(final_inputs);
	matrix_free(final_outputs);
	matrix_free(output_errors);
	matrix_free(hidden_errors);
}

void network_train_batch_imgs(neural_network_t* net, img_t** imgs, int batch_size) {
	for (int i = 0; i < batch_size; i++) {
		if (i % 100 == 0) printf("Img No. %d\n", i);
		img_t* cur_img = imgs[i];
		matrix_t* img_data = matrix_flatten(cur_img->img_data, 0); // 0 = flatten to column vector
		matrix_t* output = matrix_create(10, 1);
		output->entries[cur_img->label][0] = 1; // Setting the result
		network_train(net, img_data, output);
		matrix_free(output);
		matrix_free(img_data);
	}
}

matrix_t* network_predict_img(neural_network_t* net, img_t* img) {
	matrix_t* img_data = matrix_flatten(img->img_data, 0);
	matrix_t* res = network_predict(net, img_data);
	matrix_free(img_data);
	return res;
}

double network_predict_imgs(neural_network_t* net, img_t** imgs, int n) {
	int n_correct = 0;
	for (int i = 0; i < n; i++) {
		matrix_t* prediction = network_predict_img(net, imgs[i]);
		if (matrix_argmax(prediction) == imgs[i]->label) {
			n_correct++;
		}
		matrix_free(prediction);
	}
	return 1.0 * n_correct / n;
}

matrix_t* network_predict(neural_network_t* net, matrix_t* input_data) {
	matrix_t* hidden_inputs	= dot(net->hidden_weights, input_data);
	matrix_t* hidden_outputs = apply(sigmoid, hidden_inputs);
	matrix_t* final_inputs = dot(net->output_weights, hidden_outputs);
	matrix_t* final_outputs = apply(sigmoid, final_inputs);
	matrix_t* result = softmax(final_outputs);
	return result;
}

void network_save(neural_network_t* net, char* file_string) {
	mkdir(file_string);
	// Write the descriptor file
	chdir(file_string);
	FILE* descriptor = fopen("descriptor", "w");
	fprintf(descriptor, "%d\n", net->input);
	fprintf(descriptor, "%d\n", net->hidden);
	fprintf(descriptor, "%d\n", net->output);
	fclose(descriptor);
	matrix_save(net->hidden_weights, "hidden");
	matrix_save(net->output_weights, "output");
	printf("Successfully written to '%s'\n", file_string);
	chdir(".."); // Go back to the orignal directory
}

neural_network_t* network_load(char* file_string) {
	neural_network_t* net = malloc(sizeof(neural_network_t));
	char entry[MAXCHAR];
	chdir(file_string);

	FILE* descriptor = fopen("descriptor", "r");
	fgets(entry, MAXCHAR, descriptor);
	net->input = atoi(entry);
	fgets(entry, MAXCHAR, descriptor);
	net->hidden = atoi(entry);
	fgets(entry, MAXCHAR, descriptor);
	net->output = atoi(entry);
	fclose(descriptor);
	net->hidden_weights = matrix_load("hidden");
	net->output_weights = matrix_load("output");
	printf("Successfully loaded network from '%s'\n", file_string);
	chdir("-"); // Go back to the original directory
	return net;
}

void network_print(neural_network_t* net) {
	printf("# of Inputs: %d\n", net->input);
	printf("# of Hidden: %d\n", net->hidden);
	printf("# of Output: %d\n", net->output);
	printf("Hidden Weights: \n");
	matrix_print(net->hidden_weights);
	printf("Output Weights: \n");
	matrix_print(net->output_weights);
}

void network_free(neural_network_t *net) {
	matrix_free(net->hidden_weights);
	matrix_free(net->output_weights);
	free(net);
	net = NULL;
}