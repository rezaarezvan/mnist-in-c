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
  // srand(time(NULL));

  matrix_t* A = matrix_create(3, 3);
  matrix_t* B = matrix_create(3, 3);
  int k = 1;
  int l = 7;
  for (int i = 0; i < A->rows; i++) {
			for (int j = 0; j < A->cols; j++) {
				A->entries[i][j]  = k++;
        B->entries[i][j]  = l++;
			}
		}

  matrix_print(A);
  matrix_print(B);

  matrix_t* C = multiply(A, B);
  matrix_t* D = multiply(A, B);

  matrix_print(C);
  matrix_print(D);

  // TRAINING
  // int number_imgs_1 = 10000;
  // img_t **imgs_1 = csv_to_imgs("data/mnist_test.csv", number_imgs_1);
  // neural_network_t *net_1 = network_create(784, 500, 10, 1);
  // network_train_batch_imgs(net_1, imgs_1, number_imgs_1);
  // network_save(net_1, "testing_net");

  // PREDICTING
  // int number_imgs_2 = 3000;
  // img_t **imgs = csv_to_imgs("data/mnist_test.csv", number_imgs_2);
  // img_print(imgs[0]);
  // neural_network_t *net = network_load("testing_net");
  // double score = network_predict_imgs(net, imgs[0], 1000);
  // printf("Score: %1.5f", score);

  // imgs_free(imgs, number_imgs_2);
  // network_free(net);
  return 0;
}
