// hlavicka pro interpret
#ifndef INTERPRETER
#define INTERPRETER

#include "parser.h"
#include "ial.h"
#include "structures.h"
#include <string.h>
#include <stdbool.h>    // bool
#include <stdio.h>


enum {
	I_ASSIGMENT = P_EXPR + 1,	//69
	I_IF,						//70
	I_WHILE,					//71
	I_FCE,						//72
};

int inter(tSymbolTable *ST, tListOfInstr *instrList,token **postfix_token_array);

char *IntToString(int x);
char *DoubleToString(double x);
char *Conc_Str(char *s1, char *s2);
