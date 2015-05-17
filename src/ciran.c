#include <stdio.h>
#include <malloc.h>

#include "list.h"
#include "matrix.h"
#include "components.h"

static void itos( char* buf, int x ) {
	int size = 0,
	    tmp  = x;
	char *p;

	if ( !x ) size = 1;
	while( tmp ) tmp /= 10, size++;
	for( p = buf + size-1; p >= buf; p-- )
		*p = '0' + x % 10, x /= 10;
	*(buf + size) = '\0';
}

static char** create_head( int vol, int cur ) {
	int i, j;
	char **head;
	head = (char**) malloc( sizeof(char*)*(vol+cur) );
	for( i = 0; i < vol; i++ ) {
		head[i] = (char*) malloc( sizeof(char)*16 );
		head[i][0] = 'v';
		itos( head[i]+1, i );
	}
	for( j = 1; i <= vol + cur; i++, j++ ) {
		head[i] = (char*) malloc( sizeof(char)*16 );
		head[i][0] = 'i';
		itos( head[i]+1, j );	
	}
	return head;
}

int main( int argc, char *argv[] ) {
	FILE *fp;
	int correct         = 1,
	    volsrc_count    = 0,
	    number_max      = 0,
	    tmp,
	    reti;
	matrix_t *A, *x, *z;
	element_t elem;
	list_t *elements = NULL;

	if ( argc != 2 ) {
		fprintf(stderr, "%s\n", "Usage: ciran file");
		return -1;
	}
	fp = fopen(argv[1], "r");

	/* parsing of file */
	do {
		reti = element_get_next( fp, &elem );
		if ( reti == 1 ) {
			number_max = (elem.source > number_max)? elem.source: number_max;
			number_max = (elem.drain > number_max)? elem.drain: number_max;
			volsrc_count = (elem.type == VOL)? volsrc_count+1: volsrc_count;
			elements = list_add( elements, &elem, sizeof(element_t) );
		}
		if ( reti == -1 ) 
			correct = 0;
	} while ( reti != 0 );

	if ( correct == 0 ) {
		fprintf(stderr, "\nParse error of file \"%s\".\n", argv[1]);
		return -1;
	}

	list_foreach( elements, element_print );

	/* build matrix */
	tmp =  number_max + 1 + volsrc_count;
	A = matrix_create( tmp, tmp );
	matrix_head_rows( A, create_head( number_max+1, volsrc_count ));
	matrix_head_cols( A, create_head( number_max+1, volsrc_count ));
	matrix_print( A );
}