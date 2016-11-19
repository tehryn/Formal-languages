#ifndef expression_h_INCLUDED
#define expression_h_INCLUDED

#include <stdlib.h>
#include "scanner.h"

struct stack_expresion
{
	token *arr;
	int size;
	int top;
};


int expresion_in_to_post 	( token **postfix_token_array, token *return_token );
int stack_expression_init 	( struct stack_expresion* s, int size  );
int stack_expression_destroy( struct stack_expresion* s );
int stack_expression_empty 	( const struct stack_expresion* s );
int stack_expression_full 	( const struct stack_expresion* s );
int stack_expression_top 	( struct stack_expresion* s, token* t  );
int stack_expression_pop 	( struct stack_expresion* s, token* t );
int stack_expression_push	( struct stack_expresion* s, token t );
int operator_priority 		(token t);
void expression_fatal_error	(struct stack_expresion* s1, struct stack_expresion* s2, int err_num);


#endif 	// expression_h_INCLUDED