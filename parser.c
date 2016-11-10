#include "parser.h"

int analyze_syntax() {
	token t;
	stack_int_t s;

	if (stack_int_create(&s, 64) < 0) {
		fprintf(stderr, "Memory allocation failed");
		return -1;
	}

	stack_int_push(&s, 2, P_CLASS, S_EOF);

	do {
		t = get_token();
		if (t.id >= 0 && process_token(t, &s) == 0) {} // do nothing -> next cycle
		else {
			stack_int_destroy(&s);
			return -1;
		}
	} while (t.id != S_EOF);
	stack_int_destroy(&s);
	return 0;
}

int process_token (token t, stack_int_t *s) {
	int on_top;
	while (!stack_int_is_empty(s)) {
		if (stack_int_top(s, &on_top) < 0) { // item on top of the stack into on_top
			// TODO what happend?
		}

		switch (on_top) {
// If uncomment, push P_START again on stack
/*			case P_START:
				stack_int_pop(s);

				if (t.id != S_CLASS) {
					fprintf(stderr, "On line %u expected 'class'\n", LINE_NUM);
					return -1;
				}

				stack_int_push(s, 1, P_CLASS);
				return 0;
*/
			case P_CLASS:
				stack_int_pop(s);

				if (t.id == S_EOF) {
					return 0;
				}
				else if (t.id == S_SIMPLE_IDENT) {
					stack_int_push(s, 4, S_LEFT_BRACE, P_CLASS_BODY, S_RIGHT_BRACE, P_CLASS);
					return 0;
				}
				fprintf(stderr, "On line %u expected identifikator\n", LINE_NUM);
				return -1;
			case P_CLASS_BODY:
				if (t.id == S_RIGHT_BRACE) {
					return 0;
				}
				if (t.id == S_STATIC) {
					stack_int_push(s, 4, P_TYPE, S_SIMPLE_IDENT, P_DEF, P_CLASS_BODY);
					return 0;
				}
				fprintf (stderr, "On line %u expected static function or static variable definition or '}'", LINE_NUM);
				return -1;
			case P_TYPE:
				if (t.id == S_VOID) {

				}
				if (t.id == S_INT) {

				}
				if (t.id == S_DOUBLE) {

				}
				if (t.id == S_STRING) {
					
				}
			case P_DEF:
			case P_DEF_ARGUMENTS:
			case P_DEF_ARGUMENTS2:
			case P_IDENT:
			case P_FUNC_BODY:
			case P_VAR_EXPR:
			case P_GUIDANCE:
			case P_USE_ARGUMENTS:
			case P_USE_ARGUMENTS2:
			case P_RETURN_EXPR:
			case P_ELSE_EXISTANCE:
			case P_IF_ELSE_SECTION:
			case P_FUNC_BODY_H1:
			case P_FUNC_BODY_H2:
			case P_BOOL_TYPE:
			case P_EXPR: break;
		}
	}
	return 0;
}
