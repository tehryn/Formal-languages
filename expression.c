#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "htab.h"
#include "scanner.h"
#include "expression.h"


#define FUN_ARG -266


int expr_analyze ( token t_in, token *t_out, token **postfix_token_array, int *token_count, int *expr_data_type, htab_t *global_table, htab_t *local_table, ...)
{
	void * ma1[2]={0,0};	// memory1 buffer
	//int ma1_top=0;			// memory1 number of pointers
	void * ma2[256]={0,};	// memory2 buffer
	int ma2_top=0;			// memory2 number of pointers
	
	int e_type = -1;
	int lex_rules = 0;
	int end_token = S_SEMICOMMA;	
	va_list al;
	
	if (t_in.id==FUN_ARG)
	{
		va_start(al, local_table);
		end_token = va_arg(al, int);
		va_end(al);
		t_in=get_token();
	}
		
	if (t_in.id<=0 || t_in.id==S_SEMICOMMA || (t_in.id==S_RIGHT_PARE && end_token!=S_COMMA) || t_in.id==end_token)
	{
		fprintf(stderr, "ERROR: Invalid token. func(expr_analyze)\n");
		return ERR_LEXICAL_ANALYSIS;
	}

	struct stack_expresion tmp_exp_stack, postfix_exp_stack;
	*token_count=0;

	if ( stack_expression_init(&tmp_exp_stack, 32) != 0 || stack_expression_init(&postfix_exp_stack, 32) != 0 )
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
	while( input_token.id!=S_SEMICOMMA && input_token.id!=end_token)
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


		else if( input_token.id==TYPE_STRING || input_token.id==TYPE_DOUBLE || input_token.id==TYPE_INT || input_token.id==TYPE_BOOLEAN || input_token.id==S_FALSE || input_token.id==S_TRUE )	// operand (number, string or boolean value)
		{
			if ( (lex_rules&1) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
			
			if (type_priority(input_token.id) > type_priority(e_type))
				e_type = input_token.id;
			
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

			while( tmp_token.id != S_LEFT_PARE )
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


		else if ( operator_priority(input_token.id) != -1)							// operator
		{
			if ( (lex_rules&2) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);


			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);

			while( operator_priority(tmp_token.id) >= operator_priority(input_token.id) )
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


		else if (input_token.id==S_SIMPLE_IDENT || input_token.id==S_FULL_IDENT)
		{
			if ( (lex_rules&1) != 0)
				FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
			
			htab_item *tmp_table_item;
			if (input_token.ptr != NULL)
				FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);


			tmp_table_item = htab_find_item(local_table, input_token.ptr);
			if (tmp_table_item == NULL)
			{
				tmp_table_item = htab_find_item(global_table, input_token.ptr);
				if (tmp_table_item == NULL)
					FATAL_ERROR("ERROR: Symbol not defined. func(expr_analyze)\n", ERR_SEM_NDEF_REDEF);
			}
			
			int ident_type=0;
			
			if (tmp_table_item->func_or_var==1)		// variable
			{
				if (tmp_table_item->data==NULL)
					FATAL_ERROR("ERROR: Expression with uninitialized variable. func(expr_analyze)\n", ERR_UNINICIALIZED_VAR);
				
				input_token.ptr=tmp_table_item;
				ident_type=tmp_table_item->data_type;
				
				if (stack_expression_push(&postfix_exp_stack, input_token) != 0)
					FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;
			}
			
			else if (tmp_table_item->func_or_var==2)
			{
				token fun_in_token;
				int arg_count = tmp_table_item->number_of_arguments;
				
				fun_in_token=get_token();
				if (fun_in_token.id!=S_LEFT_PARE)
					FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
				
				int err_ret=-1;
				token fun_last_token;
				token *fun_param_arr=NULL;
				int fp_exp_count = -1;
				int fparam_data_type=-1;
				
				fun_in_token.id=FUN_ARG;
				
				for (int i=0; i<arg_count-1; i++)
				{
					err_ret=expr_analyze(fun_in_token, &fun_last_token, &fun_param_arr, &fp_exp_count, &fparam_data_type, global_table, local_table, S_COMMA);
					if (err_ret!=0)
						FATAL_ERROR("ERROR: Expression error. func(expr_analyze)\n", err_ret);
					
					if (fun_last_token.id!=S_COMMA || fparam_data_type!=((int *)(tmp_table_item->data))[i])
					{
						free(fun_param_arr);
						fun_param_arr=NULL;
						FATAL_ERROR("ERROR: Incompatible function parameters... func(expr_analyze)\n", ERR_SEM_COMPATIBILITY);
					}
		
					for (int j=0; j<fp_exp_count; j++ )
					{
						if (stack_expression_push(&postfix_exp_stack, fun_param_arr[j]) != 0)
							FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
						ma1[1]=postfix_exp_stack.arr;
					}
					
					free(fun_param_arr);
					fun_param_arr=NULL;
					fp_exp_count = -1;
					fparam_data_type = -1;
				}
				
				err_ret=expr_analyze(fun_in_token, &fun_last_token, &fun_param_arr, &fp_exp_count, &fparam_data_type, global_table, local_table, S_COMMA);
				if (err_ret!=0)
					FATAL_ERROR("ERROR: Expression error. func(expr_analyze)\n", err_ret);
					
				if ( fun_last_token.id!=S_RIGHT_PARE || (arg_count!=0 && fparam_data_type!=((int *)(tmp_table_item->data))[arg_count-1]) )
				{
					free(fun_param_arr);
					fun_param_arr=NULL;
					FATAL_ERROR("ERROR: Incompatible function parameters... func(expr_analyze)\n", ERR_SEM_COMPATIBILITY);
				}
				
				for (int j=0; j<fp_exp_count; j++ )
				{
					if (stack_expression_push(&postfix_exp_stack, fun_param_arr[j]) != 0)
						FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
					ma1[1]=postfix_exp_stack.arr;
				}
				
				free(fun_param_arr);
				fun_param_arr=NULL;
				
				input_token.ptr=tmp_table_item;
				ident_type=tmp_table_item->data_type;

				if (stack_expression_push(&postfix_exp_stack, input_token) != 0)
					FATAL_ERROR("ERROR: Memory could not be allocated. func(expr_analyze)\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;
			}
			
			else
				FATAL_ERROR("ERROR: Symbol not defined. func(expr_analyze)\n", ERR_SEM_NDEF_REDEF);
			
			if (type_priority(ident_type) > type_priority(e_type))
				e_type = ident_type;
			
			lex_rules=9;
		}
		

		else																	// invalid token
			FATAL_ERROR("ERROR: Invalid token. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
		

		input_token=get_token();
	}
	
	if (left_bracket_count>right_bracket_count)
		FATAL_ERROR("ERROR: Unallowed combination of operands and operators. func(expr_analyze)\n", ERR_LEXICAL_ANALYSIS);
	

	*t_out = input_token;
	*expr_data_type = e_type;

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


int operator_priority (int op)
{
	if ( op==S_DIV || op==S_MUL )
		return 5;
	else if ( op==S_MINUS || op==S_PLUS )
		return 4;
	else if ( op==S_LESS_EQUAL || op==S_GREATER_EQUAL || op==S_LESS || op==S_GREATER || op==S_OR || op==S_AND )
		return 3;
	else if ( op==S_EQUAL || op==S_NOT_EQUAL )
		return 2;
	else if ( op==S_COMMA )
		return 1;

	return -1;
}


int type_priority (int type)
{
	if ( type==TYPE_STRING )
		return 5;
	else if ( type==TYPE_DOUBLE )
		return 4;
	else if ( type==TYPE_INT )
		return 3;
	else if ( type==TYPE_BOOLEAN )
		return 1;
	
	return -1;
}

