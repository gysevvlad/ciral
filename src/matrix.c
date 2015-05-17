#include "matrix.h"

matrix_t* matrix_create( int r, int c ) {
	matrix_t *matrix;
	int i;
	matrix = (matrix_t*) malloc ( sizeof(matrix_t) );
	matrix->row_count = r;
	matrix->col_count = c;
	matrix->row_head = NULL;
	matrix->col_head = NULL;
	matrix->data = (double**) malloc (sizeof(double*)*r);
	for( i = 0; i < r; i++ )
		matrix->data[i] = (double*) calloc (c, sizeof(double));
	return matrix;
}

double* matrix_at( matrix_t *matrix, int r, int c ) {
	return matrix->data[r]+c;
}

void matrix_print( matrix_t *matrix ) {
	int i, j; 
	size_t tmp;
	printf("         ");
	for ( i = 0; i < matrix->col_count; i++ ) {
		tmp = strlen(matrix->col_head[i]);
		for ( j = 0; j < 8 - tmp; j++, putchar(' '));
		printf("%s ", matrix->col_head[i]);
	}
	printf("\n");
	for ( i = 0; i < matrix->row_count; i++, printf("\n")) {
		tmp = strlen(matrix->row_head[i]);
		for ( j = 0; j < 8 - tmp; j++, putchar(' '));
			printf("%s ", matrix->row_head[i]);	
		for( j = 0; j < matrix->col_count; j++ )
			printf("%f ", *matrix_at(matrix, i, j));
	}
}

void matrix_head_rows( matrix_t *matrix, char **head ) {
	matrix->row_head = head;
}

void matrix_head_cols( matrix_t *matrix, char **head ) {
	matrix->col_head = head;
}

void matrix_destroy( matrix_t* matrix) {
	int i;
	for ( i = 0; i < matrix->row_count; i++ ) {
		free( matrix->row_head[i] );
		free( matrix->data[i] );
	}
	free( matrix->row_head );
	free( matrix->data );
	for ( i = 0; i < matrix->col_count; i++ )
		free( matrix->col_head[i] );
	free( matrix->col_head );
	free( matrix );
}