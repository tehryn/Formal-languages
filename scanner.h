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
#define TYPE_STRING 1102
#define TYPE_BOOLEAN 1103
#define TYPE_STATIC_DOUBLE 1104
#define TYPE_STATIC_INT 1105
#define TYPE_STATIC_STRING 1106
#define TYPE_STATIC_BOOLEAN 1107

#define LINE_COMMENT 1200
#define BLOCK_COMMENT 1201

#define S_EQUAL 1202
#define S_LESS_EQUAL 1203
#define S_GREATER_EQUAL 1204
#define S_NOT_EQUAL 1205

#define SIMPLE_IDENT 1300
#define FULL_IDENT 1301

#define ERR_REACHED_MAX -100
#define ERR_WRONG_COMMENT_SYNTAX -101
#define ERR_FSEEK -102

#define S_SIZE 16

extern unsigned LINE_NUM;
extern int ERROR_CHECK;
extern FILE* f;

/** Structure that represents token
@param id Id of token (Keyword, numeric constant, operator, ...)
@param ptr Pointer into data (value of identifikator, name of identifikator...) or NULL if data are not needed.
*/
typedef struct token {
	int id;
	void *ptr;
} token;

/** Detect if input String is key word or not
@param word String (or array of chars) for detection
@pre Word is ended by char '\0'
@return If word represents key word, return id of specific key word, otherwise return 0
*/
int is_keyword(char *word);

/**
TODO
*/
int is_special_char(char c);
/** Detect if input string is numeric literal or not
@param word String (or array of chars) for detection
@param len length of word (without '\0', if there is)
@pre size of allocated space for word is bigger or equal len
@return If word is numeric literal, return TYPE_INT for integer or TYPE_DOUBLE for double, otherwise return 0
*/
int is_num_literal(char *word, unsigned len);

/** Detect if input string is simple identifikator or not
@param word String (or array of chars) for detection
@param len length of word (without '\0', if there is)
@pre size of allocated space for word is bigger or equal len
@return 1 if word represents simple identifikator, otherwise return 0
*/
int is_simple_ident(char *word, unsigned len);

/** Detect if input string is full identifikator or not
@param word String (or array of chars) for detection
@param len length of word (without '\0', if there is)
@pre size of allocated space for word is bigger or equal len
@return 1 if word represents simple identifikator, otherwise return 0
*/
int is_full_ident(char *word, unsigned len);

/** Ignore all chars until end of comment
@param comment_type Type of comment (LINE_COMMENT or BLOCK_COMMENT)
@pre global variable f is already opened file or active stream
@return 0 when skipped comment, return 1 when comment was ended by EOF or return -1, if end of BLOCK_COMMENT was not found
*/
int skip_comment (unsigned comment_type);

/** Load chars until function reach end of string
@param word pointer to allocated space for saving chars from stream
@param max pointer to length of allocated space in bytes
@pre global variable f is already opened file
@pre word points to already allocated space
@pre *max >= 1
@return Loaded string, return sNULL when function reach EOF or returns NULL and set *max to zero, if reallocation fails
*/
char * load_string(char *word, unsigned *max);

/**
TODO
*/
token get_token();
#endif
