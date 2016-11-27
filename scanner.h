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
#define reset_scanner() (fseek(f, LINE_NUM = 0, SEEK_SET))

/*! TODO */
#define SPEC_CHAR_FSEEK(spec) (((spec) == S_EQUAL || (spec) == S_LESS_EQUAL || (spec) == S_GREATER_EQUAL || (spec) == S_NOT_EQUAL)?-2:-1)

/*! TODO */
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
TYPE_STATIC_DOUBLE, /*!< data type static double */ //22
TYPE_STATIC_INT, /*!< data type static int */ //23
TYPE_STATIC_STRING, /*!< data type static String */ //24
TYPE_STATIC_BOOLEAN, /*!< data type static boolean */ //25
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

/** @var TODO*/
extern unsigned LINE_NUM;

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
char *load_string(char *word, unsigned *max);

/**
 * Convert string into double or integer (depends on type variable) and store it into new allocated space
 * @param  str    String that represents number
 * @param  type   Type of number that represent string (should be TYPE_INT or TYPE_DOUBLE)
 * @param  valide Variable that will be set into 0 in case of succes, into 1 in case of error while allocating memory, into 2 in case of invalide string or into 3 in case of invalide type
 * @return        Pointer into value that is result of conversion
 */
void *str2num(char *str, int type, int *valide);

/** Retrive token from source code
@pre global variable f is already opened file
@post token.id > 0 ( 0 in case of lexical error, otherwise error while setting offset or allocating memory)
@return token, where token.id is identifikator and token.ptr is string (or poiter to NULL if string is not needed)
*/
token get_token();
#endif
