/**
* Authors: Miroslava Misova, Nemanja Vasiljevic, Jiri Matejka, Sava Nedeljkovic
* School: VUT FIT, BRNO
* Project: Interpret for IFJ16
* gcc version: 5.4.0 (ubuntu 16.04.2)
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
 * Function returns length of 0 terminated string.
 * @param  string array of chars
 * @pre string!=NULL
 * @return integer value, length of string.
 */
int length(char * string);

/**
 * Returns substring of a given length, beginning at a given position of a given string
 * Function allocates new memory for a substring
 * @param  s input string 
 * @param  i index where the substring begins
 * @param  n length of a substring
 * @pre s != NULL
 * @pre n > 0
 * @pre i => 0
 * @pre strlen(s) >= n+i
 * @return array of chars containing found substring
 */
char * substring(char * s, int i, int n);

/**
 * Returns sorted array of a chars.
 * Function uses Shell sort algorithm.
 * Function allocates new memory for a return array of chars.
 * @param  str input string 
 * @pre s != NULL
 * @return sorted array of a chars.
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

/**  \struct t_stack_int
 * \brief Structure for stack of integers
 */
typedef struct t_stack_int
{
	/** Index of a top element in a stack */
	int top;
	/** Size of stack (array length) */
	int size;
	/** Pointer to an array of integers */
	int *data; // array
} stack_int_t;

/** 
 * \brief Function initializes a stack, allocates required memory and sets its variables.
 * \param[in, out] stack pointer to a stack.
 * \param[in] n max. number of elements which stack could contain.
 * \pre stack!=NULL
 * \pre n!=0
 * \post stack!=NULL
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_int_create ( struct t_stack_int * stack, int n );

/** 
 * \brief Function destroys a stack, frees its memory and sets its variables.
 * \param[in, out] s pointer to a stack.
 * \pre stack!=NULL
 */
void stack_int_destroy( struct t_stack_int * stack );

/**
 * Function pushes given elements to the stack.
 * @param  stack   pointer to a stack.
 * @param  num     number of elements which should be pushed to the stack
 * @param  VARARGS integer values which should be oushed to the stack
 * @return         integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_int_push(struct t_stack_int* stack, int num, ...);

/**
 * Function pops and gives back top element from the stack.
 * @param  stack pointer to a stack.
 * @return integer value, top element from the stack.
 */
int stack_int_pop(struct t_stack_int* stack);

/**
 * Function gives back top element from the stack.
 * @param  stack pointer to a stack.
 * @param  var   top element from the stack.
 * @return       integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_int_top(struct t_stack_int* stack, int* var);

/**
 * Decrements stack pointer by given number of elements.
 * @param  stack pointer to a stack.
 * @param  n     number of elements which should be removed from stack. 
 * @return       integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_int_clean(struct t_stack_int* stack, int n);

/**
 * Function checks whether stack is empty.
 * @param  stack pointer to a stack.
 * @return integer value which tells, if the stack is empty, 0 -> not empty, !=0 -> empty.
 */
int stack_int_is_empty( struct t_stack_int * stack );

/**
 * Function checks whether stack is full.
 * @param  stack pointer to a stack.
 * @return integer value which tells, if the stack is full, 0 -> not full, !=0 -> full.
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
