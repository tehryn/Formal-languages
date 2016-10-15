#ifndef SCANNER
#define SCANNER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define BOOLEAN 1000
#define BREAK 1001
#define CLASS 1002
#define CONTINUE 1003
#define DO 1004
#define DOUBLE 1005
#define ELSE 1006 
#define FALSE 1007
#define FOR 1008
#define IF 1009
#define INT 1010
#define RETURN 1011
#define STRING 1012
#define STATIC 1013
#define TRUE 1014
#define VOID 1015
#define WHILE 1016

#define TYPE_DOUBLE 1100
#define TYPE_INT 1101

#define LINE_COMMENT 1200
#define BLOCK_COMMENT 1201

int is_key_word(char *word);
int is_num_literal(char *word, unsigned len);
// int is_str_literal(char *word, unsigned len); // TODO - bude si to hlidat pro cteni souboru?
int is_simple_ident(char *word, unsigned len);
int is_full_ident(char *word, unsigned len);
int skip_comment (int comment_type, FILE *f);
#endif
