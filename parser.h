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
  P_START = S_EOF + 1,  //50
  P_CLASS,              //51
  P_CLASS_BODY,         //52
  P_TYPE,               //53
  P_DEF,                //54
  P_DEF_ARGUMENTS,      //55
  P_DEF_ARGUMENTS2,     //56
  P_IDENT,              //57
  P_FUNC,               //58
  P_FUNC_BODY,          //59
  P_VAR_EXPR,           //60
  P_GUIDANCE,           //61
  P_RETURN_EXPR,        //62
  P_ELSE_EXISTANCE,     //63
  P_IF_ELSE_SECTION,    //64
  P_FUNC_BODY_H1,       //65
  P_EXPR,               //66
  P_I_END,               //67
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
