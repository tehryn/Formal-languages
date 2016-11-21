/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: TODO
**/
#ifndef STACK
#define STACK
#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#define STACK_INIT_SIZE 16
#define ARRAY_INIT_SIZE 64

/**
 *
 */
typedef struct stack_htab {
	int top;
	size_t size;
	htab_t *data;
} stack_htab;


typedef struct array_htab {
	unsigned idx;
	size_t size;
	htab_t *data;
} array_htab;

/**
 * Initialize stack
 * @param  stack Stack for initialization
 * @return       0 on succes, 1 when memory allocation failed
 */
int stack_htab_init(stack_htab *stack);

/**
 * Push new item into stack. Reallocate itself if stack is full
 * @param  stack Stack where item will be pushed
 * @param  table Hash table that will be pushed into stack
 * @return       0 on succes 1 if reallocation failed (memory will not be freed)
 */
int stack_htab_push(stack_htab *stack, htab_t table);

/**
 * Delete item on top
 * @param  stack Stack where item will be deleted
 * @return       0 on succes if stack is already empty (before pop), return 1
 */
int stack_htab_pop(stack_htab *stack);

/**
 * Retrive specific item from stack
 * @param  stack    Stack with items
 * @param  bactrack How far from top item is stored
 * @return          Pointer to specific item
 */
htab_t *stack_htab_get_item(stack_htab *stack, unsigned bactrack);

/**
 * Free all memory allocated by stack
 * @param stack Stack that shall be freed
 */
void stack_htab_destroy(stack_htab *stack);

/**
 * Initialize array
 * @param  array array for initialization
 * @return       0 on succes, 1 if memory allocation failed
 */
int array_htab_init(array_htab *array);

/**
 * Insert item into array and also reallocate memory if array is full
 * @param  array Array where item will be inserted
 * @param  htab  Item (hash table) that will be inserted
 * @return       0 on succes, 1 when reallocation failed
 */
int array_htab_insert(array_htab *array, htab_t htab);

/**
 * Free all memory allocated by array
 * @param array Array that shall be freed
 */
void array_htab_destroy(array_htab *array);
#endif
