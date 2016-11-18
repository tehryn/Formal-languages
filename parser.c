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

	while (!stack_int_top(s, &on_top)) // stack_int_top == -1 if stack is empty
	{

		switch (on_top) 
		{
			// ======================== P_CLASS ========================
			case P_CLASS:
				stack_int_pop(s);

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;

				if (t.id == S_EOF) // The end of the input file and no other classes
				{
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

					if(t.id == S_SIMPLE_IDENT)
					{
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

						t = get_token();
						if (t.id <= 0)
							return ERR_LEXICAL_ANALYSIS;

						if (t.id == S_LEFT_BRACE)
						{
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

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;

				if (t.id == S_RIGHT_BRACE) // scanner: '}' -> no class body
				{
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
					else
					{
						fprintf (stderr, "PARSER: On line %u expected some data type.\n", LINE_NUM);
						return ERR_SYNTACTIC_ANALYSIS;
					}

					t = get_token();
					if (t.id <= 0)
						return ERR_LEXICAL_ANALYSIS;

					if (t.id == S_SIMPLE_IDENT)
					{
						// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
						// data_type = type;
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

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;

				if (t.id == S_LEFT_PARE) // scanner: '(' - function
				{
					// TODO ? - ukladat nejake dalsi informace do tabulky?
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
					break; // goto P_CLASS_BODY
				}

				if (t.id == S_ASSIGNMENT) // scanner: '=' -> variable with inicialization
				{
					// TODO ? - ukladat nejake dalsi informace do tabulky?
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

			case P_DEF_ARGUMENTS: // TODO
				stack_int_pop(s);

				t = get_token();
				if (t.id <= 0)
					return ERR_LEXICAL_ANALYSIS;

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
					break; // goto case S_RIGHT_PARE
				
				if (stack_int_push(s, 3, P_DEF_ARGUMENTS2, S_SIMPLE_IDENT, P_TYPE) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return -1;	
				}
				break; // goto case P_TYPE
		}
	}

	return ERR_SYNTACTIC_ANALYSIS;
}


/*
			case S_EOF:
				if (t.id == S_EOF)
					return 0;


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

				if(i.id == S_RIGHT_PARE) // ')' - no other arguments
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

			case default:
				stack_int_pop(s);
				fprintf(stderr, "Intern fault. Unexpected situation.\n");
				return -1;
*/