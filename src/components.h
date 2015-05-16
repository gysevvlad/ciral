#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

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

#endif