#include "parser.h"

extern int expr_analyze(token * t, stack_int_t *s); // potrebuji, aby mi zmenila token, muze pouzivat stack

int analyze_syntax()
{
	token t;
	stack_int_t s;

	if (stack_int_create(&s, 64) < 0)
	{
		fprintf(stderr, "Memory allocation of stack failed.\n");
		return -1;
	}

	if (stack_int_push(&s, 2, S_EOF, P_CLASS) < 0)
	{
		fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
		return -1;
	}

	do {
		t = get_token();
		if (t.id >= 0 && process_token(t, &s) == 0) {} // do nothing -> next cycle
		else
		{
			stack_int_destroy(&s);
			return -1;
		}
	} while (t.id != S_EOF);

	stack_int_destroy(&s);

	return 0;
}

int process_token (token t, stack_int_t *s)
// TODO !!! mlati se zde hodnoty enum stacku a parseru!!!  - opraveno???
{
	int on_top;

	while (!stack_int_is_empty(s))
	{
		if (stack_int_top(s, &on_top) < 0) // item on top of the stack into on_top
		{
			// TODO intern imposible fault
			// return -1 ??
		}

		switch (on_top)
		{
			case S_ASSIGNMENT:
				stack_int_pop(s);

				if (t.id == S_ASSIGNMENT) // '='
					return 0;

				fprintf(stderr, "PARSER: On line %u expected assignment.\n", LINE_NUM);
				return -1;


			case S_FULL_IDENT:
				stack_int_pop(s);

				if (t.id == S_FULL_IDENT)
					return 0;

				fprintf(stderr, "PARSER: On line %u expected full identifikator.\n", LINE_NUM);
				return -1;


			case S_LEFT_BRACE:
				stack_int_pop(s);

				if (t.id == S_LEFT_BRACE) // '{'
					return 0;

				fprintf(stderr, "PARSER: On line %u expected '{'.\n", LINE_NUM);
				return -1;


			case S_LEFT_PARE:
				stack_int_pop(s);

				if (t.id == S_LEFT_PARE) // '('
					return 0;

				fprintf(stderr, "PARSER: On line %u expected '('.\n", LINE_NUM);
				return -1;


			case S_RIGHT_BRACE:
				stack_int_pop(s);

				if (t.id == S_RIGHT_BRACE) // '}'
					return 0;

				fprintf(stderr, "PARSER: On line %u expected '}'.\n", LINE_NUM);
				return -1;


			case S_RIGHT_PARE:
				stack_int_pop(s);

				if (t.id == S_RIGHT_PARE) // ')'
					return 0;

				fprintf(stderr, "PARSER: On line %u expected ')'.\n", LINE_NUM);
				return -1;


			case S_SEMICOMMA:
				stack_int_pop(s);

				if (t.id == S_SEMICOMMA) // ';'
					return 0;

				fprintf(stderr, "PARSER: On line %u expected ';'.\n", LINE_NUM);
				return -1;


			case S_SIMPLE_IDENT:
				stack_int_pop(s);

				if (t.id == S_SIMPLE_IDENT)
					return 0;

				fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
				return -1;


			// If uncomment, push P_START again on stack (not sure if would still work)
			/*
			case P_START:
				stack_int_pop(s);

				if (t.id != S_CLASS)
				{
					fprintf(stderr, "On line %u expected 'class'\n", LINE_NUM);
					return -1;
				}

				if(stack_int_push(s, 1, P_CLASS) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				return 0;
			*/

			case P_CLASS:
				stack_int_pop(s);

				if (t.id == S_EOF) // The end of the input file
					return 0;

				if (t.id == S_CLASS)
				{
					if (stack_int_push(s, 5, P_CLASS, S_RIGHT_BRACE, P_CLASS_BODY, S_LEFT_BRACE, S_SIMPLE_IDENT) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				fprintf(stderr, "PARSER: On line %u expected 'class'.\n", LINE_NUM);
				return -1;


			case P_CLASS_BODY:
				stack_int_pop(s);

				if (t.id == S_RIGHT_BRACE) // '}' - no class body
				{
					break; // goto S_RIGHT_BRACE
				}

				if (t.id == S_STATIC)
				{
					if (stack_int_push(s, 4, P_CLASS_BODY, P_DEF, S_SIMPLE_IDENT, P_TYPE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				fprintf (stderr, "PARSER: On line %u expected static function, static variable definition or '}'", LINE_NUM);
				return -1;


			case P_TYPE:
				stack_int_pop(s);

				if (t.id == S_VOID)
					return 0;
				if (t.id == S_INT)
					return 0;
				if (t.id == S_DOUBLE)
					return 0;
				if (t.id == S_STRING)
					return 0;

				fprintf(stderr, "PARSER: On line %u expected data type.\n", LINE_NUM);
				return -1;


			case P_DEF:
				stack_int_pop(s);

				if (t.id == S_LEFT_PARE) // '('
				{
					if (stack_int_push(s, 5, S_RIGHT_BRACE, P_FUNC_BODY_H2, S_LEFT_BRACE, S_RIGHT_PARE, P_DEF_ARGUMENTS) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if (t.id == S_SEMICOMMA) // ';'
					return 0; // just stack_int_pop(s);

				if (t.id == S_ASSIGNMENT) // '='
				{
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				fprintf(stderr, "PARSER: On line %u expected definition of function, semicomma ';' or assignment to variable.\n", LINE_NUM);
				return -1;


			case P_DEF_ARGUMENTS:
				stack_int_pop(s);

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
					break; // goto case S_RIGHT_PARE

				if (stack_int_push(s, 3, P_DEF_ARGUMENTS2, S_SIMPLE_IDENT, P_TYPE) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_TYPE


			case P_DEF_ARGUMENTS2:
				stack_int_pop(s);

				if (t.id == S_RIGHT_PARE) // ')' - no other arguments
					break; // goto case S_RIGHT_PARE

				if (t.id == S_COMMA) // ',' - other arguments
				{
					if (stack_int_push(s, 3, P_DEF_ARGUMENTS2, S_SIMPLE_IDENT, P_TYPE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				fprintf(stderr, "PARSER: On line %u expected definition of arguments ('type' simple identifikator atc.).\n", LINE_NUM);
				return -1;


			case P_FUNC_BODY:
				stack_int_pop(s);

				if(t.id == S_SIMPLE_IDENT)
				{
					if (stack_int_push(s, 1, P_GUIDANCE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if(t.id == S_FULL_IDENT)
				{
					if (stack_int_push(s, 1, P_GUIDANCE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if(t.id == S_RETURN)
				{
					if (stack_int_push(s, 1, P_RETURN_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if(t.id == S_IF)
				{
					if (stack_int_push(s, 5, P_ELSE_EXISTANCE, P_IF_ELSE_SECTION, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if(t.id == S_WHILE)
				{
					if (stack_int_push(s, 6, S_RIGHT_BRACE, P_FUNC_BODY_H1, S_LEFT_BRACE, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if (stack_int_push(s, 3, P_VAR_EXPR, S_SIMPLE_IDENT, P_TYPE) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_TYPE


			case P_VAR_EXPR:
				stack_int_pop(s);

				if (t.id == S_SEMICOMMA) // ';'
					return 0; // just stack_int_pop(s);

				if (t.id == S_ASSIGNMENT) // '='
				{
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				fprintf(stderr, "PARSER: On line %u after variable must be ';' or assignment.\n", LINE_NUM);
				return -1;


			case P_GUIDANCE:
				stack_int_pop(s);

				if (t.id == S_LEFT_PARE) // '('
				{
					if (stack_int_push(s, 3, S_SEMICOMMA, S_RIGHT_PARE, P_USE_ARGUMENTS) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if (t.id == S_SEMICOMMA) // ';'
					return 0; // just stack_int_pop(s);

				if (t.id == S_ASSIGNMENT) // '='
				{
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				fprintf(stderr, "PARSER: On line %u expected '(', ';' or assignment.\n", LINE_NUM);
				return -1;


			case P_USE_ARGUMENTS:
				stack_int_pop(s);

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
					break; // goto case S_RIGHT_PARE

				if (stack_int_push(s, 2, P_USE_ARGUMENTS2, P_EXPR) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_EXPR


			case P_USE_ARGUMENTS2:
				stack_int_pop(s);

				if(t.id == S_COMMA)
				{
					if (stack_int_push(s, 2, P_USE_ARGUMENTS2, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if(t.id == S_RIGHT_PARE) // ')' - no other arguments
					break; // goto case S_RIGHT_PARE

				fprintf(stderr, "PARSER: On line %u expected ')' or ','.\n", LINE_NUM);
				return -1;


			case P_RETURN_EXPR:
				stack_int_pop(s);

				if (t.id == S_SEMICOMMA) // ';'
					return 0; // just stack_int_pop(s);

				if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_EXPR


			case P_ELSE_EXISTANCE:
				stack_int_pop(s);

				if(t.id == S_ELSE)
				{
					if (stack_int_push(s, 1, P_IF_ELSE_SECTION) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}
				else
					break; // else is not there


			case P_IF_ELSE_SECTION:
			stack_int_pop(s);

				if (t.id == S_LEFT_BRACE) // '{'
				{
					if (stack_int_push(s, 2, S_RIGHT_BRACE, P_FUNC_BODY_H1) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					return 0;
				}

				if (stack_int_push(s, 1, P_FUNC_BODY_H2) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_FUNC_BODY_H2


			case P_FUNC_BODY_H1:
				stack_int_pop(s);

				if (t.id == S_RIGHT_BRACE) // '}'
					break; // goto S_RIGHT_BRACE

				if (stack_int_push(s, 2, P_FUNC_BODY_H1, P_FUNC_BODY) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_FUNC_BODY_H2


			case P_FUNC_BODY_H2:
				stack_int_pop(s);

				if (stack_int_push(s, 1, P_FUNC_BODY) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;
				}
				break; // goto case P_FUNC_BODY_H2


			//case P_BOOL_TYPE: ??


			case P_EXPR:
				stack_int_pop(s);
				if (expr_analyze(&t, s) < 0)
					return -1;
				break;

			default:
				stack_int_pop(s);
				fprintf(stderr, "Intern fault. Unexpected situation.\n");
				return -1;
		}
	}
	return 0;
}
