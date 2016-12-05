#include "structures.h"

/**
 * Initialize stack
 * @param  stack Stack for initialization
 * @return       0 on succes, 1 when memory allocation failed
 */
int stack_htab_init(stack_htab *stack) {
	stack->data = (htab_t **) malloc(sizeof(htab_t *)*STACK_HTAB_INIT_SIZE);
	if (stack->data == NULL) {
		stack->size = 0;
		stack->top = -1;
		return 1;
	}
	stack->size = STACK_HTAB_INIT_SIZE;
	stack->top = -1;
	return 0;
}

/**
 * Push new item into stack. Reallocate itself if stack is full
 * @param  stack Stack where item will be pushed
 * @param  table Pointer to hash table that will be pushed into stack
 * @return       0 on succes 1 if reallocation failed (memory will not be freed)
 * @pre          Stack was inicializated
 */
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

/**
 * Delete item on top
 * @param  stack Stack where item will be deleted
 * @return       pointer to poped table on succes if stack is already empty (before pop), return NULL
 * @pre          Stack was inicializated
 */
htab_t *stack_htab_pop(stack_htab *stack) {
	if (stack->top < 0)
		return NULL;
	return stack->data[stack->top--];
}

/**
 * Retrive specific item from stack
 * @param  stack    Stack with items
 * @param  bactrack How far from top item is stored
 * @return          Pointer to specific item or NULL if bactrack is too big
 * @pre             Stack was inicializated
 */
htab_t *stack_htab_get_item(stack_htab *stack, unsigned bactrack) {
	int idx = stack->top - bactrack;
	if (idx < 0)
		return NULL;
	return stack->data[idx];
}

/**
 * Return item that is at the bottom of stack
 * @param  stack Stack where item is stored
 * @return       Item that is stored on the bottom, NULL if stack is empty
 */
htab_t *stack_htab_get_first(stack_htab *stack) {
	return stack->data[0];
}

/**
 * Free all memory allocated by stack
 * @param stack Stack that shall be freed
 * @pre         Stack was inicializated
 */
void stack_htab_destroy (stack_htab *stack) {
	if (stack != NULL) {
		free(stack->data);
	}
}

/**
 * Initialize array
 * @param  array array for initialization
 * @return       0 on succes, 1 if memory allocation failed
 */
int array_htab_init(array_htab *array) {
	if (array == NULL) {
		return 1;
	}
	array->idx = 0;
	array->size = ARRAY_HTAB_INIT_SIZE;
	array->data = (htab_t **) malloc(ARRAY_HTAB_INIT_SIZE * sizeof(htab_t*));
	if (array->data == NULL) {
		array->size = 0;
		return 1;
	}
	return 0;
}

/**
 * Insert item into array and also reallocate memory if array is full
 * @param  array Array where item will be inserted
 * @param  htab  Item (pointer to hash table) that will be inserted
 * @return       0 on succes, 1 when reallocation failed
 * @pre          Array was inicializated
 */
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

/**
 * Retrive specific item from array
 * @param  array Array with items
 * @param  idx   Index in array
 * @return       Pointer to specific item or NULL if item on index is not inicializated
 * @pre          Array was inicializated
 */
htab_t* array_htab_get_item(array_htab *array, unsigned idx) {
	if (idx >= array->idx) {
		return NULL;
	}
	else {
		return array->data[idx];
	}
}

/**
 * Free all memory allocated by array and all memory allocated by all hash tables in array
 * @param array Array that shall be freed
 * @pre         Array was inicializated
 */
void array_htab_destroy(array_htab *array) {
	if (array != NULL) {
//		for (unsigned i = 0; i < array->idx; i++) {
//			htab_free_all((array->data[i]));
//		}
		free(array->data);
	}
}

/**
 * Inicialize new array of strings
 * @param array array that will be inicializated
 * @return      0 in case of succes, 1 in case of error in memory allocation
 * @pre         input pointer points to allocated space
 * @pre         Array was inicializated
 */
int array_string_init(array_string *array) {
	array->idx = 0;
	array->data = (char **) malloc(sizeof(char *)*ARRAY_STRING_INIT_SIZE);
	if (array->data == NULL) {
		array->size = 0;
		return 1;
	}
	array->size = ARRAY_STRING_INIT_SIZE;
	return 0;
}

/**
 * Make deep copy of string and insert copy into array
 * @param  array array where string will be inserted
 * @param  str   string that will be copied
 * @return       0 in case of succes, 1 in case of error while allocating memory (no memory will be freed)
 * @pre          Array was inicializated
 */
int array_string_insert(array_string *array, const char *str) {
	if (array->idx >= array->size) {
		array->size *= 2;
		char **tmp = (char **) realloc(array->data, array->size*sizeof(char *));
		if (tmp == NULL) {
			array->size /= 2;
			return 1;
		}
		array->data = tmp;
	}
	char *new_string = (char *) malloc(strlen(str) + 1);
	if (new_string == NULL) {
		return 1;
	}
	strcpy(new_string, str);
	array->data[array->idx++] = new_string;
	return 0;
}

/**
 * Find string in array
 * @param  array Array where string will be seeked
 * @param  str   String that will be seeked
 * @return       NULL is string was not found, pointer to string if string was found
 * @pre Array was inicializated
 */
char *array_string_find(array_string *array, const char *str) {
	for (unsigned i = 0; i < array->idx; i++) {
		if (!strcmp(str, array->data[i])) {
			return array->data[i];
		}
	}
	return NULL;
}

/**
 * Free all memory allocated by array
 * @param array Array that will be freed
 * @pre Array was inicializated
 */
void array_string_destroy(array_string *array) {
//	for (unsigned i = 0; i < array->idx; i++) {
//		free(array->data[i]);
//	}
	free(array->data);
}
