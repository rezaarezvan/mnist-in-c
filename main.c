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
  srand(time(NULL));

  // TRAINING
  int number_imgs = 10000;
  Img **imgs = csv_to_imgs("data/mnist_test.csv", number_imgs);
  NeuralNetwork *net = network_create(784, 300, 10, 1);
  network_train_batch_imgs(net, imgs, number_imgs);
  network_save(net, "testing_net");

  // PREDICTING
  // int number_imgs = 3000;
  // Img **imgs = csv_to_imgs("data/mnist_test.csv", number_imgs);
  // NeuralNetwork *net = network_load("testing_net");
  // double score = network_predict_imgs(net, imgs, 1000);
  // printf("Score: %1.5f", score);

  // imgs_free(imgs, number_imgs);
  // network_free(net);
  return 0;
}