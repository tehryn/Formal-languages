#include "scanner.h"
#include "stack_int.h"
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
  P_FUNC_BODY,
  P_VAR_EXPR,
  P_GUIDANCE,
  P_USE_ARGUMENTS,
  P_USE_ARGUMENTS2,
  P_RETURN_EXPR,
  P_ELSE_EXISTANCE,
  P_IF_ELSE_SECTION,
  P_FUNC_BODY_H1,
  P_FUNC_BODY_H2,
//  P_BOOL_TYPE,
  P_EXPR
};

// it eats tokens and says if syntax analysis success or not
int parser();

int analysis(stack_int_t *s);

int skip_expr(token * t);

bool token_wanted(token * t);