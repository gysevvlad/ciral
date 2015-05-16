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

void list_foreach( list_t *list, void(f)(void*) );

list_t* list_unfold( list_t *list, void*(f)(void), size_t size );

#endif LIST_H