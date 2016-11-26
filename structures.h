/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: TODO
**/
#ifndef STRUCTURES
#define STRUCTURES
#include <stdio.h>
#include <stdlib.h>
#include "htab.h"

/**
 * TODO
 */
#define STACK_INIT_SIZE 16

/**
 * TODO
 */
#define ARRAY_INIT_SIZE 64

/**
 * TODO
 */
typedef struct stack_htab {
	int top;
	size_t size;
	htab_t **data;
} stack_htab;

/**
 * TODO
 */
typedef struct array_htab {
	unsigned idx;
	size_t size;
	htab_t **data;
} array_htab;

/**
 * TODO
 */
typedef struct array_string {
	unsigned idx;
	size_t size;
	char **data;
} array_string;

/**
 * Initialize stack
 * @param  stack Stack for initialization
 * @return       0 on succes, 1 when memory allocation failed
 */
int stack_htab_init(stack_htab *stack);

/**
 * Push new item into stack. Reallocate itself if stack is full
 * @param  stack Stack where item will be pushed
 * @param  table Pointer to hash table that will be pushed into stack
 * @return       0 on succes 1 if reallocation failed (memory will not be freed)
 * @pre          Stack was inicializated
 */
int stack_htab_push(stack_htab *stack, htab_t *table);

/**
 * Delete item on top
 * @param  stack Stack where item will be deleted
 * @return       pointer to poped table on succes if stack is already empty (before pop), return NULL
 * @pre          Stack was inicializated
 */
htab_t *stack_htab_pop(stack_htab *stack);

/**
 * Retrive specific item from stack
 * @param  stack    Stack with items
 * @param  bactrack How far from top item is stored
 * @return          Pointer to specific item or NULL if bactrack is too big
 * @pre             Stack was inicializated
 */
htab_t *stack_htab_get_item(stack_htab *stack, unsigned bactrack);

/**
 * Free all memory allocated by stack
 * @param stack Stack that shall be freed
 * @pre         Stack was inicializated
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
 * @param  htab  Item (pointer to hash table) that will be inserted
 * @return       0 on succes, 1 when reallocation failed
 * @pre          Array was inicializated
 */
int array_htab_insert(array_htab *array, htab_t *htab);

/**
 * Retrive specific item from array
 * @param  array Array with items
 * @param  idx   Index in array
 * @return       Pointer to specific item or NULL if item on index is not inicializated
 * @pre          Array was inicializated
 */
htab_t *array_htab_get_item(array_htab *array, unsigned idx);

/**
 * Free all memory allocated by array and all memory allocated by all hash tables in array
 * @param array Array that shall be freed
 * @pre         Array was inicializated
 */
void array_htab_destroy(array_htab *array);

/**
 * Inicialize new array of strings
 * @param array array that will be inicializated
 * @return      0 in case of succes, 1 in case of error in memory allocation
 * @pre         input pointer points to allocated space
 * @pre         Array was inicializated
 */
int array_string_init(array_string *array);

/**
 * Make deep copy of string and insert copy into array
 * @param  array array where string will be inserted
 * @param  str   string that will be copied
 * @return       0 in case of succes, 1 in case of error while allocating memory
 * @pre          Array was inicializated
 */
int array_string_insert(array_string *array, char *str);

/**
 * Find string in array
 * @param  array Array where string will be seeked
 * @param  str   String that will be seeked
 * @return       NULL is string was not found, pointer to string if string was found
 * @pre Array was inicializated
 */
char *array_string_find(array_string *array, char *str);

/**
 * Free all memory allocated by array
 * @param array Array that will be freed
 * @pre Array was inicializated
 */
void array_string_destroy(array_string *array);
#endif
