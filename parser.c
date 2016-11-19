#include "parser.h"
#include <stdbool.h>    // bool
#include <string.h>

extern int expr_analyze(token * t, stack_int_t *s); // potrebuji, aby mi zmenila token, muze pouzivat stack

int parser()
{
	stack_int_t s;

	int parser_return;

	if (stack_int_create(&s, 64) < 0)
	{
		fprintf(stderr, "Intern fault. Memory allocation of stack failed.\n");
		return ERR_INTERN_FAULT;
	}

	if (stack_int_push(&s, 2, S_EOF, P_CLASS) < 0)
	{
		fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
		return ERR_INTERN_FAULT;
	}

	parser_return = analysis(&s);

	stack_int_destroy(&s);
	return parser_return;
}

int analysis (stack_int_t *s)
{
	token t;
	int on_top;
	bool main_existance = false; // if class Main exists in whole input file
	bool main_run_existance = false; // if function run exist in function Main
	char * class_name = NULL; // TODO - pro pripad, ze budu chtit ukladat nazvy trid - zatim neudelano

	int type = 0; // t.id should not be 0
	bool void_existance = false; // if true - control no existance of void variable and return with no expr. in void function
	bool token_got = false; // variable for P_EXPR - if the token have been read

	while (!stack_int_top(s, &on_top)) // stack_int_top == -1 if stack is empty
	{

		switch (on_top)
		{
			// ======================== P_CLASS ========================
			case P_CLASS:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_EOF) // The end of the input file and no other classes
				{
					token_got = false;
					stack_int_top(s, &on_top); // if should be S_EOF
					if (on_top != S_EOF)
						return ERR_SYNTACTIC_ANALYSIS;

					stack_int_pop(s);
					if (main_existance && main_run_existance) // everything is perfect, i can leave the function
						return 0;
					else
					{
						fprintf(stderr, "Semantic fauilt. Class Main does not exist or it exists but has no function run.\n");
						return ERR_SEM_NDEF_REDEF;
					}
				}

				if (t.id == S_CLASS)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
					token_got = true;

					if(t.id == S_SIMPLE_IDENT)
					{
						token_got = false;
						// make a copy of class name
						if (class_name != NULL)	// TODO ? - just realloc
							free(class_name);
						int class_name_strlen = strlen((char*) t.ptr) + 1; // '\0'
						class_name = malloc(class_name_strlen); // * sizeof(char) = 1
						if (class_name == NULL)
						{
							fprintf(stderr, "Intern fault. Memory allocation of class_name failed.\n");
							return ERR_INTERN_FAULT;
						}
						class_name = strncpy(class_name, (char*)t.ptr, class_name_strlen);

						if (strcmp(class_name,"Main") == 0)
							main_existance = true;

						// TODO - pridat class_name do tabulky symbolu pro tridy
						if(token_got == false)
						{
							t = get_token();
							if (t.id <= 0)
								return ERR_LEXICAL_ANALYSIS;
						}
						token_got = true;

						if (t.id == S_LEFT_BRACE)
						{
							token_got = false;
							if (stack_int_push(s, 3, P_CLASS, S_RIGHT_BRACE, P_CLASS_BODY) < 0)
							{
								fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
								return ERR_INTERN_FAULT;
							}
							break;
						}

						fprintf(stderr, "PARSER: On line %u expected '{'.\n", LINE_NUM);
						return ERR_SYNTACTIC_ANALYSIS;
					}
					fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				fprintf(stderr, "PARSER: On line %u expected 'class'.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_CLASS_BODY ========================

			case P_CLASS_BODY:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_RIGHT_BRACE) // scanner: '}' -> no class body
				{
					token_got = false;
					stack_int_top(s, &on_top);
					if (on_top != S_RIGHT_BRACE)
					{
						fprintf(stderr, "PARSER: On line %u read '{' but it should not be there.\n", LINE_NUM);
						return ERR_SYNTACTIC_ANALYSIS;
					}
					stack_int_pop(s);
					break; // goto P_CLASS
				}

				if (t.id == S_STATIC)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
					token_got = true;

					if      (t.id == S_VOID)
					{
						type = S_VOID;
						void_existance = true;
					}
					else if (t.id == S_INT)
					{
						type = S_INT;
						void_existance = false;
					}
					else if (t.id == S_DOUBLE)
					{
						type = S_DOUBLE;
						void_existance = false;
					}
					else if (t.id == S_STRING)
					{
						type = S_STRING;
						void_existance = false;
					}
					else if (t.id == S_BOOLEAN)
					{
						type = S_BOOLEAN;
						void_existance = false;
					}
					else
					{
						fprintf (stderr, "PARSER: On line %u expected some data type.\n", LINE_NUM);
						return ERR_SYNTACTIC_ANALYSIS;
					}

					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
					token_got = true;

					if (t.id == S_SIMPLE_IDENT)
					{
						// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
						// data_type = type;
						token_got = false;
						if (stack_int_push(s, 2, P_CLASS_BODY, P_DEF) < 0)
						{
							fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
							return ERR_INTERN_FAULT;
						}

						break;
					}
					fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;

				fprintf (stderr, "PARSER: On line %u expected static function, static variable definition or '}'\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_DEF ========================

			case P_DEF:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_LEFT_PARE) // scanner: '(' - function
				{
					// TODO ? - ukladat nejake dalsi informace do tabulky?
					token_got = false;
					if (stack_int_push(s, 5, S_RIGHT_BRACE, P_FUNC_BODY_H2, S_LEFT_BRACE, S_RIGHT_PARE, P_DEF_ARGUMENTS) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break;
				}

				if (t.id == S_SEMICOMMA) // scanner: ';' -> just variable without inicialisation
				{
					// TODO ? - ukladat nejake dalsi informace do tabulky?
					token_got = false;
					break; // goto P_CLASS_BODY
				}

				if (t.id == S_ASSIGNMENT) // scanner: '=' -> variable with inicialization
				{
					// TODO ? - ukladat nejake dalsi informace do tabulky?
					// TODO - porovnani typu expr a typu promenne
					token_got = false;
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto P_EXPR
				}

				fprintf(stderr, "PARSER: On line %u expected definition of function, semicomma ';' or assignment to variable.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_DEF_ARGUMENTS ========================

			case P_DEF_ARGUMENTS:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
				{
					token_got = false;
					stack_int_pop(s);
					break; // goto case S_RIGHT_PARE
				}

				if (t.id == S_INT)
					type = S_INT;
				else if (t.id == S_DOUBLE)
					type = S_DOUBLE;
				else if (t.id == S_STRING)
					type = S_STRING;
				else if (t.id == S_BOOLEAN)
					type = S_BOOLEAN;
				else
				{
					fprintf (stderr, "PARSER: On line %u expected some data type.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;
				token_got = true;

				if (t.id == S_SIMPLE_IDENT)
				{
					// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
					// data_type = type;
					token_got = false;
					if (stack_int_push(s, 1, P_DEF_ARGUMENTS2) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}

					break;
				}
				fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;


			// ======================== P_DEF_ARGUMENTS2 ========================

			case P_DEF_ARGUMENTS2:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')' - no other arguments
				{
					token_got = false;
					stack_int_pop(s);
					break; // goto case S_RIGHT_PARE
				}
				else if (t.id != S_COMMA) // ',' - other arguments
				{
					token_got = false;
					fprintf(stderr, "PARSER: On line %u expected ')' or ','.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;
				token_got = true;

				if      (t.id == S_INT)
					type = S_INT;
				else if (t.id == S_DOUBLE)
					type = S_DOUBLE;
				else if (t.id == S_STRING)
					type = S_STRING;
				else if (t.id == S_BOOLEAN)
					type = S_BOOLEAN;
				else
				{
					fprintf (stderr, "PARSER: On line %u expected some data type.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;
				token_got = true;

				if (t.id == S_SIMPLE_IDENT)
				{
					// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
					// data_type = type;
					token_got = false;
					if (stack_int_push(s, 1, P_DEF_ARGUMENTS2) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}

					break;
				}
				fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;


			// ======================== P_FUNC_BODY ========================

			case P_FUNC_BODY:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if(t.id == S_SIMPLE_IDENT)
				{
					token_got = false;
					if (stack_int_push(s, 1, P_GUIDANCE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_GUIDANCE
				}

				if(t.id == S_FULL_IDENT)
				{
					token_got = false;
					if (stack_int_push(s, 1, P_GUIDANCE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_GUIDANCE
				}

				if(t.id == S_RETURN)
				{
					token_got = false;
					if (stack_int_push(s, 1, P_RETURN_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_RETURN_EXPR
				}

				if(t.id == S_IF)
				{
					token_got = false;
					if (stack_int_push(s, 5, P_ELSE_EXISTANCE, P_IF_ELSE_SECTION, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_LEFT_PARE
				}

				if(t.id == S_WHILE)
				{
					token_got = false;
					if (stack_int_push(s, 6, S_RIGHT_BRACE, P_FUNC_BODY_H1, S_LEFT_BRACE, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_LEFT_PARE
				}

				// it must be definition of local variable (no other rules)

				if      (t.id == S_INT)
					type = S_INT;
				else if (t.id == S_DOUBLE)
					type = S_DOUBLE;
				else if (t.id == S_STRING)
					type = S_STRING;
				else if (t.id == S_BOOLEAN)
					type = S_BOOLEAN;
				else
				{
					fprintf (stderr, "PARSER: On line %u unexpected function body.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;
				token_got = true;

				if (t.id == S_SIMPLE_IDENT)
				{
					// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
					// data_type = type;
					token_got = false;
					if (stack_int_push(s, 1, P_VAR_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto P_VAR_EXPR
				}
				fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ------------------------ P_VAR_EXPR -------------------------------------

			case P_VAR_EXPR:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_SEMICOMMA) // ';'
				{
					token_got = false;
					break; // just stack_int_pop(s);
				}
				if (t.id == S_ASSIGNMENT) // '='  TODO - porovnani typu expr a typu promenne
				{
					token_got = false;
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_EXPR
				}

				fprintf(stderr, "PARSER: On line %u after variable must be ';' or assignment.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ------------------------- P_GUIDANCE -----------------------------------

			case P_GUIDANCE:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_LEFT_PARE) // '('
				{
					token_got = false;
					if (stack_int_push(s, 3, S_SEMICOMMA, S_RIGHT_PARE, P_USE_ARGUMENTS) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_USE_ARGUMENTS
				}

				if (t.id == S_SEMICOMMA) // ';'
				{
					token_got = false;
					break; // just stack_int_pop(s);
				}
				if (t.id == S_ASSIGNMENT) // '=' TODO - porovnani prirazeni (stejny datovy typ)
				{
					token_got = false;
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_EXPR
				}

				fprintf(stderr, "PARSER: On line %u expected '(', ';' or assignment.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// --------------------- P_USE_ARGUMENTS ----------------------------------

			case P_USE_ARGUMENTS:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
				{
					token_got = false;
					stack_int_pop(s);
					break; // just stack pop
				}

				if (stack_int_push(s, 2, P_USE_ARGUMENTS2, P_EXPR) < 0) // TODO - porovnani typu expr - pocotadlo argumentu?
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return ERR_INTERN_FAULT;
				}
				break; // goto case P_EXPR

			// ------------------- P_USE_ARGUMENTS2 ------------------------------------

			case P_USE_ARGUMENTS2:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if(t.id == S_COMMA)
				{
					token_got = false;
					if (stack_int_push(s, 2, P_USE_ARGUMENTS2, P_EXPR) < 0) // TODO - porovnani typu expr - pocotadlo argumentu?
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_EXPR
				}

				if(i.id == S_RIGHT_PARE) // ')' - no other arguments
				{
					token_got = false;
					stack_int_pop(s);
					break; // just stack pop
				}

				fprintf(stderr, "PARSER: On line %u expected ')' or ','.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ------------------- P_RETURN_EXPR --------------------------------

			case P_RETURN_EXPR: // TODO - porovnat s typem vystupu funkce
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if (t.id == S_SEMICOMMA) // ';' - void funkce?
				{
					token_got = false;
					break; // just stack_int_pop(s);
				}

				if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return ERR_INTERN_FAULT;
				}
				break; // goto case P_EXPR

			// --------------- P_ELSE_EXISTANCE --------------------------------

			case P_ELSE_EXISTANCE:
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;
				}
				token_got = true;

				if(t.id == S_ELSE)
				{
					token_got = false;
					if (stack_int_push(s, 1, P_IF_ELSE_SECTION) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return -1;
					}
					break; // goto P_IF_ELSE_SECTION
				}
				else
				{
					token_got = true;
					break; // else is not there - goto FUNC_BODY
				}
		}
	}

	return ERR_SYNTACTIC_ANALYSIS;
}
