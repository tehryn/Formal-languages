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
#include "parser.h"

enum {
	I_ASSIGMENT = P_I_ENDELSE+1,//71
	I_IF,						//72
	I_ELSE,						//73
	I_WHILE,					//74
	I_END, 						//75  
	I_FCE,						//76
	I_RETURN,					//77
	I_PRINT,					//78
	I_ENDIF,					//79
	I_ENDWHILE,					//80
	I_ENDELSE					//81
};

typedef struct I_Instr
{
	int type_instr;
	void *adr1;
	void *adr2;
	void *adr3;
	struct I_Instr *next_instr;
}I_Instr;

typedef struct Instr_List{
	I_Instr *Active;
	I_Instr *Last;
}Instr_List;

int Add_Instr(Instr_List *L, I_Instr *new);


token *inter_plus(token a,token b);
token *inter_arm_op(token tmp1,token tmp2, int i);
token *inter_bool_op(token tmp1,token tmp2, int i);
int inter(Instr_List *L, stack_htab *I_Htable);

char *IntToString(int x);
char *DoubleToString(double x);
char *Conc_Str(char *s1, char *s2);
int is_emb_fce(htab_item *item_tmp1,token *postfix_array,token *return_token, stack_htab *I_Htable);
extern htab_t *stack_htab_get_first(stack_htab *stack);




#endif
