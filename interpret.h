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
	I_ASSIGMENT = 68+1,	//69
	I_IF,						//70
	I_ELSE,						//71
	I_WHILE,					//72
	I_END, 						//73  end of IF || end of ELSE || end of WHILE
	I_FCE,						//74
	I_RETURN					//75
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

/**
 * Search for item of local or global hash table in the stack
 * @param  stack	stack where item will be searched
 * @param  key		name of variable or function which will be searched
 * @return			pointer to item of hash. table where searched thing is, or NULL if the search was not successful
 * @pre				stack has been inicializated
 */
htab_item * stack_htab_find_htab_item(stack_htab * stack, char * key)
{
	htab_t * Table = NULL;
	htab_item * TableItem = NULL;
	if (is_full_ident(key, strlen(key)))
	{
		Table = stack_htab_get_first(stack);
		if (Table == NULL)
			return NULL;
		TableItem = htab_find_item(Table, key);
		return TableItem; // could be NULL
	}
	else if (is_simple_ident(key, strlen(key)))
	{
		Table = stack_htab_get_item(stack, 0);
		if (Table == NULL)
			return NULL;
		TableItem = htab_find_item(Table, key);
		return TableItem; // could be NULL
	}
	return NULL;
}



#endif
