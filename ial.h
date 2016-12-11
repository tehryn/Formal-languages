/**
* Author: TODO
* Login: TODO
* School: VUT FIT, BRNO
* Project: Interpret for IFJ16
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: TODO
**/

#ifndef IAL
#define IAL

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "garbage_collector.h"
#include "error.h"

/** */
#define MAX(a,b) ((a)   > (b) ? (a) : (b))

/** */
#define AlphabetArray 256

/**
 * [length description]
 * @param  string [description]
 * @return        [description]
 */
int length(char * string);

/**
 * [substring description]
 * @param  s [description]
 * @param  i [description]
 * @param  n [description]
 * @return   [description]
 */
char * substring(char * s, int i, int n);

/**
 * [shellsort description]
 * @param  str [description]
 * @return     [description]
 */
char * shellsort(char * str);

/**
 * [find description]
 * @param  s      [description]
 * @param  search [description]
 * @return        [description]
 */
int find( char *s,  char *search);

/**
 * [computeJumps description]
 * @param string   [description]
 * @param badchar[AlphabetArray] [description]
 */
void computeJumps( char *string, int badchar[AlphabetArray]);

/**
 *
 */
typedef struct t_stack_int
{
	/** */
	int top;
	/** */
	int size;
	/** */
	int *data; // array
} stack_int_t;

/**
 * [stack_int_create  description]
 * @param  stack [description]
 * @param  n     [description]
 * @return       [description]
 */
int stack_int_create ( struct t_stack_int * stack, int n );

/**
 * [stack_int_destroy description]
 * @param stack [description]
 */
void stack_int_destroy( struct t_stack_int * stack );

/**
 * [stack_int_push description]
 * @param  stack   [description]
 * @param  num     [description]
 * @param  VARARGS [description]
 * @return         [description]
 */
int stack_int_push(struct t_stack_int* stack, int num, ...);

/**
 * [stack_int_pop description]
 * @param  stack [description]
 * @return       [description]
 */
int stack_int_pop(struct t_stack_int* stack);

/**
 * [stack_int_top description]
 * @param  stack [description]
 * @param  var   [description]
 * @return       [description]
 */
int stack_int_top(struct t_stack_int* stack, int* var);

/**
 * [stack_int_clean description]
 * @param  stack [description]
 * @param  n     [description]
 * @return       [description]
 */
int stack_int_clean(struct t_stack_int* stack, int n);

/**
 * [stack_int_is_empty description]
 * @param  stack [description]
 * @return       [description]
 */
int stack_int_is_empty( struct t_stack_int * stack );

/**
 * [stack_int_is_full description]
 * @param  stack [description]
 * @return       [description]
 */
int stack_int_is_full( struct t_stack_int * stack );

/** @defgroup hash_table Hash table */
/** @addtogroup hash_table
  * @brief Group of structure hash table where is stored variables, functions and classes and functions that operate upon it
  * @{
  */

/**
 * Abstract data type that represents item of hash table
 */
typedef struct htab_item {

	/** String ID */
	char* key;
	/** Data type for variable or returns type of function */
	unsigned data_type;
	/** Tells if item represents function or variable (0 - not defined, 1 - variable, 2 - function) */
	unsigned func_or_var;
	/** pointer to the place with data, for function it is int* (int array of data_types of parametres) */
	void* data;
	/** 0 - not initialized, 1 - initialized */
	unsigned initialized;
	/** Number of arguments in function */
	unsigned number_of_arguments;
	/** Pointer to local symbol table */
	void * local_table;
	/** Pointer to instruction tabe of function */
	void * instruction_tape;
	/** If item is argument of function, it tells which it is argument */
	int argument_index;
	/** Pointer to next item */
	struct htab_item* next_item;
} htab_item;


/**
 * Abstract data type that represents hash table
 */
typedef struct htab_t {
	/** Pointer to hash function, &hash_function by default */
	unsigned (*hash_fun_ptr) (const char * str, unsigned htab_size);
	/** Size of table (number of lines) */
	unsigned htab_size;
	/** Real number of items */
	unsigned number_items;
	/** Array of pointers to items */
	htab_item **ptr;
} htab_t;

/**
 * Creates new table with default hash function
 * @param  size Size of new table
 * @return      Pointer to new table
 */
htab_t * htab_init(unsigned size);

/**
 * Creates new table with specific hash function
 * @param  size     Size of new table
 * @param  hash_fun Pointer to hash function
 * @return          Pointer to new table
 */
htab_t * htab_init2(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size));

/**
 * Finds item by key in table
 * @param  T   Table where item will be sought
 * @param  key Key of searched item
 * @return     Pointer to searched item, if item will not be find, returns NULL
 */
htab_item * htab_find_item(htab_t * T, const char * key); // NULL if not there

/**
 * Creates new item and insert it into table
 * @param  T   Table where item will be inserted
 * @param  key Key of new item (key will be copied)
 * @return     Pointer to new item
 */
htab_item * htab_insert_item(htab_t * T, const char * key); // NULL if failed

/**
 * Makes copy of table (without pointers)
 * @param  T Table that will be copied
 * @return   Copy of table
 */
htab_t * htab_copy(htab_t * T);

/**
 * Finds arguments of functions
 * @param  T     Table where function is stored
 * @param  index Index of argument
 * @return       Item where is argument stored
 * @pre index >= 0
 */
htab_item * htab_find_item_by_argument_index(htab_t * T, int index); // NULL if not there

/**
 * Free memory allocated when inserting items but since we use garbage collector, it only sets pointers to NULL
 * @param T Table where items will be freed
 */
void htab_clear_items(htab_t * T);

/**
 * Free allocated memory but since we use garbage collector, it only sets pointers to NULL
 * @param T Table that will be freed
 */
void htab_free_all(htab_t * T);
/** @} */
#endif
