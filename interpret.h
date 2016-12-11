/**
 * \file   expression.h
 * \author Nemanja Vasiljevic, xvasil03
 * \date   11.12.2016
 * \brief  Documentation for interpret processing
 * \details This module is used for processing Instruction list made of 3AC.
 */
 
#ifndef interpret
#define interpret

#include <string.h>
#include <stdbool.h>   
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "structures.h"
#include "expression.h"

/** @defgroup interpret_processing Interpret processing */
/** @addtogroup interpret_processing
  * @brief Interpret processing instruction tape made for language IFJ16.
  * @{
  */

  
/*! Enumator for type of instructions */
enum {
	I_ASSIGMENT = 100+1,		//101
	I_IF,						//102
	I_ELSE,						//103
	I_WHILE,					//104
	I_END, 						//105
	I_FCE,						//106
	I_RETURN,					//107
	I_PRINT,					//108
	I_ENDIF,					//109
	I_ENDWHILE,					//110
	I_ENDELSE					//111
};

/** 
 * \brief Function add instruction at end of instruction tape.
 * \param[in, out] L pointer to instruction tape.
 * \param[in] new pointer to instruction.
 * \pre new!=NULL
 * \pre size!=0
 * \return integer value which tells, how the whole process has been executed, 0 -> no error, -1 -> error.
 */
int Add_Instr(Instr_List *L, I_Instr *new);


/** 
 * \brief Interpret calls this function when instruction has expression. Function process postfix array, manage all aritmetic and bool operation and calls interpret. 
 * \param[in] postfix_array array of postfix tokens.
 * \param[in] I_Htable pointer to stack of tables.
 * \param[in] S pointer to stack for tokens.
 * \param[in] L pointer to instruction tape.
 * \param[in] void_flag flag for void function, 1 represent that interpret is in void function, 0 for non-void function.
 * \pre I_Htable!=NULL
 * \pre L!=NULL
 * \pre S!=NULL
 * \return token as result of expression, token->id tells how the whole process has been executed, token->id >= 0 -> no error, <0 or NULL -> error. 
 */
token *do_expression(token *postfix_array, stack_htab *I_Htable,struct stack_expresion *S,Instr_List *L,int void_flag);



/** 
 * \brief Function adds tokens data or concatenate tokens data if one of operands is string.
 * \param[in] a is first operand. 
 * \param[in] b is second operand.
 * \return token, token->ptr is pointer to result of addition / concatenate of two tokens data. token->id >= 0 -> no error, <0 or NULL -> error.
 */
token *inter_plus(token a,token b);


/** 
 * \brief Function does aritmetic operation beetween two tokens data.
 * \param[in] tmp1 is first operand.
 * \param[in] tmp2 is second operand.
 * \param[in] i stand for type of aritmetic operation. 1 minus, 2 multiply, 3 div
 * \pre 0 < i < 4
 * \return token, token->ptr is pointer to result of aritmetic operation of two tokens data. token->id >= 0 -> no error, <0 or NULL -> error.
 */
token *inter_arm_op(token tmp1,token tmp2, int i);



/** 
 * \brief Function does boolean operation beetween two tokens data.
 * \param[in] tmp1 is first operand.
 * \param[in] tmp2 is second operand.
 * \pre 0 < i < 8
 * \param[in] i stand for type of aritmetic operation. 1-> ==, 2-> !=, 3-> >=, 4-> >, 5-> <=, 6-> <, 7-> !(only for boolean)
 * \return token, token->ptr is pointer to result of boolean operation of two tokens data. token->id >= 0 -> no error, <0 or NULL -> error.
 */
token *inter_bool_op(token tmp1,token tmp2, int i);


/** 
 * \brief Function does boolean operation beetween two tokens data.
 * \param[in] L pointer to instruction tape.
 * \param[in] I_Htable pointer to stack of tables.
 * \param[out] return_token is result of function on instruction tape.  Used if function is called by indirect recursion from do_expression.
 * \param[in] void_flag flag for void function, 1 represent that interpret is in void function, 0 for non-void function.
 * \pre I_Htable!=NULL;
 * \pre return_token!=NULL;
 * \return integer value which tells, how the whole processed has been executed, 0 -> no error, !=0 -> error.
 */
int inter(Instr_List *L, stack_htab *I_Htable,token *return_token, int void_flag);


/** 
 * \brief Function translates integer to string.
 * \param[in] x integer that is going to be converted.
 * \return converted string, NULL for error.
 */
char *IntToString(int x);



/** 
 * \brief Function translates double to string.
 * \param[in] x double that is going to be converted.
 * \return converted string, NULL for error.
 */
char *DoubleToString(double x);

/** 
 * \brief Function concatenates two strings.
 * \param[in] s1 first operand.
 * \param[in] s2 second operand. 
 * \return concatenated string, NULL for error.
 */
char *Conc_Str(char *s1, char *s2);


/**
 * Search for item of local or global hash table in the stack
 * @param  stack	stack where item will be searched
 * @param  key		name of variable or function which will be searched
 * @return			pointer to item of hash. table where searched thing is, or NULL if the search was not successful
 * @pre				stack has been inicializated
 */
htab_item * stack_htab_find_htab_item(stack_htab * stack, char * key);

extern htab_t *stack_htab_get_first(stack_htab *stack);


void I_Instr_null_elements(I_Instr * Instruction);

#endif
