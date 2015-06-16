#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "declaration.h"
#include "parser.h"
#include "matrix.h"
#include "list.h"

/* circuit elements */
typedef struct {
	enum {
		VOL,   /* Voltage Source */
		CUR,   /* Current Source */
		RES,   /* Resistor       */
		DIO,   /* Diode          */
		CAP,   /* Capacitor      */
		IND    /* Inductor       */
	} type;
	int number,   /* <int>   */
	    source,   /* <node+> */
	    drain;    /* <node-> */
	double value; /* <value> */
} element_t;

/*
ret:
	 1 - ok
	 0 - end of file
	-1 - parse error
*/
int element_get_next( FILE *fp, element_t *elem );

void element_print( element_t *elem );

void matrix_add_element( matrix_t *matrix, element_t *element );

int elements_get( FILE *fp, list_t **elements, list_t **diodes );

list_t* element_diod_transform( element_t *diod );

void element_diod_update( matrix_t *result, element_t *diode );

#endif