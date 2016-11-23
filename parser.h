#ifndef PARSER
#define PARSER

#include "scanner.h"
#include "stack_int.h"
#include "htab.h"
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
  P_START = S_EOF + 1, //50
  P_CLASS, //51
  P_CLASS_BODY, //52
  P_TYPE, //53
  P_DEF, //54
  P_DEF_ARGUMENTS, //55
  P_DEF_ARGUMENTS2, //56
  P_IDENT, //57
  P_FUNC,
  P_FUNC_BODY, //58
  P_VAR_EXPR, //59
  P_GUIDANCE, //60
  P_USE_ARGUMENTS, //61
  P_USE_ARGUMENTS2, //62
  P_RETURN_EXPR, //63
  P_ELSE_EXISTANCE, //64
  P_IF_ELSE_SECTION, //65
  P_FUNC_BODY_H1, //66
  P_FUNC_BODY_H2, //67
//  P_BOOL_TYPE,
  P_EXPR //68
};

// it eats tokens and says if syntax analysis success or not
int parser();

int analysis(stack_int_t *s);

int skip_expr(token * t);

bool token_wanted(token * t);
#endif
