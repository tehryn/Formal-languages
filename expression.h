#ifndef expression_h_INCLUDED
#define expression_h_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ial.h"
#include "scanner.h"


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

#define END_EXPR -266

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


struct stack_expresion
{
	token *arr;
	int size;
	int top;
};

/*
// t_in -> vstupni token
// t_out -> ukazatel na token, kde bude ulozena hodnota posledniho tokenu
// postfix_token_array -> ukazatel na pole tokenu, kde budou ulozeny jednotlive tokeny vyrazu ve formatu postfix
// token_count -> ukazatel na int, kde bude ulozen pocet tokenu ve vyslednem poli tokenu
// expr_data_type -> ukazatel na int, kde bude ulozena hodnota datoveho typu celeho vyrazu
// global_table a local_table -> hash. tabulky
// priklad volani:
//	token t_in = get_token();
//	token t_out;
//	token *postfix_token_array;
//	int token_count, expr_data_type;
//	int expr_analyze(t_in, &t_out, &postfix_token_array, &token_count, &expr_data_type, global_table, local_table);
*/
int expr_analyze 			( token t_in, token *t_out, char* class_name, int error_6_flag, token **postfix_token_array, int *token_count, int *expr_data_type, htab_t *global_table, htab_t *local_table, ...);
int stack_expression_init 	( struct stack_expresion* s, int size );
int stack_expression_destroy( struct stack_expresion* s );
int stack_expression_empty 	( const struct stack_expresion* s );
int stack_expression_full 	( const struct stack_expresion* s );
int stack_expression_top 	( struct stack_expresion* s, token* t );
int stack_expression_pop 	( struct stack_expresion* s, token* t );
int stack_expression_push	( struct stack_expresion* s, token t );
int operator_priority 		(int op);
int type_priority 			(int type);
int type_name_convertion 	(int type);

#endif	// expression_h_INCLUDED
