/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: 25. 10. 2016
**/
#include "stack.h"


stack_t *stack_t_init(stack_t *stack, size_t size) {
	stack->data = (htab_t *) malloc(sizeof(htab_t)*size);
	if (stack->data == NULL)
		return NULL;
	stack->size = size;
	stack->top = -1;
	return stack;
}

int stack_t_push(stack_t *stack, htab_t table) {
	stack->top++;
	if ((unsigned)stack->top >= stack->size) {
		htab_t *tmp = stack->data;
		stack->size *= 2;
		tmp =(htab_t *) realloc(stack->data, stack->size);
		if (tmp == NULL) {
			stack->size /= 2;
			return 0;
		}
		stack->data = tmp;
	}
	stack->data[stack->top] = table;
	return 1;
}

int stack_t_pop(stack_t *stack) {
	if (stack->top < 0)
		return 0;
	stack->top--;
	return 1;
}

htab_t *stack_t_get_item(stack_t *stack, unsigned bactrack) {
	int idx = stack->top - bactrack;
	if (idx < 0)
		return NULL;
	return &stack->data[idx];
}
