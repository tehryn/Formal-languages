/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: TODO
**/
#include "structures.h"


int stack_htab_init(stack_htab *stack) {
	stack->data = (htab_t **) malloc(sizeof(htab_t *)*STACK_INIT_SIZE);
	if (stack->data == NULL) {
		stack->size = 0;
		stack->top = -1;
		return 1;
	}
	stack->size = STACK_INIT_SIZE;
	stack->top = -1;
	return 0;
}

int stack_htab_push(stack_htab *stack, htab_t *table) {
	stack->top++;
	if ((unsigned)stack->top >= stack->size) {
		htab_t **tmp = stack->data;
		stack->size *= 2;
		tmp =(htab_t **) realloc(stack->data, stack->size*sizeof(htab_t*));
		if (tmp == NULL) {
			stack->size /= 2;
			return 1;
		}
		stack->data = tmp;
	}
	stack->data[stack->top] = table;
	return 0;
}

htab_t *stack_htab_pop(stack_htab *stack) {
	if (stack->top < 0)
		return NULL;
	return stack->data[stack->top--];
}

htab_t *stack_htab_get_item(stack_htab *stack, unsigned bactrack) {
	int idx = stack->top - bactrack;
	if (idx < 0)
		return NULL;
	return stack->data[idx];
}
/*
bool found = false;
unsigned i = 0;
htab_t *table;
htab_item *item;
while (!found) {
	table = stack_htab_get_item(&stack, bactrack);
	if ((item = htab_find_item(table, key)) != NULL) {
		found = true;
	}
}
 */

void stack_htab_destroy (stack_htab *stack) {
	if (stack != NULL) {
		free(stack->data);
	}
}

int array_htab_init(array_htab *array) {
	if (array == NULL) {
		return 1;
	}
	array->idx = 0;
	array->size = ARRAY_INIT_SIZE;
	array->data = (htab_t **) malloc(ARRAY_INIT_SIZE * sizeof(htab_t*));
	if (array->data == NULL) {
		array->size = 0;
		return 1;
	}
	return 0;
}

int array_htab_insert(array_htab *array, htab_t *htab) {
	if (array == NULL) {
		return 1;
	}
	if (array->idx >= array->size) {
		htab_t **tmp;
		array->size *= 2;
		tmp = (htab_t **) realloc(array->data, array->size*sizeof(htab_t*));
		if (tmp == NULL) {
			array->size /= 2;
			return 1;
		}
		array->data = tmp;
	}
	array->data[array->idx++] = htab;
	return 0;
}

htab_t* array_htab_get_item(array_htab *array, unsigned idx) {
	if (idx >= array->idx) {
		return NULL;
	}
	else {
		return array->data[idx];
	}
}

void array_htab_destroy(array_htab *array) {
	if (array != NULL) {
		for (unsigned i = 0; i < array->idx; i++) {
			htab_free_all((array->data[i]));
		}
		free(array->data);
	}
}
