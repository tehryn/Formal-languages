#ifndef SCANNER
#define SCANNER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "error.h"

/*! TODO */
#define ERR_WRONG_COMMENT_SYNTAX -100

/*! TODO */
#define ERR_FSEEK -101

/*! TODO */
#define S_SIZE 32

/*! TODO */
enum {
S_BOOLEAN = 1, /*!< Keyword boolean */
S_BREAK, /*!< Keyword break */
S_CLASS, /*!< Keyword class */
S_CONTINUE, /*!< Keyword continue */
S_DO, /*!< Keyword do */
S_DOUBLE, /*!< Keyword double */
S_ELSE, /*!< Keyword else */
S_FALSE, /*!< Keyword false */
S_FOR, /*!< Keyword for */
S_IF, /*!< Keyword if */
S_INT, /*!< Keyword int */
S_RETURN, /*!< Keyword return */
S_STRING, /*!< Keyword String */
S_STATIC, /*!< Keyword static */
S_TRUE, /*!< Keyword true */
S_VOID, /*!< Keyword void */
S_WHILE, /*!< Keyword while */

TYPE_DOUBLE, /*!< data type double */
TYPE_INT, /*!< data type int */
TYPE_STRING, /*!< data type String */
TYPE_BOOLEAN, /*!< data type boolean */
TYPE_STATIC_DOUBLE, /*!< data type static double */
TYPE_STATIC_INT, /*!< data type static int */
TYPE_STATIC_STRING, /*!< data type static String */
TYPE_STATIC_BOOLEAN, /*!< data type static boolean */
// TODO [static]? void

BLOCK_COMMENT, /*!< identifikator of block comment */
LINE_COMMENT, /*!< identifikator of one line comment */

S_SIMPLE_IDENT, /*!< stands for simple identifikator */
S_FULL_IDENT, /*!< stands for full identifikator */

S_EQUAL, /*!< stands for == */
S_LESS_EQUAL, /*!< stands for <= */
S_GREATER_EQUAL, /*!< stands for >= */
S_NOT_EQUAL, /*!< stands for != */
S_LEFT_PARE, /*!< stands for ( */
S_RIGHT_PARE, /*!< stands for ) */
S_LEFT_BRACE, /*!< stands for { */
S_RIGHT_BRACE, /*!< stands for } */
S_COMMA, /*!< stands for , */
S_SEMICOMMA, /*!< stands for ; */
S_PLUS, /*!< stands for + */
S_MINUS, /*!< stands for - */
S_DIV, /*!< stands for / */
S_MUL, /*!< stands for * */
S_ASSIGNMENT, /*!< stands for = */
S_EOF /*!< stands for EOF */
};

/** @var TODO*/
extern unsigned LINE_NUM;

/** @var TODO*/
extern int ERROR_CHECK;

/** @var TODO*/
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

/** Detect if input char represents some of special chars like =, +, ;, ..., also detect if there is >=, ==, != ot <= in file
@param c input char
@pre global variable f is already opened file
@return if input is special char, return its value (set by enum) otherwise return 0
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

/** Retrive token from source code
@pre global variable f is already opened file
@post token.id > 0 ( 0 in case of lexical error, otherwise error while setting offset or allocating memory)
@return token, where token.id is identifikator and token.ptr is string (or poiter to NULL if string is not needed)
*/
token get_token();

/** Set offset of file at begining
@return In case of error, return -1
@post return value is not -1
*/
int reset();
#endif
