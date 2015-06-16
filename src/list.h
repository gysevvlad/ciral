#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <malloc.h>
#include <string.h>

typedef struct list_struct {
	struct list_struct *next, *prev;
	void *data;
} list_t;


list_t* list_add( list_t *list, void *data, size_t size );

void list_add_list( list_t *src, list_t *dst );

list_t* list_map( list_t *list, list_t*(func)(void*));


#define list_foreach_v( num, ... ) list_foreach_v##num( __VA_ARGS__ )

#define list_foreach list_foreach_v0

void list_foreach_v0( list_t *list, void(func)(void*));

void list_foreach_v1( list_t *list, void(func)(void*, void*), void* arg1 );

void list_foreach3( list_t *list, void(f)(void*, void*, void*), void* first_arg, void* second_arg );

list_t* list_unfold( list_t *list, void*(f)(void), size_t size );

#endif LIST_H