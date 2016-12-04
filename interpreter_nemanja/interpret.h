// hlavicka pro interpret
#ifndef INTeRPRET
#include <string.h>
#include <stdbool.h>    // bool
#include <stdio.h>
#include <stdlib.h>
#include "help.h"
#define I_STACKSIZE 30
#define LINE_NUM 1


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


enum {
S_BOOLEAN = 1, /*!< Keyword boolean */ //1
S_BREAK, /*!< Keyword break */ //2
S_CLASS, /*!< Keyword class */ //3
S_CONTINUE, /*!< Keyword continue */ //4
S_DO, /*!< Keyword do */ //5
S_DOUBLE, /*!< Keyword double */ //6
S_ELSE, /*!< Keyword else */ //7
S_FALSE, /*!< Keyword false */ //8
S_FOR, /*!< Keyword for */ //9
S_IF, /*!< Keyword if */ //10
S_INT, /*!< Keyword int */ //11
S_RETURN, /*!< Keyword return */ //12
S_STRING, /*!< Keyword String */ //13
S_STATIC, /*!< Keyword static */ //14
S_TRUE, /*!< Keyword true */ //15
S_VOID, /*!< Keyword void */ //16
S_WHILE, /*!< Keyword while */ //17

TYPE_DOUBLE, /*!< data type double */ //18
TYPE_INT, /*!< data type int */ //19
TYPE_STRING, /*!< data type String */ //20
TYPE_BOOLEAN, /*!< data type boolean */ //21
TYPE_INT_BIN, //22 TODO
TYPE_INT_OCTAL,  //23 TODO
TYPE_INT_HEX, //24 TODO
TYPE_DOUBLE_HEX, //25 TODO
// TODO [static]? void

BLOCK_COMMENT, /*!< identifikator of block comment */ //26
LINE_COMMENT, /*!< identifikator of one line comment */ //27

S_SIMPLE_IDENT, /*!< stands for simple identifikator */ //28
S_FULL_IDENT, /*!< stands for full identifikator */ //29

S_EQUAL, /*!< stands for == */ //30
S_LESS_EQUAL, /*!< stands for <= */ //31
S_GREATER_EQUAL, /*!< stands for >= */ //32
S_LESS, /*!< stands for < */ //33
S_GREATER, /*!< stands for > */ //34
S_OR, /*!< stands for || */ //35
S_AND, /*!< stands for && */ //36
S_NOT_EQUAL, /*!< stands for != */ //37
S_LEFT_PARE, /*!< stands for ( */ //38
S_RIGHT_PARE, /*!< stands for ) */ //39
S_LEFT_BRACE, /*!< stands for { */ //40
S_RIGHT_BRACE, /*!< stands for } */ //41
S_COMMA, /*!< stands for , */ //42
S_SEMICOMMA, /*!< stands for ; */ //43
S_PLUS, /*!< stands for + */ //44
S_MINUS, /*!< stands for - */ //45
S_DIV, /*!< stands for / */ //46
S_MUL, /*!< stands for * */ //47
S_ASSIGNMENT, /*!< stands for = */ //48
S_EOF /*!< stands for EOF */ //49
};








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




int inter(Instr_List *L,htab_t *I_Htable);

char *IntToString(int x);
char *DoubleToString(double x);
char *Conc_Str(char *s1, char *s2);
token *Create_New_token(int id, htab_t *T, int key);

#endif
