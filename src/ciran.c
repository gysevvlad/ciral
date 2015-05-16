#include <stdio.h>

#include "list.h"
#include "components.h"

int main( int argc, char *argv[] ) {
	FILE *fp;
	int correct = 1,
	    reti;
	element_t elem;
	list_t *elements = NULL;

	if ( argc != 2 ) {
		fprintf(stderr, "%s\n", "Usage: ciran file");
		return -1;
	}
	fp = fopen(argv[1], "r");

	do {
		reti = element_get_next( fp, &elem );
		if ( reti == 1 )
			elements = list_add( elements, &elem, sizeof(element_t) );
		if ( reti == -1 ) 
			correct = 0;
	} while ( reti != 0 );

	if ( correct == 0 ) {
		fprintf(stderr, "\nParse error of file \"%s\".\n", argv[1]);
		return -1;
	}

	list_foreach( elements, element_print );
}