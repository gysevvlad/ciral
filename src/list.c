#include "list.h"

void list_foreach_v1( list_t *list, void(func)(void*, void*), void* arg1 ) {
	while( list != NULL ) {
		func( arg1, list-> data );
		list = list-> prev;
	}
	return;
}

void list_foreach_v0( list_t *list, void(func)(void*) ) {
	while( list != NULL ) {
		func( list-> data );
		list = list-> prev;
	}
}

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

void list_add_list( list_t *src, list_t *dst ) {
	if ( src == NULL ) 
		return;
	while( src->prev != NULL ) 
		src = src->prev;
	src->prev = dst;
	if ( dst != NULL ) 
		dst->next = src;
}

list_t* list_map( list_t *list, list_t*(func)(void*)) {
	list_t *list_new;
	if ( list == NULL ) return NULL;
	list_new = func(list->data);
	list = list->prev;
	while( list != NULL ) {
		list_add_list( list_new, func(list->data) );
		list = list->prev;
	}
	return list_new;	
}

void list_foreach3( list_t *list, void(f)(void*,void*, void*), void* first_arg, void* second_arg ) {
	while( list != NULL ) {
		f( first_arg, second_arg, list-> data );
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