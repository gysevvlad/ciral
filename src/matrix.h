#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int row_count,
	    col_count;
	double **data;
	char **row_head,
	     **col_head;
} matrix_t;

matrix_t* matrix_create( int, int );

void matrix_head_rows( matrix_t*, char** );

void matrix_head_cols( matrix_t*, char** );

void matrix_print( matrix_t* );

void matrix_destroy( matrix_t* );

matrix_t* matrix_row_delete( matrix_t*, int );

matrix_t* matrix_col_delete( matrix_t*, int );

double* matrix_at(matrix_t* ,int, int);

#endif