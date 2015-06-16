#include "components.h"

/* approximation of diode */
void element_diod_update( matrix_t *result, element_t *diode ) {
	double new_value;
	if ( diode == NULL || result == NULL ) return;
	if ( diode->source == 0 ) 
		new_value = *matrix_at( result, 
		                        diode->drain  - 1, 
		                        0 );
	else 
		if ( diode->drain  == 0 ) 
			new_value = *matrix_at( result, 
			                        diode->source - 1, 
			                        0 );
		else 
			new_value = *matrix_at( result, diode->source - 1, 0 ) - 
			            *matrix_at( result, diode->drain - 1, 0 );
    diode->value = new_value;
}

/* convert diode equivalent elements: resistor, current source */
list_t* element_diod_transform( element_t *diod ) {
	element_t tmp;
	list_t   *list = NULL;
	double Vd = diod->value,
	       Is = 1e-12,
	       Vt = 25.85e-3,
	       I  = Is*(exp(Vd/Vt)-1);

	tmp.source = diod->source;
	tmp.drain  = diod->drain;
	tmp.number = -1; /* temp value, I hope it will work */

	/* add new equivalent resistor */
	tmp.type   = RES;
	tmp.value  = Vt/I;
	list       = list_add( list, &tmp, sizeof(element_t) );

	/* add new equivalent current source */
	tmp.type   = CUR;
	tmp.value  = I - I/Vt*Vd;
	list       = list_add( list, &tmp, sizeof(element_t) );

	return list;
}

int elements_get( FILE *fp, list_t **elements, list_t **diodes ) {
	int reti    = 0,
	    correct = 1;
	element_t elem;
	do {
		reti = element_get_next( fp, &elem );
		if ( reti == 1 ) {
			ELEM_NODE_COUNT = (elem.source > ELEM_NODE_COUNT)? elem.source: ELEM_NODE_COUNT;
			ELEM_NODE_COUNT = (elem.drain > ELEM_NODE_COUNT)?  elem.drain : ELEM_NODE_COUNT;
			ELEM_DIODES_COUNT = (elem.type == DIO)? ELEM_DIODES_COUNT + 1 : ELEM_DIODES_COUNT;
			if ( elem.type == DIO )
				*diodes = list_add( *diodes, &elem, sizeof(element_t) );
			else 
				*elements = list_add( *elements, &elem, sizeof(element_t) );
		}
		if ( reti == -1 ) 
			correct = 0;
	} while ( reti != 0 );
	return correct;
}

void matrix_add_element( matrix_t* matrix, element_t *elem ) {
	int n = -1;
	double *tmp;

	switch (elem->type) {
		case RES:
			tmp = matrix_at( matrix, elem->source, elem->source );
			*tmp = *tmp + 1.0/elem->value;
			tmp = matrix_at( matrix, elem->drain, elem->drain );
			*tmp = *tmp + 1.0/elem->value;
			tmp = matrix_at( matrix, elem->source, elem->drain );
			*tmp = *tmp - 1.0/elem->value;
			tmp = matrix_at( matrix, elem->drain, elem->source );
			*tmp = *tmp - 1.0/elem->value;
			break;

		case CUR:
			tmp = matrix_at( matrix, elem->source, matrix->col_count - 1 );
			*tmp = *tmp - elem->value;
			tmp = matrix_at( matrix, elem->drain, matrix->col_count - 1 );
			*tmp = *tmp + elem->value;
			break;

		case VOL:
			n = ELEM_NODE_COUNT + elem->number + 1;			
			tmp = matrix_at( matrix, n, elem->source );
			*tmp = *tmp + 1;
			tmp = matrix_at( matrix, n, elem->drain );
			*tmp = *tmp - 1;
			tmp = matrix_at( matrix, elem->source, n );
			*tmp = *tmp + 1;
			tmp = matrix_at( matrix, elem->drain, n );
			*tmp = *tmp - 1;
			tmp = matrix_at( matrix, n, matrix->col_count - 1 );
			*tmp = elem->value;
			break;

		default:
			break;
	}
}

/*
ret:
	 1 - ok
	 0 - end of file
	-1 - parse error
*/
int element_get_next( FILE *fp, element_t *elem ) {
	char buf[32],
	     *p = buf;
	     
	if (token_get_next( fp, buf, 32 ) == 0) return 0;
	if (!token_check( buf, 1 )) {
		fprintf(stderr, "Line %d: unknown token \"%s\". Expected: [RVIDCL]<uint>.\n", 
		        PARSE_LINE, buf);
		return -1;
	}
	/* type */
	switch (*p++) {
		case 'R': elem->type = RES; break;
		case 'I': elem->type = CUR; break;
		case 'V': elem->type = VOL; break;
		case 'C': elem->type = CAP; break;
		case 'L': elem->type = IND; break;
		case 'D': elem->type = DIO; break;
	}
	/* number */
	if ( elem->type == VOL )
		elem->number = ELEM_VOLSRC_COUNT++;
	else
		elem->number = atoi(p);

	/* source */
	if (token_get_next( fp, buf, 32 ) == 0) {
		fprintf(stderr, "Line %d: missing token. Expected: <node+>.\n", 
		        PARSE_LINE);
		return -1;
	}
	if (!token_check( buf, 2 )) {
		fprintf(stderr, "Line %d: unknown token \"%s\". Expected: <node+>.\n", 
		        PARSE_LINE, buf);
		return -1;
	}
	elem->source = atoi(buf);

	/* drain */
	if (token_get_next( fp, buf, 32 ) == 0) {
		fprintf(stderr, "Line %d: missing token. Expected: <node->.\n", 
		        PARSE_LINE);
		return -1;
	}
	if (!token_check( buf, 2 )) {
		fprintf(stderr, "Line %d: unknown token \"%s\". Expected: <node->.\n", 
		        PARSE_LINE, buf);
		return -1;
	}
	elem->drain = atoi(buf);

	/* value */
	if (elem->type != DIO) {
		if (token_get_next( fp, buf, 32 ) == 0 ) {
			fprintf(stderr, "Line %d: missing token. Expected: <value>.\n", 
		        PARSE_LINE);
			return -1;
		}
		if (!token_check( buf, 3)) {
			fprintf(stderr, "Line %d: unknown token \"%s\". Expected: <value>.\n", 
		        PARSE_LINE, buf);
			return -1;
		}
		elem->value = atof(buf);
	} else {
		elem->value = 0.8;
	}

	return 1;
}

void element_print( element_t *elem ) {
	char type;
	switch ( elem->type ) {
		case RES: type = 'R'; break;
		case CUR: type = 'I'; break;
		case VOL: type = 'V'; break;
		case CAP: type = 'C'; break;
		case IND: type = 'L'; break;
		case DIO: type = 'D'; break;
		default:  type = 'U'; break;
	}
	printf("%c%d %d %d %f\n", type, elem->number, elem->source, elem->drain, elem->value);
}