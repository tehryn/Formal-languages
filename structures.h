/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* Project: Interpret for IFJ16
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: 2016-12-03
**/
#ifndef STRUCTURES
#define STRUCTURES
#include <stdio.h>
#include <stdlib.h>
#include "ial.h"
//#include "interpret.h"

/** @defgroup structures Structures*/
/** @addtogroup structures
  * @brief Structures is group of structures and functions upon them
  * @{
  */

/*! Default size for allocation memory for Stack of hash tables */
#define STACK_HTAB_INIT_SIZE 16

/*! Default size for allocation memory for array of hash tables */
#define ARRAY_HTAB_INIT_SIZE 64

/*! Default size for allocation memory for array of strings */
#define ARRAY_STRING_INIT_SIZE 8

/*! Default size for allocation memory of stack of instructions */
#define STACK_INSTR_INIT_SIZE 8


typedef struct I_Instr
{
	int type_instr;
	void *adr1;
	void *adr2;
	void *adr3;
	struct I_Instr *next_instr;
}I_Instr;

typedef struct Instr_List{
	I_Instr *Active;
	I_Instr *Last;
}Instr_List;

/**
 * Stack of hash tables
 */
typedef struct stack_htab {
	/** Index of item on top of stack */
	int top;
	/** Maximum number of items after last allocation */
	size_t size;
	/** Array of hash tables */
	htab_t **data;
} stack_htab;

/**
 * Array of hash tables
 */
typedef struct array_htab {
	/** Index where will be added new hash table */
	unsigned idx;
	/** Maximum number of items after last allocation */
	size_t size;
	/** Array of hash tables */
	htab_t **data;
} array_htab;

/**
 * Array of strings
 */
typedef struct array_string {
	/** Index where will be added new hash table */
	unsigned idx;
	/** Maximum number of items after last allocation */
	size_t size;
	/** Array of hash tables */
	char **data;
} array_string;


/**
 * Stack of instructions
 */
typedef struct stack_instr {
	/** Index of item on top of stack */
	int top;
	/** Maximum number of items after last allocation */
	size_t size;
	/** Array of hash tables */
	I_Instr **data;
} stack_instr;

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
 * @pre          Stack was initializated
 */
int stack_htab_push(stack_htab *stack, htab_t *table);

/**
 * Delete item on top
 * @param  stack Stack where item will be deleted
 * @return       pointer to poped table on success if stack is already empty (before pop), return NULL
 * @pre          Stack was initializated
 */
htab_t *stack_htab_pop(stack_htab *stack);

/**
 * Retrive specific item from stack
 * @param  stack    Stack with items
 * @param  bactrack How far from top item is stored
 * @return          Pointer to specific item or NULL if bactrack is too big
 * @pre             Stack was initializated
 */
htab_t *stack_htab_get_item(stack_htab *stack, unsigned bactrack);

/**
 * Return item that is at the bottom of stack
 * @param  stack Stack where item is stored
 * @return       Item that is stored on the bottom, NULL if stack is empty
 */
htab_t *stack_htab_get_first(stack_htab *stack);

/**
 * Free all memory allocated by stack
 * @param stack Stack that shall be freed
 * @pre         Stack was initializated
 */
void stack_htab_destroy(stack_htab *stack);

/**
 * Initialize array
 * @param  array array for initialization
 * @return       0 on success, 1 if memory allocation failed
 */
int array_htab_init(array_htab *array);

/**
 * Insert item into array and also reallocate memory if array is full
 * @param  array Array where item will be inserted
 * @param  htab  Item (pointer to hash table) that will be inserted
 * @return       0 on success, 1 when reallocation failed
 * @pre          Array was initializated
 */
int array_htab_insert(array_htab *array, htab_t *htab);

/**
 * Retrive specific item from array
 * @param  array Array with items
 * @param  idx   Index in array
 * @return       Pointer to specific item or NULL if item on index is not initializated
 * @pre          Array was initializated
 */
htab_t *array_htab_get_item(array_htab *array, unsigned idx);

/**
 * Free all memory allocated by array and all memory allocated by all hash tables in array
 * @param array Array that shall be freed
 * @pre         Array was initializated
 */
void array_htab_destroy(array_htab *array);

/**
 * Inicialize new array of strings
 * @param array array that will be initializated
 * @return      0 in case of success, 1 in case of error in memory allocation
 * @pre         input pointer points to allocated space
 * @pre         Array was inicializated
 */
int array_string_init(array_string *array);

/**
 * Make deep copy of string and insert copy into array
 * @param  array array where string will be inserted
 * @param  str   string that will be copied
 * @return       0 in case of success, 1 in case of error while allocating memory
 * @pre          Array was initializated
 */
int array_string_insert(array_string *array, const char *str);

/**
 * Find string in array
 * @param  array Array where string will be sought
 * @param  str   String that will be sought
 * @return       NULL is string was not found, pointer to string if string was found
 * @pre Array was initializated
 */
char *array_string_find(array_string *array, const char *str);

/**
 * Free all memory allocated by array
 * @param array Array that will be freed
 * @pre Array was initializated
 */
void array_string_destroy(array_string *array);


int stack_instr_init(stack_instr *stack);
int stack_instr_push(stack_instr *stack, I_Instr *instr);
I_Instr *stack_instr_pop(stack_instr *stack);
void stack_instr_destroy(stack_instr *stack);
/** @} */

#endif
