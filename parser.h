#ifndef PARSER
#define PARSER

#include "scanner.h"
#include "ial.h"
#include "structures.h"
#include "interpret.h"
#include <string.h>
#include <stdbool.h>    // bool

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

// it eats tokens and says if syntax analysis success or not
int parser();

int analysis(stack_int_t *s, unsigned runtime, stack_htab Stack_of_TableSymbols, Instr_List * InstructionTape);

int skip_expr(token * t);

bool token_wanted(token * t);

int embedded_functions_into_hash_table(htab_t * global_table_symbols, array_string* all_class_names);

#endif
