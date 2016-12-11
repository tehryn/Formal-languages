/**
 * \file   expression.h
 * \author Sava Nedeljkovic, xnedel08
 * \date   11.12.2016
 * \brief  Documentation for expression processing
 * \project interpreter for IFJ16
 * \details This module is used for processing expressions.
 *  It checks whether expressions follows allowed rules.
 *  Final expression is converted to postfix format.
 */



#ifndef expression_h_INCLUDED
#define expression_h_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ial.h"
#include "scanner.h"


/** @defgroup expression_processing Expression processing */
/** @addtogroup expression_processing
  * @brief Precedence analysis and expression processing
  * of the language IFJ16.
  * @{
  */


/*! Error constants */
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


/*! Macro frees alocated memory, prints error message and returns with given error code */
#define FATAL_ERROR(message, error_code) do { 	if(ma1[0]!=NULL) free(ma1[0]); if(ma1[1]!=NULL) free(ma1[1]); 	\
												fputs((message), stderr); 										\
												return (error_code); } while(0)

/*! Macro that duolicates given string */
#define STRDUP(l, s) do {	char *tmp = (char *)mem_alloc( sizeof(char) * ( strlen((char *)(s)) + 1 ) );				\
							if (tmp == NULL)																			\
								FATAL_ERROR("EXPRESSION: Memory could not be allocated. 11\n", ERR_INTERN_FAULT); 		\
							else																						\
							{																							\
								strcpy(tmp, (char *)(s));																\
								(l) = tmp;																				\
							} 																							\
						} while(0)

/**  \struct stack_expresion
 * \brief Structure for stack of tokens
 */
struct stack_expresion
{
	/** \brief Pointer to an array of tokens */
	token *arr;
	/** \brief Size of stack (array length) */
	int size;
	/** \brief Index of a top element in a stack */
	int top;
};


/**
 * \brief Function analyzes precedence and converts expression to postfix format.
 * \param[in] t_in first token in an expression.
 * \param[out] t_out last token red by the function, which is not contained in a final expression.
 * \param[in] class_name name of a class, which contains processed expression.
 * \param[in] error_6_flag flag which determines whether error number 6 could be returned.
 * \param[out] postfix_token_array final postfix expression.
 * \param[out] token_count number of tokens in a retuned expression array.
 * \param[out] expr_data_type data type of the retuned expression.
 * \param[in] global_table global table of symbols.
 * \param[in] local_table local table of symbols.
 * \pre class_name!=NULL
 * \pre global_table!=NULL
 * \pre local_table!=NULL
 * \pre error_6_flag==1 || error_6_flag==0
 * \post postfix_token_array!=NULL
 * \post t_out!=NULL
 * \post token_count!=0
 * \post postfix_token_array[token_count-1]==END_EXPR
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int expr_analyze 				( token t_in, token *t_out, char* class_name, int error_6_flag, token **postfix_token_array, int *token_count, int *expr_data_type, htab_t *global_table, htab_t *local_table, ...);



/**
 * \brief Function initializes a stack, allocates required memory and sets its variables.
 * \param[in, out] s pointer to a stack.
 * \param[in] size max. number of tokens which stack could contain.
 * \pre s!=NULL
 * \pre size!=0
 * \post s!=NULL
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_expression_init 		( struct stack_expresion* s, int size );



/**
 * \brief Function destroys a stack, frees its memory and sets its variables.
 * \param[in, out] s pointer to a stack.
 * \pre s!=NULL
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_expression_destroy	( struct stack_expresion* s );



/**
 * \brief Function checks whether stack is empty.
 * \param[in] s pointer to a stack.
 * \pre s!=NULL
 * \return integer value which tells, if the stack is empty, 0 -> not empty, !=0 -> empty.
 */
int stack_expression_empty 		( const struct stack_expresion* s );



/**
 * \brief Function checks whether stack is full.
 * \param[in] s pointer to a stack.
 * \pre s!=NULL
 * \return integer value which tells, if the stack is full, 0 -> not full, !=0 -> full.
 */
int stack_expression_full 		( const struct stack_expresion* s );



/**
 * \brief Function gives back top element from the stack.
 * \param[in] s pointer to a stack.
 * \param[out] t top element from the stack.
 * \pre s!=NULL
 * \pre t!=NULL
 * \pre s is not empty
 * \post t!=NULL
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_expression_top 		( struct stack_expresion* s, token* t );



/**
 * \brief Function pops and gives back top element from the stack.
 * \param[in] s pointer to a stack.
 * \param[out] t top element from the stack.
 * \pre s!=NULL
 * \pre t!=NULL
 * \pre s is not empty
 * \post t!=NULL
 * \post s->size == s->size-1
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_expression_pop 		( struct stack_expresion* s, token* t );



/**
 * \brief Function pushes given element to the stack.
 * \param[in] s pointer to a stack.
 * \param[out] t token which should be pushed to the stack.
 * \pre s!=NULL
 * \pre s is not full
 * \post s->size == s->size+1
 * \post s->arr[s->size] == t
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int stack_expression_push		( struct stack_expresion* s, token t );



/**
 * \brief Function tells the priority of a given operator.
 * \param[in] op operator symbol.
 * \pre op>=0
 * \return integer value which tells the priority of a given operator.
 */
int operator_priority 			(int op);



/**
 * \brief Function tells the priority of a given data type,
 *  which is later used for determining data type of the whole expression.
 * \param[in] type data type symbol.
 * \pre type>=0
 * \return integer value which tells the priority of a given data type.
 */
int type_priority 				(int type);



/**
 * \brief Function converts names of the given data type,
 *  so it could be understood by the function expr_analyze().
 * \param[in] type data type symbol.
 * \return converted integer value which tells the what is the given data type.
 */
int type_name_convertion 		(int type);



/**
 * \brief Function prints token value (and its id) to STDERR.
 *  This function is only used for debugging.
 * \param[in] t token which should be printed.
 * \param[in] id_flag value which determines whether the id of a token should be printed as well.
 * \pre id_flag == 0 || id_flag == 1
 */
void print_token				(token t, int id_flag);



/**
 * \brief Function prints array of tokens (and their ids) to STDERR.
 *  This function is only used for debugging.
 * \param[in] arr array of tokens which should be printed.
 * \param[in] id_flag value which determines whether the id of a tokens should be printed as well.
 * \pre id_flag == 0 || id_flag == 1
 */
void print_token_array			(token * arr, int id_flag);



/** @} */
#endif	// expression_h_INCLUDED
