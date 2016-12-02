/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: 25. 10. 2016
**/

#include "garbage_collector.h"

mem_list_t GARBAGE_COLLECTOR;

void mem_list_t_init() {
	GARBAGE_COLLECTOR.first = GARBAGE_COLLECTOR.last = NULL;
}

static void *add_item() {
	mem_item_t *new_item = (mem_item_t *) malloc(sizeof(mem_item_t));
	if (new_item == NULL)
		return NULL;

	if (GARBAGE_COLLECTOR.first == NULL)
		GARBAGE_COLLECTOR.first = new_item;

	else
		GARBAGE_COLLECTOR.last->next = new_item;
	GARBAGE_COLLECTOR.last = new_item;
	return new_item;
}

static mem_item_t *find_item(void *ptr) {
	mem_item_t *tmp = GARBAGE_COLLECTOR.first;
	while (tmp->ptr != ptr && tmp)
		tmp = tmp->next;
	return tmp;
}

void * mem_alloc(size_t size) {
	mem_item_t *item =(mem_item_t *) add_item();
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

void * mem_realloc(void *ptr, size_t size) {
	mem_item_t *item = find_item(ptr);
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

void free_memory() {
	mem_item_t *tmp;
	while(GARBAGE_COLLECTOR.first) {
		tmp = GARBAGE_COLLECTOR.first->next;
		free(GARBAGE_COLLECTOR.first->ptr);
		free(GARBAGE_COLLECTOR.first);
		GARBAGE_COLLECTOR.first = tmp;
	}
}
