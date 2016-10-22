#include "memory_keeper.h"

mem_list_t * mem_list_t_init(mem_list_t *L) {
	L->first = NULL;
}

static void *add_item(mem_list_t *L) {
	mem_item_t *new_item = malloc(sizeof(mem_item_t));
	if (new_item == NULL)
		return NULL;
		
	new_item->size = size;
	if (L->first == NUL) 
		L->first = new_item;
	
	else {
		mem_item_t *tmp = L->first;
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = item;
	}
	return item;
}

void * mem_alloc(void *ptr, size_t size, mem_list_t *L) {
	mem_item_t *item = add_item(L);
	if (item == NULL)
		return NULL;
	item->ptr = malloc(size);
	if (item->ptr == NULL) {
		item->size = 0;
		return NULL;
	}
	item->size = size;
	return item->ptr;
}

void free_memory(mem_list_t *L) {
	if (L == NULL) return;
	mem_item_t *tmp;
	while(L->first) {
		tmp = L->first->next;
		free(L->first->ptr);
		free(L->first);
		L->first = tmp;
	}
}
