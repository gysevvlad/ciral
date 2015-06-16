#include <stdio.h>
#include <malloc.h>
#include <limits.h>

#include "declaration.h"
#include "list.h"
#include "liner_alg.h"
#include "components.h"

static void itos( char* buf, int x );
static char** create_head( int vol, int cur );

int main( int argc, char *argv[] ) {
	FILE *fp;
	matrix_t *matrix    = NULL,
	         *answ      = NULL,
	         *prev_answ = NULL,
	         *mtmp      = NULL;
	list_t *elements       = NULL;
	list_t *diodes         = NULL;
	list_t *convert_diodes = NULL;
	int itmp, i, iter = 0;
	double diff;

	if ( argc != 2 ) {
		fprintf(stderr, "%s\n", "Usage: ciran file");
 		return -1;
	}
	fp = fopen(argv[1], "r");
	if ( fp == NULL ) {
		fprintf(stderr, "%s\n", "File not found");
		return -1;
	}

	/* parsing of file */
	if ( elements_get( fp, &elements, &diodes ) == -1 ) {
		fprintf(stderr, "\nParse error of file \"%s\".\n", argv[1] );
		return -1;
	}

	itmp = ELEM_NODE_COUNT + ELEM_VOLSRC_COUNT + 1;
	answ = matrix_create( itmp - 1, 1 );
do {
	diff = 0;
	matrix_destroy( prev_answ );
	prev_answ = answ;
	convert_diodes = list_map( diodes, element_diod_transform );
	matrix = matrix_create( itmp, itmp + 1 );
	list_foreach_v( 1, elements, matrix_add_element, matrix );
	list_foreach_v( 1, convert_diodes, matrix_add_element, matrix );
	mtmp = matrix_col_delete( matrix, 0 ); matrix_destroy( matrix );
	matrix = matrix_row_delete( mtmp, 0 ); matrix_destroy( mtmp ); mtmp = NULL;
	answ = gauss( matrix );
	if ( answ == NULL ) {
		fprintf(stderr, "Gauss failed");
		return -1;
	}
	matrix_destroy( matrix );
	for( i = 0; i < answ->row_count; i++ )
		diff = max( fabs(*matrix_at( answ, i, 0) - *matrix_at( prev_answ, i, 0)), 
		            diff );
	list_foreach_v( 1, diodes, element_diod_update, answ );
	iter++;
} while ( iter < 10000 && diff > 0.000001);
	printf("Iter: %d\n", iter);

	for( i = 0; i < ELEM_NODE_COUNT; i++ )
		printf("Node %d: %f V\n", i+1, *matrix_at( answ, i, 0));
	for( i = 0; i < ELEM_VOLSRC_COUNT; i++ )
		printf("Current in V%d: %f A\n", i+1, *matrix_at( answ, i+ELEM_NODE_COUNT, 0));
	return 0;
}