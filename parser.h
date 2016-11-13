#include "scanner.h"
#include "stack_int.h"

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
int analyze_syntax();

int process_token(token t, stack_int_t *s);