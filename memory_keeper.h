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
}

mem_list_t * mem_list_t_init(mem_list_t *L);
void * mem_alloc(void *ptr, size_t size, mem_list_t *L);
void free_memory(mem_list_t *L);
void * mem_realloc(void *ptr, size_t size, mem_list_t *L);
#endif
