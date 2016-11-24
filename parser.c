#include "parser.h"

extern int expr_analyze(token * t); // potrebuji, aby mi zmenila token

char *join_strings(char *str1, char *str2) {
	size_t len[2] = {strlen(str1), strlen(str2)};
	char *result = (char *) malloc(len[0] + len[1] + 2);
	strcpy(result, str1);
	result[len[0]] = '.';
	strcpy(&result[len[0]+1], str2);
	return result;
}

int parser()
{
	stack_int_t s;

	int parser_return;

	// vytvoreni a naplneni zasobniku na stavy
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

	// vytvoreni a inicializace tabulky symbolu (hash. table)
	htab_t * TableSymbols;
	TableSymbols = htab_init(HTAB_SIZE);
	if (TableSymbols == NULL)
	{
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parser cannot malloc hash table.\n");
		return ERR_INTERN_FAULT;
	}
	// zasobnik tabulek symbolu a naplneni prvni tabulkou na static promenne a funkce
	stack_htab Stack_of_TableSymbols;
	if (stack_htab_init(& Stack_of_TableSymbols) != 0)
	{
		htab_free_all(TableSymbols);
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parser cannot malloc stack of hash tables.\n");
		return ERR_INTERN_FAULT;
	}

	if (stack_htab_push(& Stack_of_TableSymbols, TableSymbols) != 0)
	{
		stack_htab_destroy(& Stack_of_TableSymbols);
		htab_free_all(TableSymbols);
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parser cannot realloc stack of hash tables.\n");
		return ERR_INTERN_FAULT;
	}

	parser_return = analysis(&s, 1, Stack_of_TableSymbols);




	stack_htab_destroy(& Stack_of_TableSymbols);
	htab_free_all(TableSymbols);
	stack_int_destroy(&s);
	return parser_return;
}

int analysis (stack_int_t *s, unsigned runtime, stack_htab Stack_of_TableSymbols)
{
	token t;
	bool token_got = false; // variable for P_EXPR - if the token have been read

	int on_top; // top of the stack_int - zasobnik stavu

	bool main_existance = false; // if class Main exists in whole input file - TODO
	bool main_run_existance = true; // if function run exist in function Main - TODO

	char * class_name = NULL; // pro vyrobu full_ident
	int class_name_strlen = 0;
	char * func_var_name = NULL; // pro ukladani do tabulky

	int type = 0; // t.id data_type is not 0

	bool void_existance = false; // if true - control no existance of void variable and return with no expr. in void function


	htab_t * TableSymbols = NULL; // TODO - for later use
	htab_item * TableItem = NULL;

	while (!stack_int_top(s, &on_top)) // stack_int_top == -1 if stack is empty
	{

		switch (on_top)
		{
			// ======================== SYMBOLS =============================

			case S_LEFT_BRACE:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_LEFT_BRACE) // '{'
				{
					token_got = false;
					break;
				}

				fprintf(stderr, "PARSER: On line %u expected '{'.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			case S_LEFT_PARE:
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_LEFT_PARE) // '('
				{
					token_got = false;
					break;
				}

				fprintf(stderr, "PARSER: On line %u expected '('.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			case S_RIGHT_BRACE:
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_RIGHT_BRACE) // '}'
				{
					token_got = false;
					break;
				}

				fprintf(stderr, "PARSER: On line %u expected '}'.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			case S_RIGHT_PARE:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')'
				{
					token_got = false;
					break;
				}

				fprintf(stderr, "PARSER: On line %u expected ')'.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			case S_SEMICOMMA:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_SEMICOMMA) // ';'
				{
					token_got = false;
					break;
				}

				fprintf(stderr, "PARSER: On line %u expected ';'.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_CLASS =============================
			case P_CLASS:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
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
						if (!main_existance)
							fprintf(stderr, "Semantic fauilt. Class Main does not exist.\n");
						else
							fprintf(stderr, "Semantic fauilt. Class Main has no function run.\n");
						return ERR_SEM_NDEF_REDEF;
					}
				}

				if (t.id == S_CLASS)
				{
					token_got = false;

					if (token_got == false)
					{
						t = get_token();
						if (t.id == 0)
							return ERR_LEXICAL_ANALYSIS;
						else if (t.id < 0)
							return ERR_INTERN_FAULT;
					}
					token_got = true;

					if(t.id == S_SIMPLE_IDENT)
					{
						token_got = false;
						// make a copy of class name
						if (class_name != NULL)	// TODO ? - just realloc
							free(class_name);
						class_name_strlen = strlen((char*) t.ptr) + 1; // '\0'
						class_name = (char *) malloc(class_name_strlen); // * sizeof(char) = 1
						if (class_name == NULL)
						{
							fprintf(stderr, "Intern fault. Memory allocation of class_name failed.\n");
							return ERR_INTERN_FAULT;
						}
						class_name = strncpy(class_name, (char*)t.ptr, class_name_strlen);

						printf("MACKA:%s\n", class_name);

						if (strcmp(class_name,"Main") == 0)
							main_existance = true;

						if (token_got == false)
						{
							t = get_token();
							if (t.id == 0)
								return ERR_LEXICAL_ANALYSIS;
							else if (t.id < 0)
								return ERR_INTERN_FAULT;
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
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
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
					token_got = false;
					if (token_got == false)
					{
						t = get_token();
						if (t.id == 0)
							return ERR_LEXICAL_ANALYSIS;
						else if (t.id < 0)
							return ERR_INTERN_FAULT;
					}
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
					token_got = false;

					if (token_got == false)
					{
						t = get_token();
						if (t.id == 0)
							return ERR_LEXICAL_ANALYSIS;
						else if (t.id < 0)
							return ERR_INTERN_FAULT;
					}
					token_got = true;

					if (t.id == S_SIMPLE_IDENT)
					{
						// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
						// data_type = type;
						if (runtime == 1)
						{
							// TableSymbols = stack_htab_get_item(&Stack_of_TableSymbols, 0);

							if (func_var_name != NULL)
								free (func_var_name);

							printf("MACKA:%s\n", (char*) t.ptr);
							func_var_name = join_strings(class_name, (char*) t.ptr);

							printf("MACKA:%s\n", func_var_name);

							/*
							TableItem = htab_find_item(TableSymbols, func_var_name);
							if (TableItem == NULL)
							{
								TableItem = htab_insert_item(TableSymbols, func_var_name);
							}
							else
							{
								fprintf(stderr, "PARSER: % has been already defined.\n");
								return ERR_SEM_NDEF_REDEF;
							}
							if(TableItem == NULL)
							{
								fprintf(stderr, "Intern fault. Parser cannot insert item into Table of symbols (malloc problem).\n");
								return ERR_INTERN_FAULT;
							}
							*/
						}

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
				}
				fprintf (stderr, "PARSER: On line %u expected static function, static variable definition or '}'\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_DEF ===============================

			case P_DEF:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_LEFT_PARE) // scanner: '(' - function
				{
					// TODO ? - ukladat nejake dalsi informace do tabulky?
					token_got = false;
					if (stack_int_push(s, 5, S_RIGHT_BRACE, P_FUNC, S_LEFT_BRACE, S_RIGHT_PARE, P_DEF_ARGUMENTS) < 0)
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

			// ======================== P_DEF_ARGUMENTS =====================

			case P_DEF_ARGUMENTS:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
				{
					//token_got = false;
					//stack_int_pop(s);
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
				token_got = false;

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
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

			// ======================== P_DEF_ARGUMENTS2 ====================

			case P_DEF_ARGUMENTS2:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')' - no other arguments
				{
					//token_got = false;
					//stack_int_pop(s);
					break; // goto case S_RIGHT_PARE
				}
				else if (t.id != S_COMMA) // ',' - other arguments
				{
					token_got = false;
					fprintf(stderr, "PARSER: On line %u expected ')' or ','.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}
				
				token_got = false;

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
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
				token_got = false;

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
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

			// ======================== P_FUNC ==============================

			case P_FUNC:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				// definition of local variable
				if      (t.id == S_INT)
					type = S_INT;
				else if (t.id == S_DOUBLE)
					type = S_DOUBLE;
				else if (t.id == S_STRING)
					type = S_STRING;
				else if (t.id == S_BOOLEAN)
					type = S_BOOLEAN;
				else if (t.id == S_RIGHT_BRACE) // no other stafs in function
					break;
				else
				{ // P_FUNC_BODY
					if (stack_int_push(s, 2, P_FUNC, P_FUNC_BODY) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto P_FUNC_BODY
				}
				token_got = false;

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_SIMPLE_IDENT)
				{
					// TODO - vlozit t.ptr do hash. tabulky (pod full_ident? class_name+t.ptr)
					// data_type = type;
					token_got = false;
					if (stack_int_push(s, 2, P_FUNC, P_VAR_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto P_VAR_EXPR
				}

				fprintf(stderr, "PARSER: On line %u expected simple identifikator.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_FUNC_BODY =========================

			case P_FUNC_BODY:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
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
					if (stack_int_push(s, 4, P_IF_ELSE_SECTION, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_LEFT_PARE
				}

				if (t.id == S_INT || t.id == S_DOUBLE || t.id == S_STRING || t.id == S_BOOLEAN)
				{
					fprintf (stderr, "PARSER: On line %u cannot be a definition of variable or function.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				fprintf (stderr, "PARSER: On line %u unexpected function body.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_VAR_EXPR ==========================

			case P_VAR_EXPR:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
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

			// ======================== P_GUIDANCE ==========================

			case P_GUIDANCE:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_LEFT_PARE) // '('
				{
					//token_got = false;
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
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

			// ======================== P_USE_ARGUMENTS =====================

			case P_USE_ARGUMENTS:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_RIGHT_PARE) // ')' - no arguments
				{
					//token_got = false;
					//stack_int_pop(s);
					break; // just stack pop
				}

				if (stack_int_push(s, 2, P_USE_ARGUMENTS2, P_EXPR) < 0) // TODO - porovnani typu expr - pocotadlo argumentu?
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return ERR_INTERN_FAULT;
				}
				break; // goto case P_EXPR

			// ======================== P_USE_ARGUMENTS2 ====================

			case P_USE_ARGUMENTS2:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
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

				if(t.id == S_RIGHT_PARE) // ')' - no other arguments
				{
					//token_got = false;
					//stack_int_pop(s);
					break; // just stack pop
				}

				fprintf(stderr, "PARSER: On line %u expected ')' or ','.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_RETURN_EXPR =======================

			case P_RETURN_EXPR: // TODO - porovnat s typem vystupu funkce
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
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

			// ======================== P_ELSE_EXISTANCE ====================

			case P_ELSE_EXISTANCE:
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if(t.id == S_ELSE)
				{
					token_got = false;
					if (stack_int_push(s, 1, P_IF_ELSE_SECTION) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto P_IF_ELSE_SECTION
				}
				else
				{
					token_got = true;
					break; // else is not there - goto FUNC_BODY
				}

			// ======================== P_IF_ELSE_SECTION ===================

			case P_IF_ELSE_SECTION:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_LEFT_BRACE) // '{'
				{
					token_got = false;
					if (stack_int_push(s, 2, S_RIGHT_BRACE, P_FUNC_BODY_H1) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto P_FUNC_BODY_H1
				}

				if (stack_int_push(s, 1, P_FUNC_BODY) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return ERR_INTERN_FAULT;
				}
				break; // goto case P_FUNC_BODY

			// ======================== P_FUNC_BODY_H1 ======================

			case P_FUNC_BODY_H1:
				stack_int_pop(s);
				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				if (t.id == S_RIGHT_BRACE) // '}'
				{
					//token_got = false;
					//stack_int_pop(s);
					break; // goto S_RIGHT_BRACE
				}

				if (stack_int_push(s, 2, P_FUNC_BODY_H1, P_FUNC_BODY) < 0)
				{
					fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
					return ERR_INTERN_FAULT;
				}
				break; // goto case P_FUNC_BODY

			// ======================== P_EXPR ==============================

			case P_EXPR:
				stack_int_pop(s);

				if (token_got == false)
				{
					t = get_token();
					if (t.id == 0)
						return ERR_LEXICAL_ANALYSIS;
					else if (t.id < 0)
						return ERR_INTERN_FAULT;
				}
				token_got = true;

				int expr_return;
				expr_return = skip_expr(&t);
				if (expr_return != 0)
					return expr_return;
				token_got = true;
				break;

			default:
				fprintf(stderr, "Intern fault. Unexpected situation.\n");
				return ERR_INTERN_FAULT;

		}
	}

	return ERR_SYNTACTIC_ANALYSIS;
}

int skip_expr(token * t)
{
	// token je nacteny
	// TODO - S_COMMA
	int number_pares = 0;
	int pocet_zpracovanych_tokenu = 0;
	bool token_got = true;
	do {
		if (token_wanted(t))
		{
			token_got = false;
		}
		else // special situation
		{
			if (t->id == S_LEFT_PARE)
			{
				number_pares++;
				token_got = false;
			}


			else if (t->id == S_RIGHT_PARE && pocet_zpracovanych_tokenu == 0)
			{
				fprintf(stderr, "PARSER: On line %u expected expresion.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;
			}
			else if (t->id == S_RIGHT_PARE && number_pares > 0)
			{
				number_pares--;
				token_got = false;
			}
			else if (t->id == S_RIGHT_PARE)
				return 0;


			else if (t->id == S_SEMICOMMA && pocet_zpracovanych_tokenu == 0)
			{
				fprintf(stderr, "PARSER: On line %u expected expresion.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;
			}
			else if (t->id == S_SEMICOMMA) return 0;

			else if (t->id == S_COMMA && number_pares > 0) token_got = false;

			else
			{
				fprintf(stderr, "PARSER: On line %u unexpected word, symbol, or EOF in expresion.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;
			}
		}
		pocet_zpracovanych_tokenu++;

		if (token_got == false)
		{
			*t = get_token();
			if (t->id == 0)
				return ERR_LEXICAL_ANALYSIS;
			else if (t->id < 0)
				return ERR_INTERN_FAULT;
		}
		token_got = true;
	} while (1);

	return 0;
}

// Krystof Michal
bool token_wanted(token * t)
{
	if (t->id == S_TRUE) return true;
	else if (t->id == S_FALSE) return true;
	else if (t->id == TYPE_DOUBLE) return true;
	else if (t->id == TYPE_INT) return true;
	else if (t->id == TYPE_STRING) return true;
	else if (t->id == TYPE_BOOLEAN) return true;
	else if (t->id == S_SIMPLE_IDENT) return true;
	else if (t->id == S_FULL_IDENT) return true;
	else if (t->id == S_EQUAL) return true;
	else if (t->id == S_LESS_EQUAL) return true;
	else if (t->id == S_GREATER_EQUAL) return true;
	else if (t->id == S_NOT_EQUAL) return true;
	else if (t->id == S_PLUS) return true;
	else if (t->id == S_MINUS) return true;
	else if (t->id == S_DIV) return true;
	else if (t->id == S_MUL) return true;
	else if (t->id == S_OR) return true;
	else if (t->id == S_AND) return true;
	else if (t->id == S_LESS) return true;
	else if (t->id == S_GREATER) return true;
	else return false;
}
