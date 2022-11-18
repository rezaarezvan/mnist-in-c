#ifndef IMG_H
	#define IMG_H

	#include "../matrix/matrix.h"

	typedef struct {
		matrix_t* img_data;
		int label;
	} img_t;

	img_t** csv_to_imgs(char* file_string, int number_of_imgs);
	void img_print(img_t* img);
	void img_free(img_t *img);
	void imgs_free(img_t **imgs, int n);
	
#endif