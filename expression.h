#ifndef expression_h_INCLUDED
#define expression_h_INCLUDED

#define FATAL_ERROR(message, error_code) do { 	if(ma1[0]!=NULL) free(ma1[0]); if(ma1[1]!=NULL) free(ma1[1]); \
												for(int i=0; i<ma2_top; i++) if(ma2[i]!=NULL) free(ma2[i]); \
												fputs((message), stderr); \
												return (error_code); } while(0)
//

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


struct stack_expresion
{
	token *arr;
	int size;
	int top;
};

int expr_analyze			( token t_in, token *t_out, token **postfix_token_array, int *token_count );
int stack_expression_init 	( struct stack_expresion* s, int size );
int stack_expression_destroy( struct stack_expresion* s );
int stack_expression_empty 	( const struct stack_expresion* s );
int stack_expression_full 	( const struct stack_expresion* s );
int stack_expression_top 	( struct stack_expresion* s, token* t );
int stack_expression_pop 	( struct stack_expresion* s, token* t );
int stack_expression_push	( struct stack_expresion* s, token t );
int operator_priority 		( token t );

#endif	// expression_h_INCLUDED
