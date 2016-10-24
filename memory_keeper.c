/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: 25. 10. 2016
**/

#include "memory_keeper.h"

void mem_list_t_init(mem_list_t *L) {
	L->first = NULL;
}

static void *add_item(mem_list_t *L) {
	mem_item_t *new_item = malloc(sizeof(mem_item_t));
	if (new_item == NULL)
		return NULL;
		
	if (L->first == NULL) 
		L->first = new_item;
	
	else 
		L->last->next = new_item;
	L->last = new_item;
	return new_item;
}

static void *find_item(void *ptr, mem_list_t *L) {
	if (L == NULL) return NULL;
	mem_item_t *tmp = L->first;
	while (tmp->ptr != ptr && tmp)
		tmp = tmp->next;
	return tmp;	
}

void * mem_alloc(size_t size, mem_list_t *L) {
	mem_item_t *item = add_item(L);
	if (item == NULL)
		return NULL;
	item->ptr = malloc(size);
	if (item->ptr == NULL) {
		item->size = 0;
		return NULL;
	}
	item->size = size;
	item->next = NULL;
	return item->ptr;
}

void * mem_realloc(void *ptr, size_t size, mem_list_t *L) {
	mem_item_t *item = find_item(ptr, L);
	if (item == NULL) return NULL;
	
	void *tmp = realloc(item->ptr, size);
	if (tmp == NULL) {
		free(item->ptr);
		item->ptr = NULL;
		item->size = 0;
		return NULL;
	}
	item->ptr = tmp;
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
