#ifndef MEMORY
#define MEMORY
#include <stdio.h>
#include <stdlib.h>

typedef struct mem_item_t {
	void *ptr;
	size_t size;
	struct mem_item_t *next;
} mem_item_t;

typedef struct mem_list_t {
	mem_item_t *first;
	mem_item_t *last;
} mem_list_t;

extern mem_list_t GARBAGE_COLLECTOR;

void mem_list_t_init();
void * mem_alloc(size_t size);
void free_memory();
void * mem_realloc(void *ptr, size_t size);
#endif
