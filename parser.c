#include "parser.h"

int analyze_syntax() {
	token t;
	stack_int_t s;
	
	if (stack_int_create(&s, 64) < 0) {
		fprintf(stderr, "Memory allocation failed");
		return -1;
	}
	
	stack_int_push(&s, 2, S_EOF, P_START);
	
	do {
		t = get_token();
		if (t.id >= 0 && process_token(t, &s)==0) {} // do nothing -> next cycle
		else {
			stack_int_destroy(&s);
			return -1;
		}
	} while (t.id != S_EOF);
	
	return 0;
}

int process_token (token t, stack_int_t *s) {
	int on_top;
	while (42) {
		if (stack_int_top(s, &on_top) < 0) { // item on top of the stack into on_top
			// TODO what happend?
		}
	
		switch (on_top) {

			case P_START:
				stack_int_pop(s);

				if (t.id != S_CLASS) {
					fprintf(stderr, "On line %u expected 'class'\n", LINE_NUM);
					stack_int_push(s, 1, P_CLASS);
					return -1;
				}

				stack_int_push(s, 1, P_CLASS);
				return 0;


			case P_CLASS:
				stack_int_pop(s);

				if (t.id == S_EOF) {
					break;
				}
				else if (t.id == S_SIMPLE_IDENT) {
					stack_int_push(s, 4, P_CLASS, S_RIGHT_BRACE, P_CLASS_BODY, S_LEFT_BRACE);
					return 0;
				}

				stack_int_push(s, 4, P_CLASS, S_RIGHT_BRACE, P_CLASS_BODY, S_LEFT_BRACE);
				fprintf(stderr, "On line %u expected identifikator\n", LINE_NUM);
				return -1;

		}
	}
}
