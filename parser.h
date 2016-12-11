/**
* Author: Miroslava Misova <BR>
* Login: xmisov00 <BR>
* School: VUT FIT, BRNO <BR>
* Project: interpreter for IFJ16 <BR>
* gcc version: 5.4.0 (ubuntu 16.04.2) <BR>
* Date: 2016-12-11
**/
#ifndef PARSER
#define PARSER

#include "scanner.h"
#include "ial.h"
#include "structures.h"
#include "interpret.h"
#include <string.h>
#include <stdbool.h>    // bool

/** Error codes */
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

/** Default size of Hash table */
#define HTAB_SIZE 7

// cases in switch in funtion process_token
enum {
  P_START = S_EOF + 1,
  P_CLASS,
  P_CLASS_BODY,
  P_TYPE,
  P_DEF,
  P_DEF_ARGUMENTS,
  P_DEF_ARGUMENTS2,
  P_IDENT,
  P_FUNC,
  P_FUNC_BODY,
  P_VAR_EXPR,
  P_GUIDANCE,
  P_RETURN_EXPR,
  P_ELSE_EXISTANCE,
  P_IF_ELSE_SECTION,
  P_FUNC_BODY_H1,
  P_EXPR,
  P_I_ENDIF,
  P_I_ENDWHILE,
  P_I_ENDELSE
};

/**
 * Initialize stack, global hash table and alocate memory for global variables
 * @return Return code of whole program
 */
int parser();

/**
 * Detect syntax and semantic errors, also updates tables of symbols.
 * @param  s                     Stack for LL grammer
 * @param  runtime               Number of calls of this function
 * @param  Stack_of_TableSymbols Stack of hash tables
 * @param  InstructionTape       Instruction tape where will be instruction generated
 * @return                       0 on succes, otherwise some errror code
 */
int analysis(stack_int_t *s, unsigned runtime, stack_htab Stack_of_TableSymbols, Instr_List * InstructionTape);

/**
 * Skip expresions (used in first call of function analusis)
 * @param  t
 * @return   [description]
 */
int skip_expr(token * t);

/**
 * [token_wanted description]
 * @param  t First token in expresion
 * @return   In case of succes returns 0, otherwise return some error code
 */
bool token_wanted(token * t);

/**
 * Adds embended function into hash table
 * @param  global_table_symbols Hash table where functions will be stored
 * @param  all_class_names      Array with names of all functions
 * @return In case of succes returns 0, otherwise return some error code                     
 */
int embedded_functions_into_hash_table(htab_t * global_table_symbols, array_string* all_class_names);

#endif
