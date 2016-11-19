#include "expression.h"
#include <stdlib.h>
#include "scanner.h"
#include "error.h"



// first parametr is pointer to an empty array of tokens, where final postfix expression will be stored
// second parametr is pointer to a token, where the last read token will be stored
/*	==== calling example ====

	token *token_arr=NULL;
	token token_back;
	
	expresion_in_to_post( &token_arr, &token_back );
	
	// token_arr holds final postfix expression, last token in array has .id == 0 (similar to \0 terminated string) and should be excluded from further calculation
	// token_back holds last read token
	
	// example of iterating over an array of tokens:
	for (int i=0; token_arr[i].id!=0; i++)
	{
		// do something with token_arr[i];
	}
*/


int expresion_in_to_post(token **postfix_token_array, token *return_token)
{

	struct stack_expresion tmp_exp_stack, postfix_exp_stack;

	if (stack_expression_init(&tmp_exp_stack, 50) != 0)
		return -1;

	if (stack_expression_init(&postfix_exp_stack, 50) != 0)
	{
		stack_expression_destroy(&tmp_exp_stack);
		return -1;
	}
	
	
	int left_bracket_count=0, right_bracket_count=0;

	token input_token, tmp_token;
	input_token.id=0;

	stack_expression_push(&tmp_exp_stack, input_token);
	
	input_token=get_token();
	while( input_token.id!=S_SEMICOMMA )
	{
		
		if (input_token.id == S_LEFT_PARE )														// left bracket (
		{	
			left_bracket_count++;
			if (stack_expression_push(&tmp_exp_stack, input_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -1);
		}

		else if(input_token.id>=TYPE_DOUBLE && input_token.id<=TYPE_STATIC_BOOLEAN)				// operand (number or string)
		{
			if (stack_expression_push(&postfix_exp_stack, input_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -2);
		}
		
		else if( input_token.id == S_RIGHT_PARE)												// right bracket )
		{
			right_bracket_count++;
			if (right_bracket_count>left_bracket_count) 
				break;
			
			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -3);
			
			while( tmp_token.id != S_LEFT_PARE)
			{
				stack_expression_pop(&tmp_exp_stack, &tmp_token);
				if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -4);
				stack_expression_top(&tmp_exp_stack, &tmp_token);
			}
			stack_expression_pop(&tmp_exp_stack, NULL); 											// Remove (
		}
		else if ( operator_priority(input_token) != -1)												// operator
		{	   
			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -5);
			while( operator_priority(tmp_token) >= operator_priority(input_token) )
			{
				stack_expression_pop(&tmp_exp_stack, &tmp_token);
				if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -6);
				stack_expression_top(&tmp_exp_stack, &tmp_token);
			}
			stack_expression_push(&tmp_exp_stack, input_token);
		}
		else																						// invalid token
		{
			 expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -7);
		}
		
		input_token=get_token();
	}
	

	
	stack_expression_top(&tmp_exp_stack, &tmp_token);
	while( tmp_token.id != 0)
	{
		stack_expression_pop(&tmp_exp_stack, &tmp_token);
		if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -1);
		stack_expression_top(&tmp_exp_stack, &tmp_token);
	}
	if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0) expression_fatal_error(&tmp_exp_stack, &postfix_exp_stack, -1);


	
	*postfix_token_array = postfix_exp_stack.arr;
	*return_token = input_token;
	
	stack_expression_destroy(&tmp_exp_stack);
	
	return 0;
}


int stack_expression_init ( struct stack_expresion* s, int size  )
{
	s->arr = (token *)malloc(size * sizeof(token));
	if (s->arr !=NULL )
	{
		s->top = -1;
		s->size = size;
		return 0;
	}
	else
		return -1;
}


int stack_expression_destroy ( struct stack_expresion* s )
{
	if (s->arr != NULL )
	{
		free(s->arr);
		s->top = -1;
		s->size = 0;
		s->arr=NULL;
		return 0;
	}
	else
		return -1;
}


int stack_expression_empty ( const struct stack_expresion* s )
{
	return (s->top == -1);
}


int stack_expression_full ( const struct stack_expresion* s )
{
	return (s->top == ( s->size - 1) );
}


int stack_expression_top ( struct stack_expresion* s, token* t  )
{
	if ( stack_expression_empty(s) )
		return 1;
	
	else
	{
		*t = s->arr[s->top];
		return 0;
	}
}


int stack_expression_pop ( struct stack_expresion* s, token* t )
{
	if ( stack_expression_empty(s) )
		return 1;
	else
	{
		if (t != NULL)
			*t = s->arr[s->top];
		
		s->top -= 1;
		return 0;
	}
}


int stack_expression_push ( struct stack_expresion* s, token t )
{
	if ( stack_expression_full(s) )
	{
		token *new_arr = NULL;
		new_arr = (token *)realloc(s->arr, 2 * s->size * sizeof(token));
		if (new_arr == NULL)
			return 1;
		else
		{
			//free(s->arr);
			s->arr=new_arr;
		}
	}

	s->top += 1;
	s->arr[s->top] = t;
	return 0;
}


// TODO - add missing operators (<, >, ||, ...) 
int operator_priority (token t)
{
	if (t.id==S_DIV || t.id==S_MUL)
		return 5;
	else if (t.id==S_MINUS || t.id==S_PLUS)
		return 4;
	else if ( t.id==S_EQUAL || t.id==S_LESS_EQUAL || t.id==S_GREATER_EQUAL || t.id==S_NOT_EQUAL || t.id==S_COMMA)
		return 3;

	return -1;
}


void expression_fatal_error(struct stack_expresion* s1, struct stack_expresion* s2, int err_num)
{
	//fprintf(stderr, "FATAL ERROR: %d!\n", err_num);
	
	(void) err_num;
	
	free(s1->arr);
	s1->arr=NULL;
	
	free(s2->arr);
	s2->arr=NULL;
	
	error_msg(ERR_LEXICAL_ANALYSIS, "ERROR: expression process failure.\n");
}
