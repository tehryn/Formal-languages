/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: TODO
**/
#include "stack.h"


int stack_htab_init(stack_htab *stack) {
	stack->data = (htab_t *) malloc(sizeof(htab_t)*STACK_INIT_SIZE);
	if (stack->data == NULL) {
		stack->size = 0;
		stack->top = -1;
		return 1;
	}
	stack->size = STACK_INIT_SIZE;
	stack->top = -1;
	return 0;
}

int stack_htab_push(stack_htab *stack, htab_t table) {
	stack->top++;
	if ((unsigned)stack->top >= stack->size) {
		htab_t *tmp = stack->data;
		stack->size *= 2;
		tmp =(htab_t *) realloc(stack->data, stack->size);
		if (tmp == NULL) {
			stack->size /= 2;
			return 1;
		}
		stack->data = tmp;
	}
	stack->data[stack->top] = table;
	return 0;
}

int stack_htab_pop(stack_htab *stack) {
	if (stack->top < 0)
		return 1;
	stack->top--;
	return 0;
}

htab_t *stack_htab_get_item(stack_htab *stack, unsigned bactrack) {
	int idx = stack->top - bactrack;
	if (idx < 0)
		return NULL;
	return &stack->data[idx];
}

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
	array->data = (htab_t *) malloc(ARRAY_INIT_SIZE * sizeof(htab_t));
	if (array->data == NULL) {
		array->size = 0;
		return 1;
	}
	return 0;
}

int array_htab_insert(array_htab *array, htab_t htab) {
	if (array == NULL) {
		return 1;
	}
	if (array->idx >= array->size) {
		array_htab *tmp;
		array->size *= 2;
		tmp = (array_htab *) realloc(array, array->size);
		if (tmp == NULL) {
			array->size /= 2;
			return 1;
		}
		array = tmp;
	}
	array->data[array->idx++] = htab;
	return 0;
}

void array_htab_destroy(array_htab *array) {
	if (array != NULL) {
		for (unsigned i = 0; i < array->idx; i++) {
			htab_clear_items(&array->data[i]);
		}
		free(array->data);
	}
}
