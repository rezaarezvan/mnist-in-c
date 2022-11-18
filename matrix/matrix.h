#ifndef MTR_H
	#define MTR_H

	typedef struct {
		double** entries;
		int rows;
		int cols;
	} matrix_t;

	matrix_t* matrix_create(int row, int col);
	void matrix_fill(matrix_t *m, int n);
	void matrix_free(matrix_t *m);
	void matrix_print(matrix_t *m);
	matrix_t* matrix_copy(matrix_t *m);
	void matrix_save(matrix_t* m, char* file_string);
	matrix_t* matrix_load(char* file_string);
	void matrix_randomize(matrix_t* m, int n);
	int matrix_argmax(matrix_t* m);
	matrix_t* matrix_flatten(matrix_t* m, int axis);
#endif