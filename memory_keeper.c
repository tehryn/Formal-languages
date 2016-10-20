#ifndef MEMORY
#define MEMORY
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
struct mem_item_t;
struct mem_list_t;

typedef struct mem_item_t {
	void *ptr;
	size_t size;
	int used;
	struct mem_item_t *next;
} mem_item_t;

typedef struct mem_list_t {
	mem_item_t *first;
}

mem_list_t * mem_list_t_init(mem_list_t *L) {
	L->first = NULL;
}

static void add_item(void *ptr, size_t size, mem_list_t *L) {
	mem_item_t *tmp = L->first;
	if (L->first == NULL) {
		tmp = ptr;
	}
	else {
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ptr
	}
}

void * mem_alloc(void *ptr, size_t size, mem_list_t *L) {
	ptr = malloc(size);
	if (ptr == NULL)
		return NULL;
	

}
#endif
