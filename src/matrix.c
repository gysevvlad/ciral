#include "matrix.h"

void matrix_row_swap( matrix_t *X, int a, int b ) {
	double *tmp;
	tmp = X->data[a];
	X->data[a] = X->data[b];
	X->data[b] = tmp;
}

void matrix_insert( matrix_t *to, matrix_t *from, int r, int c ) {
	int i, j;
	for ( i = 0; i < from->row_count; i++ )
		for ( j = 0; j < from->col_count; j++ )
			*matrix_at( to, i+r, j+c ) = *matrix_at( from, i, j );
}

matrix_t* matrix_row_delete( matrix_t* matrix, int r ) {
	int i, j;
	matrix_t* new_matrix = matrix_create( matrix->row_count-1, matrix->col_count);

	for ( i = 0; i < r; i++ )
		for( j = 0; j < matrix->col_count; j++ )
			*matrix_at( new_matrix, i, j ) = *matrix_at( matrix, i, j );
	for ( i++; i < matrix->row_count; i++ )
		for( j = 0; j < matrix->col_count; j++ )
			*matrix_at( new_matrix, i-1, j ) = *matrix_at( matrix, i, j );
	return new_matrix;
}

matrix_t* matrix_col_delete( matrix_t* matrix, int c ) {
	int i, j;
	matrix_t* new_matrix = matrix_create( matrix->row_count, matrix->col_count-1);

	for ( j = 0; j < c; j++ )
		for( i = 0; i < matrix->row_count; i++ )
			*matrix_at( new_matrix, i, j ) = *matrix_at( matrix, i, j );
	for ( j++; j < matrix->col_count; j++ )
		for( i = 0; i < matrix->row_count; i++ )
			*matrix_at( new_matrix, i, j-1 ) = *matrix_at( matrix, i, j );
	return new_matrix;
}

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

	if ( matrix->col_head == NULL ) {
		if ( matrix->row_head == NULL )
			for ( i = 0; i < matrix->row_count; i++, printf("\n"))
				for( j = 0; j < matrix->col_count; j++ )
					printf("%f ", *matrix_at(matrix, i, j));
		else
			for ( i = 0; i < matrix->row_count; i++, printf("\n")) {
				tmp = strlen(matrix->row_head[i]);
				for ( j = 0; j < 8 - tmp; j++, putchar(' '));
					printf("%s ", matrix->row_head[i]);	
				for( j = 0; j < matrix->col_count; j++ )
					printf("%f ", *matrix_at(matrix, i, j));
			}
	}
	else {
		if ( matrix->row_head == NULL ) {
			for ( i = 0; i < matrix->col_count; i++ ) {
				tmp = strlen(matrix->col_head[i]);
				for ( j = 0; j < 8 - tmp; j++, putchar(' '));
					printf("%s ", matrix->col_head[i]);	
			}
			printf("\n");			
			for ( i = 0; i < matrix->row_count; i++, printf("\n"))
				for( j = 0; j < matrix->col_count; j++ )
					printf("%f ", *matrix_at(matrix, i, j));
		}
		else {
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
	if ( matrix == NULL ) return;
	for ( i = 0; i < matrix->row_count; i++ )
		free( matrix->data[i] );
	free( matrix->data );
/*	
	free( matrix->row_head );
	free( matrix->col_head );
*/
	free( matrix );
}