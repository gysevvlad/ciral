#include "list.h"

list_t* list_add( list_t *list, void *data, size_t size ) {
	list_t *list_new;
	list_new = malloc( sizeof(list_t) );
	list_new-> data = malloc( size );
	memcpy( list_new-> data, data, size );
	list_new-> next = NULL;
	if ( list == NULL ) 
		list_new-> prev = NULL;
	else 
		list-> next = list_new,
		list_new-> prev = list;
	return list_new;
}

void list_foreach( list_t *list, void(f)(void*) ) {
	while( list != NULL ) {
		f( list-> data );
		list = list-> prev;
	}
}

list_t* list_unfold( list_t *list, void*(f)(void), size_t size ) {
	void *data;
	while( (data = f()) != NULL ) {
		list = list_add( list, data, size );	
		free( data );
	}
	return list;
}