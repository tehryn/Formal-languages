#include "scanner.h"
#include "stack_int.h"

enum {
  P_START = 1,
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
  P_BOOL_TYPE,
  P_EXPR
};

int analyze_syntax();
int process_token(token t, stack_int_t *s);
