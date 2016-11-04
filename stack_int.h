#ifndef stack_int_h_INCLUDED
#define stack_int_h_INCLUDED

#include <stdlib.h>
#include <stdarg.h>
#inlclude "garbage_collector.h"

struct t_stack_int
{
	int top;
	int size;
	int data[];
} ;


int stack_int_create ( struct t_stack_int ** stack, int n );

int stack_int_resize ( struct t_stack_int** stack, int new_size );

void stack_int_destroy( struct t_stack_int * stack );

int stack_int_push(struct t_stack_int* stack, int num, ...);

int stack_int_pop(struct t_stack_int* stack, int* var);

int stack_int_clean(struct t_stack_int* stack, int n);

int stack_int_is_empty( struct t_stack_int * stack );

int stack_int_is_full( struct t_stack_int * stack );

#endif 	// stack_int_h_INCLUDED