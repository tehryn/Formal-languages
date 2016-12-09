// hlavicka pro interpret
#ifndef interpret
#define interpret

#include <string.h>
#include <stdbool.h>    // bool
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "structures.h"
#include "expression.h"

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

int Add_Instr(Instr_List *L, I_Instr *new);

token *do_expression(token *postfix_array, stack_htab *I_Htable,struct stack_expresion *S,Instr_List *L,int fce_flag);

token *inter_plus(token a,token b);
token *inter_arm_op(token tmp1,token tmp2, int i);
token *inter_bool_op(token tmp1,token tmp2, int i);
int inter(Instr_List *L, stack_htab *I_Htable,token *return_token);

char *IntToString(int x);
char *DoubleToString(double x);
char *Conc_Str(char *s1, char *s2);

int is_emb_fce(htab_item *item_tmp1,struct stack_expresion *S,token *return_token);

extern htab_t *stack_htab_get_first(stack_htab *stack);


void I_Instr_null_elements(I_Instr * Instruction);

#endif
