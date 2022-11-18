#include "matrix/matrix.c"
#include "matrix/matrix.h"

#include "matrix/ops.c"
#include "matrix/ops.h"

#include "neural/activations.c"
#include "neural/activations.h"

#include "neural/nn.c"
#include "neural/nn.h"

#include "util/img.c"
#include "util/img.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	//TRAINING
	{
		int number_imgs = 30000;
		img_t** imgs = csv_to_imgs("data/mnist_train.csv", number_imgs);
		neural_network_t* net = network_create(784, 300, 10, 0.1);
		network_train_batch_imgs(net, imgs, number_imgs);
		network_save(net, "testing_net");
		imgs_free(imgs, number_imgs);
		network_free(net);
	}

	// PREDICTING
	{
		int number_imgs = 3000;
		img_t** imgs = csv_to_imgs("data/mnist_test.csv", number_imgs);
    img_print(imgs[0]);
		neural_network_t* net = network_load("testing_net");
    matrix_t* res = network_predict_img(net, imgs[0]);
		double score = network_predict_imgs(net, imgs, number_imgs);
		printf("Score: %1.5f\n", score);
    printf("Network predicted %d", matrix_argmax(res));
		imgs_free(imgs, number_imgs);
		network_free(net);
	}

	return 0;
}