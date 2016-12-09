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
#include <limits.h>
#include "garbage_collector.h"
#include "error.h"

/** */
#define max(a,b) ((a)   > (b) ? a : b)

/** */
#define NO_OF_CHARS 256

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
 * [computeMatchJumps description]
 * @param string     [description]
 * @param str_size   [description]
 * @param match_jump [description]
 */
void computeMatchJumps(char *string, unsigned int str_size, int match_jump[]);

/**
 * [computeJumps description]
 * @param string   [description]
 * @param str_size [description]
 * @param charjump [description]
 */
void computeJumps( char *string, unsigned int str_size, int charjump[]);

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

/**
 *
 */
typedef struct htab_item {

	/** */
	char* key; 				// string ID
	/** */
	unsigned data_type; 	// 0 - not defined, data type for variable or returns type of function
	/** */
	unsigned func_or_var; 	// 0 - not defined, 1 - variable, 2 - function
	/** */
	void* data; 			// pointer to the place with data, for function it is int* (int array of data_types of parametres)
	/** */
	unsigned initialized; 		// 0 - not initialized, 1 - initialized
	/** */
	unsigned number_of_arguments; // for function
	/** */
	void * local_table;		// htab_t* // TODO - zde bude predvyplnena lokalni tabulka s noninitialized variable
	/** */
	void * instruction_tape;	// for function
	/** */
	int argument_index;		// for variable which is argument of some function, -1 by default for non-argument variable
	/** */
	struct htab_item* next_item; // next variable
} htab_item;


/**
 *
 */
typedef struct htab_t {
	/** */
	unsigned (*hash_fun_ptr) (const char * str, unsigned htab_size); // pointer to hash function, &hash_function by default
	/** */
	unsigned htab_size; // number of lines
	/** */
	unsigned number_items; // real number of items
	/** */
	htab_item **ptr; // lines
} htab_t;

/**
 * [htab_init description]
 * @param  size [description]
 * @return      [description]
 */
htab_t * htab_init(unsigned size);

/**
 * [htab_init2 description]
 * @param  size     [description]
 * @param  hash_fun [description]
 * @return          [description]
 */
htab_t * htab_init2(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size));

/**
 * [htab_find_item description]
 * @param  T   [description]
 * @param  key [description]
 * @return     [description]
 */
htab_item * htab_find_item(htab_t * T, const char * key); // NULL if not there

/**
 * [htab_insert_item description]
 * @param  T   [description]
 * @param  key [description]
 * @return     [description]
 */
htab_item * htab_insert_item(htab_t * T, const char * key); // NULL if failed

/**
 * [htab_copy description]
 * @param  T [description]
 * @return   [description]
 */
htab_t * htab_copy(htab_t * T);

/**
 * [htab_find_item_by_argument_index description]
 * @param  T     [description]
 * @param  index [description]
 * @return       [description]
 * @pre index >= 0
 */
htab_item * htab_find_item_by_argument_index(htab_t * T, int index); // NULL if not there

/**
 * [htab_clear_items description]
 * @param T [description]
 */
void htab_clear_items(htab_t * T);

/**
 * [htab_free_all description]
 * @param T [description]
 */
void htab_free_all(htab_t * T);

#endif
