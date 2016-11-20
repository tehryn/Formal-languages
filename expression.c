#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "expression.h"


int expr_analyze(token t_in, token *t_out, token **postfix_token_array, int *token_count)
{

	void * ma1[2]={0,0};	// memory1 buffer
	//int ma1_top=0;			// memory1 number of pointers
	void * ma2[256]={0,};	// memory2 buffer
	int ma2_top=0;			// memory2 number of pointers
	
	int lex_rules = 0;
		
	if (t_in.id<=0 || t_in.id==S_SEMICOMMA)
	{
		fprintf(stderr, "ERROR: Empty expression.\n");
		return ERR_LEXICAL_ANALYSIS;
	}

	struct stack_expresion tmp_exp_stack, postfix_exp_stack;
	*token_count=0;

	if (stack_expression_init(&tmp_exp_stack, 32) != 0 || stack_expression_init(&postfix_exp_stack, 32) != 0)
		FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
	
	ma1[0]=tmp_exp_stack.arr;
	ma1[1]=postfix_exp_stack.arr;


	int left_bracket_count=0, right_bracket_count=0;

	token input_token, tmp_token;
	input_token.id=-66;
	if ( stack_expression_push(&tmp_exp_stack, input_token) != 0 )
		FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
	ma1[0]=tmp_exp_stack.arr;

	input_token=t_in;
	while( input_token.id!=S_SEMICOMMA )
	{

		if (input_token.id < 0)
			FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);

		
		else if (input_token.id == S_LEFT_PARE )								// left bracket 
		{
			if ( (lex_rules&8) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
			left_bracket_count++;
			if (stack_expression_push(&tmp_exp_stack, input_token) != 0)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
			ma1[0]=tmp_exp_stack.arr;
			lex_rules=6;
		}


		else if( (input_token.id>=TYPE_DOUBLE && input_token.id<=TYPE_BOOLEAN) || input_token.id==S_FALSE || input_token.id==S_TRUE || input_token.id==S_SIMPLE_IDENT || input_token.id==S_FULL_IDENT)	// operand (number, string or boolean value)
		{
			if ( (lex_rules&1) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
			
			if (input_token.id==TYPE_STRING)
			{
				char *tmp = (char *)malloc(sizeof(char) * strlen((char *)input_token.ptr) + 1);
				if (tmp == NULL)
					FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
				else
				{
					if (ma2_top<256) 
						ma2[ma2_top++]=tmp_exp_stack.arr;
					else
						FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);

					strcpy(tmp, (char *)input_token.ptr);
					input_token.ptr = tmp;
				}
			}

			if (stack_expression_push(&postfix_exp_stack, input_token) != 0)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
			ma1[1]=postfix_exp_stack.arr;ma1[1]=postfix_exp_stack.arr;
			lex_rules=9;
		}


		else if( input_token.id == S_RIGHT_PARE)								// right bracket 
		{
			if ( (lex_rules&4) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
			
			right_bracket_count++;
			if (right_bracket_count>left_bracket_count)
				break;

			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);

			while( tmp_token.id != S_LEFT_PARE)
			{
				stack_expression_pop(&tmp_exp_stack, &tmp_token);
				if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0)
					FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;
				stack_expression_top(&tmp_exp_stack, &tmp_token);
			}
			stack_expression_pop(&tmp_exp_stack, NULL);										// Remove (
			
			lex_rules=9;
		}


		else if ( operator_priority(input_token) != -1)							// operator
		{
			if ( (lex_rules&2) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
			
			
			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);

			while( operator_priority(tmp_token) >= operator_priority(input_token) )
			{
				stack_expression_pop(&tmp_exp_stack, &tmp_token);
				if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0)
					FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;
				stack_expression_top(&tmp_exp_stack, &tmp_token);
			}
			if (stack_expression_push(&tmp_exp_stack, input_token)!=0)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
			ma1[0]=tmp_exp_stack.arr;
			lex_rules=6;
		}


		else																	// invalid token
			FATAL_ERROR("ERROR: Invalid token. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
		

		input_token=get_token();
	}
	
	if (left_bracket_count>right_bracket_count)
		FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
	

	*t_out = input_token;

	stack_expression_top(&tmp_exp_stack, &tmp_token);
	while( tmp_token.id != -66)
	{
		stack_expression_pop(&tmp_exp_stack, &tmp_token);
		if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0)
			FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
		ma1[1]=postfix_exp_stack.arr;
		stack_expression_top(&tmp_exp_stack, &tmp_token);
	}


	*token_count=postfix_exp_stack.top+1;
	*postfix_token_array = (token *)malloc(sizeof(token) * (postfix_exp_stack.top+1) );
	if (*postfix_token_array==NULL)
		FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
	
	for (int i=0; i<=postfix_exp_stack.top; i++)
		(*postfix_token_array)[i] = postfix_exp_stack.arr[i];
	
	
	if(ma1[0]!=NULL) free(ma1[0]); 
	if(ma1[1]!=NULL) free(ma1[1]);
	
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
			s->arr=new_arr;
			s->size *= 2;
		}
	}

	s->top += 1;
	s->arr[s->top] = t;
	return 0;
}


int operator_priority (token t)
{
	if (t.id==S_DIV || t.id==S_MUL)
		return 5;
	else if (t.id==S_MINUS || t.id==S_PLUS)
		return 4;
	else if ( t.id==S_EQUAL || t.id==S_LESS_EQUAL || t.id==S_GREATER_EQUAL || t.id==S_NOT_EQUAL || t.id==S_LESS || t.id==S_GREATER || t.id==S_OR || t.id==S_AND || t.id==S_COMMA)
		return 3;

	return -1;
}

