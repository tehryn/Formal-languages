#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ial.h"
#include "scanner.h"
#include "expression.h"


#define ERR_WARNING 0
#define ERR_LEXICAL_ANALYSIS 1
#define ERR_SYNTACTIC_ANALYSIS 2
#define ERR_SEM_NDEF_REDEF 3
#define ERR_SEM_COMPATIBILITY 4
#define ERR_SEM_OTHERS 6
#define ERR_INPUT_NUMBER 7
#define ERR_UNINICIALIZED_VAR 8
#define ERR_DIVISION_ZERO 9
#define ERR_OTHERS 10
#define ERR_INTERN_FAULT 99

#define FUN_ARG -266

#define FATAL_ERROR(message, error_code) do { 	if(ma1[0]!=NULL) free(ma1[0]); if(ma1[1]!=NULL) free(ma1[1]); 	\
												for(int i=0; i<ma2_top; i++) if(ma2[i]!=NULL) free(ma2[i]); 	\
												fputs((message), stderr); 										\
												return (error_code); } while(0)


#define STRDUP(l, s) do {	if (ma2_top>=512)																			\
								FATAL_ERROR("EXPRESSION: Memory could not be allocated. 11\n", ERR_INTERN_FAULT); 		\
							char *tmp = (char *)malloc( sizeof(char) * ( strlen((char *)(s)) + 1 ) );					\
							if (tmp == NULL)																			\
								FATAL_ERROR("EXPRESSION: Memory could not be allocated. 11\n", ERR_INTERN_FAULT); 		\
							else																						\
							{																							\
								strcpy(tmp, (char *)(s));																\
								ma2[ma2_top++] = tmp;																	\
								(l) = tmp;																				\
							} 																							\
						} while(0)


int expr_analyze ( token t_in, token *t_out, char* class_name, int error_6_flag, token **postfix_token_array, int *token_count, int *expr_data_type, htab_t *global_table, htab_t *local_table, ...)
{
	void * ma1[2]={0,0};	// memory1 buffer
	//int ma1_top=0;			// memory1 number of pointers
	void * ma2[512]={0,};	// memory2 buffer
	int ma2_top=0;			// memory2 number of pointers


	int bool_operation = 0;
	int return_type_bool=0;
	int e_type = -1;
	int syn_rules = 6;
	int end_token = S_SEMICOMMA;
	va_list al;

	if (t_in.id==FUN_ARG)
	{
		va_start(al, local_table);
		end_token = va_arg(al, int);
		va_end(al);
		t_in=get_token();
	}

	if (t_in.id<=0)
	{
		fprintf(stderr, "EXPRESSION: Invalid token. 1\n");
		return ERR_INTERN_FAULT;
	}


	if (t_in.id==0)
	{
		fprintf(stderr, "EXPRESSION: Invalid token. 2\n");
		return ERR_LEXICAL_ANALYSIS;
	}

	if (t_in.id==S_SEMICOMMA || (t_in.id==S_RIGHT_PARE && end_token!=S_COMMA) || t_in.id==end_token)
	{
		fprintf(stderr, "EXPRESSION: Empty expression. 3\n");
		return ERR_SYNTACTIC_ANALYSIS;
	}

	struct stack_expresion tmp_exp_stack, postfix_exp_stack;
	*token_count=0;

	if ( stack_expression_init(&tmp_exp_stack, 32) != 0 || stack_expression_init(&postfix_exp_stack, 32) != 0 )
		FATAL_ERROR("EXPRESSION: Memory could not be allocated. 4\n", ERR_INTERN_FAULT);

	ma1[0]=tmp_exp_stack.arr;
	ma1[1]=postfix_exp_stack.arr;


	int left_bracket_count=0, right_bracket_count=0;

	token input_token, tmp_token;
	input_token.id=-166;
	if ( stack_expression_push(&tmp_exp_stack, input_token) != 0 )
		FATAL_ERROR("EXPRESSION: Memory could not be allocated. 5\n", ERR_INTERN_FAULT);
	ma1[0]=tmp_exp_stack.arr;

	input_token=t_in;
	while(1)
	{

		if (input_token.id==S_SEMICOMMA || input_token.id==end_token)
		{
			if ( (syn_rules&2)!=0)
				FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 6\n", ERR_SYNTACTIC_ANALYSIS);
			else
				break;
		}

		else if (input_token.id < 0)
			FATAL_ERROR("EXPRESSION: Invalid token. 6.5\n", ERR_INTERN_FAULT);


		else if (input_token.id == S_LEFT_PARE )									// left bracket
		{
			if ( (syn_rules&8) != 0)
				FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 7\n", ERR_SYNTACTIC_ANALYSIS);

			left_bracket_count++;

			if (stack_expression_push(&tmp_exp_stack, input_token) != 0)
				FATAL_ERROR("EXPRESSION: Memory could not be allocated. 8\n", ERR_INTERN_FAULT);
			ma1[0]=tmp_exp_stack.arr;

			syn_rules=6;
		}


		else if( input_token.id==TYPE_STRING || input_token.id==TYPE_DOUBLE || input_token.id==TYPE_INT || input_token.id==TYPE_BOOLEAN || input_token.id==S_FALSE || input_token.id==S_TRUE )	// operand (number, string or boolean value)
		{
			if ( (syn_rules&1) != 0)
				FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 9\n", ERR_SYNTACTIC_ANALYSIS);

			if ( bool_operation>=2 && input_token.id!=TYPE_BOOLEAN && input_token.id!=S_TRUE && input_token.id!=S_FALSE )
				FATAL_ERROR("EXPRESSION: Invalid operand data type in a boolean expression. 9.1\n", ERR_SEM_COMPATIBILITY);

			if ( bool_operation==1 && e_type!=input_token.id && e_type!=S_DOUBLE && input_token.id!=TYPE_DOUBLE && e_type!=S_INT && input_token.id!=TYPE_INT )
				FATAL_ERROR("EXPRESSION: Unallowed operation in an boolean expression. 9.2\n", ERR_SEM_COMPATIBILITY);

			if (type_priority(type_name_convertion(input_token.id)) > type_priority(e_type))
				e_type = type_name_convertion(input_token.id);

			if (input_token.id==TYPE_STRING)
			{
				if (input_token.ptr==NULL)
					FATAL_ERROR("EXPRESSION: String data are not allocated. 10\n", ERR_INTERN_FAULT);

				STRDUP(input_token.ptr, input_token.ptr);
			}

			if (stack_expression_push(&postfix_exp_stack, input_token) != 0)
				FATAL_ERROR("EXPRESSION: Memory could not be allocated. 13\n", ERR_INTERN_FAULT);
			ma1[1]=postfix_exp_stack.arr;ma1[1]=postfix_exp_stack.arr;

			syn_rules=9;
		}


		else if( input_token.id == S_RIGHT_PARE)									// right bracket
		{
			//fprintf(stderr, "%d %d\n", end_token, left_bracket_count);	// odje torima

			if ( (syn_rules&4)!=0 && !( end_token==S_COMMA && e_type==-1))
					FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 14\n", ERR_SYNTACTIC_ANALYSIS);


			right_bracket_count++;
			if (right_bracket_count>left_bracket_count)
				break;

			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
				FATAL_ERROR("EXPRESSION: Memory could not be allocated. 15\n", ERR_INTERN_FAULT);

			while( tmp_token.id != S_LEFT_PARE )
			{
				if (stack_expression_pop(&tmp_exp_stack, &tmp_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 16\n", ERR_INTERN_FAULT);

				if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 17\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;

				if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 18\n", ERR_INTERN_FAULT);
			}

			stack_expression_pop(&tmp_exp_stack, NULL);										// Remove (

			syn_rules=9;
		}


		else if ( operator_priority(input_token.id) != -1)							// operator
		{
			if ( (syn_rules&2) != 0)
				FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 19\n", ERR_SYNTACTIC_ANALYSIS);

			if ( (e_type==S_STRING||e_type==TYPE_STRING) && input_token.id!=S_PLUS )
				FATAL_ERROR("EXPRESSION: Unallowed operation in an expression with string value. 19.1\n", ERR_SEM_COMPATIBILITY);

			if (input_token.id==S_AND || input_token.id==S_OR)
			{
				if (e_type!=S_BOOLEAN)
					FATAL_ERROR("EXPRESSION: Unallowed operation in a boolean expression. 19.2\n", ERR_SEM_COMPATIBILITY);

				return_type_bool=1;
				bool_operation = 0;
				e_type = -1;
			}
			else if ( operator_priority(input_token.id)==3 || operator_priority(input_token.id)==2 )
			{
				bool_operation++;
				e_type=S_BOOLEAN;
			}
			else if (bool_operation>=2)
				FATAL_ERROR("EXPRESSION: Unallowed operation in a boolean expression. 19.3\n", ERR_SEM_COMPATIBILITY);
			else if ( bool_operation==0 && e_type==S_BOOLEAN )
				FATAL_ERROR("EXPRESSION: Unallowed operation in a boolean expression. 19.4\n", ERR_SEM_COMPATIBILITY);

			if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
				FATAL_ERROR("EXPRESSION: Memory could not be allocated. 20\n", ERR_INTERN_FAULT);

			while( operator_priority(tmp_token.id) >= operator_priority(input_token.id) )
			{
				if (stack_expression_pop(&tmp_exp_stack, &tmp_token)!=0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 21\n", ERR_INTERN_FAULT);

				if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 22\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;

				if (stack_expression_top(&tmp_exp_stack, &tmp_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 23\n", ERR_INTERN_FAULT);
			}

			if (stack_expression_push(&tmp_exp_stack, input_token)!=0)
				FATAL_ERROR("EXPRESSION: Memory could not be allocated. 24\n", ERR_INTERN_FAULT);
			ma1[0]=tmp_exp_stack.arr;

			syn_rules=6;
		}


		else if (input_token.id==S_SIMPLE_IDENT || input_token.id==S_FULL_IDENT)	// function or variable
		{
			if ( (syn_rules&1) != 0)
				FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 25\n", ERR_SYNTACTIC_ANALYSIS);

			htab_item *tmp_table_item;
			if (input_token.ptr == NULL)
				FATAL_ERROR("EXPRESSION: Token data are not allocated. 26\n", ERR_INTERN_FAULT);
			char name[strlen((char *)input_token.ptr) + 1];
			char long_name[strlen((char *)input_token.ptr) + strlen(class_name) + 1];
			strcpy(long_name, class_name);
			strcat(long_name, ".");
			strcat(long_name, (char *)input_token.ptr);
			strcpy (name, (char *)input_token.ptr);

			if (input_token.id==S_FULL_IDENT)
			{
				if (global_table!=NULL)
					tmp_table_item = htab_find_item(global_table, name);	// find item in global table if there is global	table (table is not NULL)
				else
					tmp_table_item=NULL;									// set item to NULL if there isn't a global	table (table is NULL)

				if (tmp_table_item == NULL)									// if item is not found or there isn't a global table, exit with error
				{
					fprintf(stderr, "Symbol: %s\n", name);
					FATAL_ERROR("EXPRESSION: Symbol not defined. 27\n", ERR_SEM_NDEF_REDEF);
				}
				else
					STRDUP(input_token.ptr, name);							// if item is found in global table, allocate new memory  for name of the identifier (function or variable name)
			}

			else if (input_token.id==S_SIMPLE_IDENT)
			{
				if (local_table!=NULL)
					tmp_table_item = htab_find_item(local_table, name);				// find item in local table if there is global	table (table is not NULL)
				else
					tmp_table_item=NULL;											// set item to NULL if there isn't a local table (table is NULL)

				if (tmp_table_item == NULL)											// if item is not found in a local table or there isn't a local table, try to find item in a global table
				{
					if (global_table!=NULL)
						tmp_table_item = htab_find_item(global_table, long_name);	// find item in global table if there is global	table (table is not NULL)
					else
						tmp_table_item=NULL;										// set item to NULL if there isn't a global	table (table is NULL)

					if (tmp_table_item == NULL)										// if item is not found neither in global nor local table or there aren't neither in global nor local table, exit with error
					{
						fprintf(stderr, "Symbol: %s\n", name);
						FATAL_ERROR("EXPRESSION: Symbol not defined. 27.1\n", ERR_SEM_NDEF_REDEF);
					}
					else
					{
						STRDUP(input_token.ptr, long_name);							// if item is found in global table, allocate new memory for name of the identifier (function or variable name with class name)
						input_token.id=S_FULL_IDENT;
					}
				}
				else
					STRDUP(input_token.ptr, name);									// if item is found in local table, allocate new memory  for name of the identifier (function or variable name)
			}

			//fprintf(stderr, "test: input_token.id:%d tmp_table_item->func_or_var:%d input_token.ptr:%s key:%s initialized:%d\n", input_token.id, tmp_table_item->func_or_var, (char *)input_token.ptr, tmp_table_item->key, tmp_table_item->initialized);	// odje torima

			int ident_type=-1;

			if (tmp_table_item->func_or_var==1)		// variable
			{
				//if (tmp_table_item->data==NULL)
				if (tmp_table_item->initialized!=1 && error_6_flag==1 )
				{
					fprintf(stderr, "Symbol: %s\n", name);
					FATAL_ERROR("EXPRESSION: Expression with uninitialized variable. 27.2\n", ERR_UNINICIALIZED_VAR);
				}
				input_token.ptr=tmp_table_item;
				ident_type=tmp_table_item->data_type;

				if (stack_expression_push(&postfix_exp_stack, input_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 28\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;
			}

			else if (tmp_table_item->func_or_var==2)
			{
				token fun_in_token;
				int arg_count = tmp_table_item->number_of_arguments;

				fun_in_token=get_token();
				if (fun_in_token.id!=S_LEFT_PARE)
				{
					fprintf(stderr, "Function: %s\n", name);
					FATAL_ERROR("EXPRESSION: Function missing \"(\". 29\n", ERR_SYNTACTIC_ANALYSIS);
				}

				if (arg_count!=0 && tmp_table_item->data==NULL)
				{
					fprintf(stderr, "Function: %s\n", name);
					FATAL_ERROR("EXPRESSION: Function data types are undefined. 29.5\n", ERR_INTERN_FAULT);
				}

				int err_ret=-1;
				token fun_last_token;
				token *fun_param_arr=NULL;
				int fp_exp_count = -1;
				int fparam_data_type=-1;

				fun_in_token.id=FUN_ARG;

				for (int i=0; i<arg_count-1; i++)
				{
					err_ret=expr_analyze(fun_in_token, &fun_last_token, class_name, error_6_flag, &fun_param_arr, &fp_exp_count, &fparam_data_type, global_table, local_table, S_COMMA);
					if (err_ret!=0)
						FATAL_ERROR("EXPRESSION: Expression error. 30\n", err_ret);


					if (fun_last_token.id!=S_COMMA)
					{
						free(fun_param_arr);
						fun_param_arr=NULL;
						fprintf(stderr, "Function: %s, token id: %d.\n", name, fun_last_token.id);
						FATAL_ERROR("EXPRESSION: Function missing \",\" or it has wrong number of parameters. 31\n", ERR_SEM_COMPATIBILITY);
					}


					if (fparam_data_type!=((int *)(tmp_table_item->data))[i])
					{
						free(fun_param_arr);
						fun_param_arr=NULL;
						fprintf(stderr, "Function: %s, parameter data type: %d, parameter position: %d.\n", name, fparam_data_type, i+1);
						FATAL_ERROR("EXPRESSION: Incompatible function parameter. 32\n", ERR_SEM_COMPATIBILITY);
					}

					for (int j=0; j<fp_exp_count; j++ )
					{
						if (stack_expression_push(&postfix_exp_stack, fun_param_arr[j]) != 0)
							FATAL_ERROR("EXPRESSION: Memory could not be allocated. 33\n", ERR_INTERN_FAULT);
						ma1[1]=postfix_exp_stack.arr;
					}

					free(fun_param_arr);
					fun_param_arr=NULL;
					fp_exp_count = -1;
					fparam_data_type = -1;
				}

				err_ret=expr_analyze(fun_in_token, &fun_last_token, class_name, error_6_flag, &fun_param_arr, &fp_exp_count, &fparam_data_type, global_table, local_table, S_COMMA);
				if (err_ret!=0)
					FATAL_ERROR("EXPRESSION: Expression error. 34\n", err_ret);

				if ( fun_last_token.id!=S_RIGHT_PARE )
				{
					free(fun_param_arr);
					fun_param_arr=NULL;
					fprintf(stderr, "Function: %s, token id: %d.\n", name, fun_last_token.id);
					FATAL_ERROR("EXPRESSION: Function missing \")\" or it has wrong number of parameters. 35\n", ERR_SEM_COMPATIBILITY);
				}

				if ( arg_count!=0 && fparam_data_type!=((int *)(tmp_table_item->data))[arg_count-1] && strcmp("ifj16.print", name)!=0 )
				{
					free(fun_param_arr);
					fun_param_arr=NULL;
					fprintf(stderr, "Function: %s, parameter data type: %d, parameter position: %d.\n", name, fparam_data_type, arg_count-1+1);
					FATAL_ERROR("EXPRESSION: Incompatible function parameter or wrong number of parameters. 36\n", ERR_SEM_COMPATIBILITY);
				}


				for (int j=0; j<fp_exp_count; j++ )
				{
					if (stack_expression_push(&postfix_exp_stack, fun_param_arr[j]) != 0)
						FATAL_ERROR("EXPRESSION: Memory could not be allocated. 37\n", ERR_INTERN_FAULT);
					ma1[1]=postfix_exp_stack.arr;
				}

				free(fun_param_arr);
				fun_param_arr=NULL;

				input_token.ptr=tmp_table_item;
				ident_type=tmp_table_item->data_type;


				if (stack_expression_push(&postfix_exp_stack, input_token) != 0)
					FATAL_ERROR("EXPRESSION: Memory could not be allocated. 38\n", ERR_INTERN_FAULT);
				ma1[1]=postfix_exp_stack.arr;
			}

			else
			{
				fprintf(stderr, "Symbol: %s\n", (char *)input_token.ptr);
				FATAL_ERROR("EXPRESSION: Unknown symbol definition. 39\n", ERR_SEM_NDEF_REDEF);
			}

			if ( bool_operation>=2 && ident_type!=TYPE_BOOLEAN && ident_type!=S_BOOLEAN )
				FATAL_ERROR("EXPRESSION: Invalid operand data type in a boolean expression. 39.1\n", ERR_SEM_COMPATIBILITY);

			if ( bool_operation==1 && e_type!=ident_type && e_type!=S_DOUBLE && ident_type!=S_DOUBLE && e_type!=S_INT && ident_type!=S_INT )
				FATAL_ERROR("EXPRESSION: Unallowed operation in an boolean expression. 39.2\n", ERR_SEM_COMPATIBILITY);

			if (type_priority(ident_type) > type_priority(e_type))
				e_type = ident_type;

			syn_rules=9;
		}


		else																	// invalid token
			FATAL_ERROR("EXPRESSION: Invalid token. 40\n", ERR_LEXICAL_ANALYSIS);


		input_token=get_token();
	}

	if (left_bracket_count>right_bracket_count)
		FATAL_ERROR("EXPRESSION: Unallowed combination of operands and operators. 41\n", ERR_SYNTACTIC_ANALYSIS);


	*t_out = input_token;

	if (return_type_bool==1 && e_type!=S_BOOLEAN)
		FATAL_ERROR("EXPRESSION: Unallowed operation in a boolean expression. 41.5\n", ERR_SEM_COMPATIBILITY);

	*expr_data_type = e_type;


	if (stack_expression_top(&tmp_exp_stack, &tmp_token) !=0)
		FATAL_ERROR("EXPRESSION: Memory could not be allocated. 42\n", ERR_INTERN_FAULT);

	while( tmp_token.id != -166)
	{
		if (stack_expression_pop(&tmp_exp_stack, &tmp_token)!=0)
			FATAL_ERROR("EXPRESSION: Memory could not be allocated. 43\n", ERR_INTERN_FAULT);

		if (stack_expression_push(&postfix_exp_stack, tmp_token) != 0)
			FATAL_ERROR("EXPRESSION: Memory could not be allocated. 44\n", ERR_INTERN_FAULT);
		ma1[1]=postfix_exp_stack.arr;

		if (stack_expression_top(&tmp_exp_stack, &tmp_token) !=0)
			FATAL_ERROR("EXPRESSION: Memory could not be allocated. 45\n", ERR_INTERN_FAULT);
	}

	*token_count=postfix_exp_stack.top+1;
	*postfix_token_array = (token *)malloc(sizeof(token) * (postfix_exp_stack.top+1) );
	if (*postfix_token_array==NULL)
		FATAL_ERROR("EXPRESSION: Memory could not be allocated. 46\n", ERR_INTERN_FAULT);

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
	if (type==TYPE_BOOLEAN || type==S_BOOLEAN)
		return 6;
	else if (type==TYPE_STRING || type==S_STRING)
		return 5;
	else if (type==TYPE_DOUBLE || type==S_DOUBLE)
		return 4;
	else if (type==TYPE_INT || type==S_INT)
		return 3;
	else if (type==S_VOID)
		return 1;

	return -1;
}

int type_name_convertion (int type)
{
	if (type==TYPE_STRING || type==S_STRING)
		return S_STRING;
	else if (type==TYPE_DOUBLE || type==S_DOUBLE)
		return S_DOUBLE;
	else if (type==TYPE_INT || type==S_INT)
		return S_INT;
	else if (type==TYPE_BOOLEAN || type==S_BOOLEAN || type==S_FALSE || type==S_TRUE)
		return S_BOOLEAN;
	else if (type==S_VOID)
		return S_VOID;


	return -1;
}
