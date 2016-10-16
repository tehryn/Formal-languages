#ifndef SCANNER
#define SCANNER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "error.h"

#define S_BOOLEAN 1000
#define S_BREAK 1001
#define S_CLASS 1002
#define S_CONTINUE 1003
#define S_DO 1004
#define S_DOUBLE 1005
#define S_ELSE 1006 
#define S_FALSE 1007
#define S_FOR 1008
#define S_IF 1009
#define S_INT 1010
#define S_RETURN 1011
#define S_STRING 1012
#define S_STATIC 1013
#define S_TRUE 1014
#define S_VOID 1015
#define S_WHILE 1016

#define TYPE_DOUBLE 1100
#define TYPE_INT 1101

#define LINE_COMMENT 99998
#define BLOCK_COMMENT 99999

int get_meaning(char *word);
int is_num_literal(char *word, unsigned len);
// int is_str_literal(char *word, unsigned len); // TODO - bude si to hlidat pro cteni souboru?
int is_simple_ident(char *word, unsigned len);
int is_full_ident(char *word, unsigned len);
int skip_comment (int comment_type, FILE *f);
unsigned load_string(FILE *f, char *word, unsigned max);
#endif
