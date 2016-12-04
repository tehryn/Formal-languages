#include <string.h>
#include <stdbool.h>    // bool
#include <stdio.h>
#include <stdlib.h>
typedef struct token {
	int id;
	void *ptr;
} token;

struct stack_expresion
{
	token *arr;
	int size;
	int top;
};
int stack_expression_init 	( struct stack_expresion* s, int size );
int stack_expression_destroy( struct stack_expresion* s );
int stack_expression_empty 	( const struct stack_expresion* s );
int stack_expression_full 	( const struct stack_expresion* s );
int stack_expression_top 	( struct stack_expresion* s, token* t );
int stack_expression_pop 	( struct stack_expresion* s, token* t );
int stack_expression_push	( struct stack_expresion* s, token t );



typedef struct htab_item {
	char* key; 				// string ID
	unsigned data_type; 	// 0 - not defined, data type for variable or returns type of function
	unsigned func_or_var; 	// 0 - not defined, 1 - variable, 2 - function
	void* data; 			// pointer to the place with data, for function it is int* (int array of data_types of parametres)
	unsigned number_of_arguments; // for function
	unsigned initialized; 	// 0 - not initialized, 1 - initialized
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

void htab_clear_items(htab_t * T);
void htab_free_all(htab_t * T);

htab_item * htab_find_item(htab_t * T, const char * key); // NULL if not there
htab_item * htab_insert_item(htab_t * T, const char * key); // NULL if failed


