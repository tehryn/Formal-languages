/**
* Author: Matejka Jiri <BR>
* Login: xmatej52 <BR>
* School: VUT FIT, BRNO <BR>
* Project: Interpret for IFJ16 <BR>
* gcc version: 5.4.0 (ubuntu 16.04.2) <BR>
* Date: 2016-12-03
**/
#ifndef SCANNER
#define SCANNER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "garbage_collector.h"
/** @defgroup lexical_analysis Lexical analysis */
/** @addtogroup lexical_analysis
  * @brief Lexical analyse analyse input source code and check, if it is subset
  * of the language IFJ16. Also transfer input source code into tokens
  * @{
  */

/*! Default size for memory allocation */
#define S_SIZE 32

/*! Macro that set offset at the beginning of file */
#define reset_scanner() (fseek(f, LINE_NUM = 0, SEEK_SET))

/*! Macro that tells how much will be offset returned */
#define SPEC_CHAR_FSEEK(spec) (((spec) == S_EQUAL || (spec) == S_LESS_EQUAL || (spec) == S_GREATER_EQUAL || (spec) == S_NOT_EQUAL)?-2:-1)

/**
 * @enum Enumerator
 * @brief specifies Output of functions
 */
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
TYPE_INT_BIN,  /*!< Integer written in binary */
TYPE_INT_OCTAL,   /*!< Integer written in octal */
TYPE_INT_HEX,  /*!< Integer written in hex */
TYPE_DOUBLE_HEX,  /*!< Double written in hex */

BLOCK_COMMENT, /*!< identifikator of block comment */
LINE_COMMENT, /*!< identifikator of one line comment */

S_SIMPLE_IDENT, /*!< stands for simple identifikator */
S_FULL_IDENT, /*!< stands for full identifikator */

S_EQUAL, /*!< stands for == */
S_LESS_EQUAL, /*!< stands for <= */
S_GREATER_EQUAL, /*!< stands for >= */
S_LESS, /*!< stands for < */
S_GREATER, /*!< stands for > */
S_OR, /*!< stands for || */
S_AND, /*!< stands for && */
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

/** @var Global variable for counting lines (used for error messages) */
extern unsigned LINE_NUM;

/** @var Global variable that stands for input source file */
extern FILE* f;

/**
 * Structure that represents token
 */
typedef struct token {
	/** Id of token (Keyword, numeric constant, operator, ...) */
	int id;
	/** Pointer into data (value of identifikator, name of identifikator...) or NULL if data are not needed. */
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
@return 1 if word represents full identifikator, otherwise return 0
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
@return Loaded string, returns NULL when function reach EOF or EOL and set max to -1 or returns NULL and set *max to zero, if reallocation fails or return NULL and set max to -2 if there is invalid use of escape sequence
*/
char *load_string(char *word, int *max);

/**
 * Count power
 * @param  x   Cardinal number
 * @param  exp Exponent
 * @return     Result of x to the exponent
 */
double make_power (double x, long int exp);

/**
 * Convert string to decimal if string represents binnary integer number
 * @param str    String for conversion
 * @param result Converted number
 */
void bin2dec(char *str, int *result);

/**
 * Convert string to decimal number if string represents octal integer number
 * @param str    String for conversion
 * @param result Converted number
 */
void octal2dec(char *str, int *result);

/**
 * Convert string to decimal number if string represents hexadecimal integer number
 * @param str    String for conversion
 * @param result Converted number
 */
void hex2dec_int(char *str, int *result);

/**
 * Convert string to decimal number if string represents hexadecimal floating point number
 * @param str    String for conversion
 * @param result Converted number
 */
void hex2dec_double(char *str, double *result);

/**
 * Remove '_' from string
 * @param str String what will be changed
 */
void repair_num(char *str);

/**
 * Convert string into double or integer (depends on type variable) and store it into new allocated space
 * @param  str    String that represents number
 * @param  type   Type of number that represent string (should be TYPE_INT or TYPE_DOUBLE)
 * @param  valid Variable that will be set into 0 in case of success, into 1 in case of error while allocating memory, into 2 in case of invalid string or into 3 in case of invalid type
 * @return        Pointer into value that is result of conversion
 */
void *str2num(char *str, int type, int *valid);

/** Retrieve token from source code
@pre global variable f is already opened file
@post token.id > 0 ( 0 in case of lexical error, otherwise error while setting offset or allocating memory)
@return token, where token.id is identifikator and token.ptr is string (or pointer to NULL if string is not needed)
*/
token get_token();

/** @} */
#endif
