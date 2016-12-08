#ifndef IAL
#define IAL

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include "garbage_collector.h"
#include "error.h"
#define max(a,b) ((a)   > (b) ? a : b)
#define NO_OF_CHARS 256

int length(char * string);

char * substring(char * s, int i, int n);

char * shellsort(char * str);

int find( char *s,  char *search);

void computeMatchJumps(char *string, unsigned int str_size, int match_jump[]);

void computeJumps( char *string, unsigned int str_size, int charjump[]);

typedef struct t_stack_int
{
	int top;
	int size;
	int *data; // array
} stack_int_t;

int stack_int_create ( struct t_stack_int * stack, int n );
void stack_int_destroy( struct t_stack_int * stack );
int stack_int_push(struct t_stack_int* stack, int num, ...);
int stack_int_pop(struct t_stack_int* stack);
int stack_int_top(struct t_stack_int* stack, int* var);
int stack_int_clean(struct t_stack_int* stack, int n);
int stack_int_is_empty( struct t_stack_int * stack );
int stack_int_is_full( struct t_stack_int * stack );


typedef struct htab_item {

	char* key; 				// string ID
	unsigned data_type; 	// 0 - not defined, data type for variable or returns type of function
	unsigned func_or_var; 	// 0 - not defined, 1 - variable, 2 - function
	void* data; 			// pointer to the place with data, for function it is int* (int array of data_types of parametres)
	unsigned initialized; 		// 0 - not initialized, 1 - initialized

	unsigned number_of_arguments; // for function
	void * local_table;		// htab_t* // TODO - zde bude predvyplnena lokalni tabulka s noninitialized variable
	void * instruction_tape;	// for function
	
	int argument_index;		// for variable which is argument of some function, -1 by default for non-argument variable

	struct htab_item* next_item; // next variable

} htab_item;


typedef struct htab_t {
	unsigned (*hash_fun_ptr) (const char * str, unsigned htab_size); // pointer to hash function, &hash_function by default
	unsigned htab_size; // number of lines
	unsigned number_items; // real number of items
	htab_item **ptr; // lines
} htab_t;

htab_t * htab_init(unsigned size);
htab_t * htab_init2(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size));

htab_item * htab_find_item(htab_t * T, const char * key); // NULL if not there
htab_item * htab_insert_item(htab_t * T, const char * key); // NULL if failed

htab_t * htab_copy(htab_t * T);

// pre: index >= 0
htab_item * htab_find_item_by_argument_index(htab_t * T, int index); // NULL if not there

void htab_clear_items(htab_t * T);
void htab_free_all(htab_t * T);

#endif
