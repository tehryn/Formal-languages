#include "parser.h"
#include "expression.h"
#include "interpret.h"

char *join_strings(char *str1, char *str2) {
	size_t len[2] = {strlen(str1), strlen(str2)};
	char *result = (char *) mem_alloc(len[0] + len[1] + 2);
	if (result == NULL)
		return NULL;
	strcpy(result, str1);
	result[len[0]] = '.';
	strcpy(&result[len[0]+1], str2);
	return result;
}

char * class_name = NULL; // pro vyrobu full_ident
int class_name_strlen = 0;
char * static_func_var_name = NULL; // pro ukladani do tabulky
char * local_func_var_name = NULL; // pro ukladani do tabulky
array_string all_class_names;

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
	htab_t * global_table_symbols;
	global_table_symbols = htab_init(HTAB_SIZE);
	if (global_table_symbols == NULL)
	{
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parser cannot malloc hash table.\n");
		return ERR_INTERN_FAULT;
	}

	// zasobnik tabulek symbolu a naplneni prvni tabulkou na static promenne a funkce
	stack_htab stack_of_table_symbols;
	if (stack_htab_init(& stack_of_table_symbols) != 0)
	{
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parser cannot malloc stack of hash tables.\n");
		return ERR_INTERN_FAULT;
	}

	if (stack_htab_push(& stack_of_table_symbols, global_table_symbols) != 0)
	{
		//stack_htab_destroy(& stack_of_table_symbols);
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parser cannot realloc stack of hash tables.\n");
		return ERR_INTERN_FAULT;
	}

	// inicializace pole stringu
	if (array_string_init(&all_class_names) != 0)
	{
		//stack_htab_destroy(& stack_of_table_symbols);
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		fprintf(stderr, "Intern fault. Parset cannot malloc place for string array.\n");
		return ERR_INTERN_FAULT;
	}

	// vestavene funkce
	if (embedded_functions_into_hash_table(global_table_symbols, &all_class_names) != 0)
	{
		//stack_htab_destroy(& stack_of_table_symbols);
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		return ERR_INTERN_FAULT;
	}

	Instr_List * InstructionTape = NULL;
	InstructionTape = mem_alloc(sizeof(Instr_List));
	if (InstructionTape == NULL)
	{
		fprintf(stderr, "Intern fault. Instruction tape allocation failed.\n");
		return ERR_INTERN_FAULT;
	}
	InstructionTape->Active=NULL;
	InstructionTape->Last=NULL;

	// runtime = 1
	parser_return = analysis(&s, 1, stack_of_table_symbols, InstructionTape);
	if (parser_return != 0)
	{
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		return parser_return;
	}
	// priprava na runtime 2
	if(reset_scanner() == -1)
	{
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		fprintf(stderr, "SCANNER: Cannot reset tokens. \n");
		return ERR_INTERN_FAULT;
	}
	if (stack_int_push(&s, 2, S_EOF, P_CLASS) < 0)
	{
		//htab_free_all(global_table_symbols);
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
		return ERR_INTERN_FAULT;
	}
	// runtime = 2

	parser_return = analysis(&s, 2, stack_of_table_symbols, InstructionTape);

	if (parser_return != 0)
	{
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		return parser_return;
	}

	// call Main.run

	I_Instr * Instruction = NULL;
	Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
	if (Instruction == NULL)
	{
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
		return ERR_INTERN_FAULT;
	}
	Instruction->type_instr = I_FCE;

	htab_item * TableItem = htab_find_item(global_table_symbols, "Main.run");
	Instruction->adr1 = TableItem;

	Instruction->adr2 = NULL;
	Instruction->adr3 = NULL;
	Instruction->next_instr = NULL;

	token * t_tmp = (token*) mem_alloc(2*sizeof(token));
	if (t_tmp == NULL)
	{
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		fprintf(stderr, "Intern fault. token cannot be allocated.\n");
		return ERR_INTERN_FAULT;
	}
	t_tmp[0].ptr = (void*) mem_alloc(strlen("Main.run")+1);
	if (t_tmp[0].ptr == NULL)
	{
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		fprintf(stderr, "Intern fault. Token cannot be allocated.\n");
		return ERR_INTERN_FAULT;
	}
	strcpy((char*)t_tmp[0].ptr, "Main.run");

	// nutne nastavit t_tmp[1] ...
	Instruction->adr2 = t_tmp;


	if (Add_Instr(InstructionTape, Instruction) != 0)
	{
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
		fprintf(stderr, "Intern fault. Instruction cannot be pushed into instruction tape.\n");
		return ERR_INTERN_FAULT;
	}

	// call interpret
	int interpret_return = inter(InstructionTape, &stack_of_table_symbols);
	if (interpret_return != 0)
	{
		stack_int_destroy(&s);
		stack_htab_destroy(&stack_of_table_symbols);
		array_string_destroy(&all_class_names);
//		stack_int_destroy(&s);
		return interpret_return;
	}

	//htab_free_all(global_table_symbols);




	stack_htab_destroy(&stack_of_table_symbols);
	array_string_destroy(&all_class_names);
	stack_int_destroy(&s);
	return parser_return;
}

int analysis (stack_int_t *s, unsigned runtime, stack_htab Stack_of_TableSymbols, Instr_List * InstructionTape)
{
	token t;
	t.ptr = NULL;
	bool token_got = false; // variable for P_EXPR - if the token have been read

	int on_top; // top of the stack_int - zasobnik stavu

	unsigned type = 0; // t.id data_type is not 0

	htab_t * GlobalTableSymbols = NULL;
	GlobalTableSymbols = stack_htab_get_item(&Stack_of_TableSymbols, 0);
	if (GlobalTableSymbols == NULL)
	{
		fprintf(stderr, "Intern fault. Parset cannot get hash. table from stack.\n");
		return ERR_INTERN_FAULT;
	}

	htab_t * LocalTableSymbols = NULL;

	htab_item * TableItem = NULL;

	int number_arguments = 0; // arguments of function
	int number_allocated_arguments = 0;
	unsigned func_or_var = 0;

	int error_6_flag = 1;

	I_Instr * Instruction = NULL;

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
					{
						fprintf(stderr, "On the top is not EOF.\n");
						return ERR_SYNTACTIC_ANALYSIS;
					}

					stack_int_pop(s);

					if (class_name != NULL)
					{
						class_name = NULL;
					}


					TableItem = htab_find_item(GlobalTableSymbols, "Main.run");
					if (TableItem == NULL || TableItem->data_type != S_VOID)
					{
						fprintf(stderr, "Semantic fauilt. There is no function 'Main.run' or it is not void function.\n");
						return ERR_SEM_NDEF_REDEF;
					}
					else
						return 0;
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
						class_name = NULL;
						class_name_strlen = strlen((char*) t.ptr) + 1; // '\0'
						class_name = (char *) mem_alloc(class_name_strlen); // * sizeof(char) = 1
						if (class_name == NULL)
						{
							fprintf(stderr, "Intern fault. Memory allocation of class_name failed.\n");
							return ERR_INTERN_FAULT;
						}
						class_name = strncpy(class_name, (char*)t.ptr, class_name_strlen);

						if (runtime == 1)
						{
							if (array_string_find(&all_class_names, class_name) == NULL)
							{
								if(array_string_insert(&all_class_names, class_name) != 0)
								{
									fprintf(stderr, "Intern fault. Parser cannot insert class %s into array.\n", class_name);
									return ERR_INTERN_FAULT;
								}
							}
							else
							{
								fprintf(stderr, "PARSER:  On line %u class %s has been already defined.\n", LINE_NUM, class_name);
								return ERR_SEM_NDEF_REDEF;
							}
						}

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
						type = S_VOID;
					else if (t.id == S_INT)
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
						// save a name of statick function or variable
						if(static_func_var_name != NULL)
						{
							static_func_var_name = NULL; // garbage collector will free
						}
						static_func_var_name = join_strings(class_name, (char*) t.ptr);
						if (static_func_var_name == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot join strings.\n");
							return ERR_INTERN_FAULT;
						}

						if (runtime == 1)
						{
							TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
							if (TableItem == NULL)
							{
								TableItem = htab_insert_item(GlobalTableSymbols, static_func_var_name);
							}
							else
							{
								fprintf(stderr, "PARSER:  On line %u %s has been already defined.\n", LINE_NUM, static_func_var_name);
								return ERR_SEM_NDEF_REDEF;
							}
							if(TableItem == NULL)
							{
								fprintf(stderr, "Intern fault. Parser cannot insert item into Table of symbols (malloc problem).\n");
								return ERR_INTERN_FAULT;
							}

							TableItem->data_type = type;
						}
						if (runtime == 2)
						{
							TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
							if (TableItem == NULL)
							{
								fprintf(stderr, "Intern fault. Parser cannot find item of hash. table which should be there.\n");
								return ERR_INTERN_FAULT;
							}

							func_or_var = TableItem->func_or_var;

							if (func_or_var == 2) // function
							{
								LocalTableSymbols = htab_init(HTAB_SIZE);
								if (LocalTableSymbols == NULL)
								{
									fprintf(stderr, "Intern fault. Parser cannot malloc local hash table.\n");
									return ERR_INTERN_FAULT;
								}

								TableItem->local_table = (void *) LocalTableSymbols; // put local table of symbols to function where it belongs
							}
							else if (func_or_var == 1)
							{

								Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
								if (Instruction == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								Instruction->type_instr = I_ASSIGMENT;

								token * t_tmp = (token*) mem_alloc(sizeof(token));
								if (t_tmp == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								t_tmp->ptr = (void*) mem_alloc(strlen(static_func_var_name)+1);
								if (t_tmp->ptr == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								strcpy((char*)t_tmp->ptr, static_func_var_name);
								t_tmp->id = t.id;
								Instruction->adr1 = t_tmp;
								Instruction->adr2 = NULL;
								Instruction->adr3 = NULL;
								Instruction->next_instr = NULL;
							}
						}

						token_got = false;
						if (stack_int_push(s, 2, P_CLASS_BODY, P_DEF) < 0)
						{
							fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
							return ERR_INTERN_FAULT;
						}

						break; // goto case P_DEF
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
					token_got = false;

					if (runtime == 1)
					{
						TableItem->func_or_var = 2; // function
						TableItem->initialized = 1;
					}

					if (stack_int_push(s, 5, S_RIGHT_BRACE, P_FUNC, S_LEFT_BRACE, S_RIGHT_PARE, P_DEF_ARGUMENTS) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break;
				}

				if (t.id == S_SEMICOMMA) // scanner: ';' -> just variable without inicialisation
				{
					token_got = false;

					if (runtime == 1)
					{
						TableItem->func_or_var = 1; // variable
													// it is not initialized
						if (TableItem->data_type == S_VOID)
						{
							fprintf(stderr, "PARSER: You cannot have a void variable.\n");
							return ERR_SEM_NDEF_REDEF;
						}
					}
					else if (runtime == 2)
						Instruction = NULL;

					break; // goto P_CLASS_BODY
				}

				if (t.id == S_ASSIGNMENT) // scanner: '=' -> variable with inicialization
				{
					token_got = false;

					if (runtime == 1)
					{
						TableItem->func_or_var = 1; // variable and it will be initialized
					}

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
					if (runtime == 1)
					{
						TableItem->data = NULL;
						TableItem->data = (void*) mem_alloc(1 * sizeof(int));
						if (TableItem->data == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot malloc place for data in hash table (int array).\n");
							return ERR_INTERN_FAULT;
						}
						((int*)TableItem->data)[0] = S_EOF;
						TableItem->number_of_arguments = number_arguments; // number_arguments = 0

						number_arguments = 0;
						number_allocated_arguments = 0;
					}
					break; // goto case S_RIGHT_PARE
				}

				if (runtime == 1) // it will be first argument
				{
					number_arguments++;

					number_allocated_arguments += 4;

					TableItem->data = (void*) mem_alloc(number_allocated_arguments * sizeof(int)); // 4 for beginning
					if (TableItem->data == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot malloc place for data in hash table (int array).\n");
						return ERR_INTERN_FAULT;
					}

					((int*)TableItem->data)[0] = S_EOF;
					((int*)TableItem->data)[1] = S_EOF;
					((int*)TableItem->data)[2] = S_EOF;
					((int*)TableItem->data)[3] = S_EOF;

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
					fprintf (stderr, "PARSER: On line %u expected some data type (and not void).\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}
				token_got = false;

				if (runtime == 1)
					((int*)TableItem->data)[number_arguments-1] = type;

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
					token_got = false;
					if (runtime == 2)
					{
						TableItem = htab_find_item(LocalTableSymbols, (char*)t.ptr);
						if (TableItem == NULL)
						{
							TableItem = htab_insert_item(LocalTableSymbols, (char*)t.ptr);
						}
						else
						{
							fprintf(stderr, "PARSER: On line %u %s has been already defined in this function.\n", LINE_NUM, (char*)t.ptr);
							return ERR_SEM_NDEF_REDEF;
						}
						if(TableItem == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot insert item into Table of symbols (malloc problem).\n");
							return ERR_INTERN_FAULT;
						}

						TableItem->data_type = type;
						TableItem->func_or_var = 1;
						TableItem->initialized = 1;
					}

					if (stack_int_push(s, 1, P_DEF_ARGUMENTS2) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}

					break; // goto case P_DEF_ARGUMENTS2
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
					if (runtime == 1)
					{
						TableItem->number_of_arguments = number_arguments;

						number_arguments = 0;
						number_allocated_arguments = 0;
					}
					break; // goto case S_RIGHT_PARE
				}
				else if (t.id != S_COMMA) // ',' - other arguments
				{
					token_got = false;
					fprintf(stderr, "PARSER: On line %u expected ')' or ','.\n", LINE_NUM);
					return ERR_SYNTACTIC_ANALYSIS;
				}

				if (runtime == 1) // it will be first argument
				{
					number_arguments++;
					if(number_arguments >= number_allocated_arguments-1) // -1 for ending symbol
					{
						number_allocated_arguments += 4;

						TableItem->data = (int*) mem_realloc(TableItem->data, number_allocated_arguments * sizeof(int)); // next 4 potentional arguments
						if (TableItem->data == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot malloc place for data in hash table (int array).\n");
							return ERR_INTERN_FAULT;
						}

						((int*)TableItem->data)[number_allocated_arguments -4] = S_EOF;
						((int*)TableItem->data)[number_allocated_arguments -3] = S_EOF;
						((int*)TableItem->data)[number_allocated_arguments -2] = S_EOF;
						((int*)TableItem->data)[number_allocated_arguments -1] = S_EOF;
					}
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

				if (runtime == 1)
					((int*)TableItem->data)[number_arguments-1] = type;

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
					if (runtime == 2)
					{
						TableItem = htab_find_item(LocalTableSymbols, (char*)t.ptr);
						if (TableItem == NULL)
						{
							TableItem = htab_insert_item(LocalTableSymbols, (char*)t.ptr);
						}
						else
						{
							fprintf(stderr, "PARSER: On line %u %s has been already defined in this function.\n", LINE_NUM, (char*)t.ptr);
							return ERR_SEM_NDEF_REDEF;
						}
						if(TableItem == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot insert item into Table of symbols (malloc problem).\n");
							return ERR_INTERN_FAULT;
						}

						TableItem->data_type = type;
						TableItem->func_or_var = 1;
						TableItem->initialized = 1;
					}

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

				error_6_flag = 0;

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
				else if (t.id == S_RIGHT_BRACE) // no other stuffs in function and everything in this if will be freed by garbage collector
				{
					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_END;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;

					LocalTableSymbols = NULL;
					static_func_var_name = NULL;
					error_6_flag = 1;
					break; // goto case S_RIGHT_BARCE
				}
				else
				{ // P_FUNC_BODY -> not definition of local variable
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
					if (runtime == 2)
					{
						TableItem = htab_find_item(LocalTableSymbols, (char*)t.ptr);
						if (TableItem == NULL)
						{
							TableItem = htab_insert_item(LocalTableSymbols, (char*)t.ptr);
						}
						else
						{
							fprintf(stderr, "PARSER: On line %u %s has been already defined in this function.\n", LINE_NUM, (char*)t.ptr);
							return ERR_SEM_NDEF_REDEF;
						}
						if(TableItem == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot insert item into Table of symbols (malloc problem).\n");
							return ERR_INTERN_FAULT;
						}

						TableItem->data_type = type;
						TableItem->func_or_var = 1;


						Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
						if (Instruction == NULL)
						{
							fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
							return ERR_INTERN_FAULT;
						}
						Instruction->type_instr = I_ASSIGMENT;

						token * t_tmp = (token*) mem_alloc(sizeof(token));
						if (t_tmp == NULL)
						{
							fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
							return ERR_INTERN_FAULT;
						}
						t_tmp->ptr = (void*) mem_alloc(strlen((char *) t.ptr)+1);
						if (t_tmp->ptr == NULL)
						{
							fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
							return ERR_INTERN_FAULT;
						}
						strcpy((char*)t_tmp->ptr, (char *) t.ptr);
						Instruction->adr1 = t_tmp;
						Instruction->adr2 = NULL;
						Instruction->adr3 = NULL;
						Instruction->next_instr = NULL;
					}

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
					if (runtime == 1)
					{
						token_got = false;
						if (stack_int_push(s, 1, P_GUIDANCE) < 0)
						{
							fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
							return ERR_INTERN_FAULT;
						}
						break; // goto case P_GUIDANCE
					}
					if (runtime == 2)
					{
						token_got = false;
						TableItem = htab_find_item(LocalTableSymbols, (char*)t.ptr);
						if(TableItem!=NULL) // local variable
						{
							Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
							if (Instruction == NULL)
							{
								fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
								return ERR_INTERN_FAULT;
							}
							Instruction->type_instr = I_ASSIGMENT;

							token * t_tmp = (token*) mem_alloc(sizeof(token));
							if (t_tmp == NULL)
							{
								fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
								return ERR_INTERN_FAULT;
							}
							t_tmp->ptr = (void*) mem_alloc(strlen((char *) t.ptr)+1);
							if (t_tmp->ptr == NULL)
							{
								fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
								return ERR_INTERN_FAULT;
							}
							strcpy((char*)t_tmp->ptr, (char *) t.ptr);
							Instruction->adr1 = t_tmp;
							Instruction->adr2 = NULL;
							Instruction->adr3 = NULL;
							Instruction->next_instr = NULL;

							if (stack_int_push(s, 1, P_GUIDANCE) < 0)
							{
								fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
								return ERR_INTERN_FAULT;
							}
							break; // goto case P_GUIDANCE
						}
						else
						{
							local_func_var_name = join_strings(class_name, (char*) t.ptr);
							if (local_func_var_name == NULL)
							{
								fprintf(stderr, "Intern fault. Parser cannot join strings.\n");
								return ERR_INTERN_FAULT;
							}
							TableItem = htab_find_item(GlobalTableSymbols, local_func_var_name);
							if (TableItem == NULL)
							{
								fprintf(stderr, "PARSER:  On line %u variable or function %s has not been defined.\n", LINE_NUM, (char*)t.ptr);
								return ERR_SEM_NDEF_REDEF;
							}
							else
							{
								t.id = S_FULL_IDENT;
								t.ptr = (void*) local_func_var_name;

								if (TableItem->func_or_var == 2)
								{
									Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
									if (Instruction == NULL)
									{
										fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
										return ERR_INTERN_FAULT;
									}
									Instruction->type_instr = I_FCE;

									Instruction->adr1 = & TableItem;
									Instruction->adr2 = NULL;
									Instruction->adr3 = NULL;
									Instruction->next_instr = NULL;

									if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
									{
										fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
										return ERR_INTERN_FAULT;
									}
									break; // goto case P_EXPR
								}
								else
								{
									Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
									if (Instruction == NULL)
									{
										fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
										return ERR_INTERN_FAULT;
									}
									Instruction->type_instr = I_ASSIGMENT;

									token * t_tmp = (token*) mem_alloc(sizeof(token));
									if (t_tmp == NULL)
									{
										fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
										return ERR_INTERN_FAULT;
									}
									t_tmp->ptr = (void*) mem_alloc(strlen((char *) t.ptr)+1);
									if (t_tmp->ptr == NULL)
									{
										fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
										return ERR_INTERN_FAULT;
									}
									strcpy((char*)t_tmp->ptr, (char *) t.ptr);
									Instruction->adr1 = t_tmp;
									Instruction->adr2 = NULL;
									Instruction->adr3 = NULL;
									Instruction->next_instr = NULL;

									token_got = false;
									if (stack_int_push(s, 1, P_GUIDANCE) < 0)
									{
										fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
										return ERR_INTERN_FAULT;
									}
									break; // goto case P_GUIDANCE
								}
							}
						}
					}
				}

				if(t.id == S_FULL_IDENT)
				{
					if (runtime == 1)
					{
						token_got = false;
						if (stack_int_push(s, 1, P_GUIDANCE) < 0)
						{
							fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
							return ERR_INTERN_FAULT;
						}
						break; // goto case P_GUIDANCE
					}
					if (runtime == 2)
					{
						TableItem = htab_find_item(GlobalTableSymbols, (char*) t.ptr);
						if (TableItem == NULL)
						{
							fprintf(stderr, "PARSER:  On line %u variable or function %s has not been defined.\n", LINE_NUM, (char*)t.ptr);
							return ERR_SEM_NDEF_REDEF;
						}
						else
						{
							if (TableItem->func_or_var == 2)
							{
								Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
								if (Instruction == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								Instruction->type_instr = I_FCE;

								Instruction->adr1 = & TableItem;
								Instruction->adr2 = NULL;
								Instruction->adr3 = NULL;
								Instruction->next_instr = NULL;

								if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
								{
									fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
									return ERR_INTERN_FAULT;
								}
								break; // goto case P_EXPR
							}
							else
							{
								Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
								if (Instruction == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								Instruction->type_instr = I_ASSIGMENT;

								token * t_tmp = (token*) mem_alloc(sizeof(token));
								if (t_tmp == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								t_tmp->ptr = (void*) mem_alloc(strlen((char *) t.ptr)+1);
								if (t_tmp->ptr == NULL)
								{
									fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
									return ERR_INTERN_FAULT;
								}
								strcpy((char*)t_tmp->ptr, (char *) t.ptr);
								Instruction->adr1 = t_tmp;
								Instruction->adr2 = NULL;
								Instruction->adr3 = NULL;
								Instruction->next_instr = NULL;

								token_got = false;
								if (stack_int_push(s, 1, P_GUIDANCE) < 0)
								{
									fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
									return ERR_INTERN_FAULT;
								}
								break; // goto case P_GUIDANCE
							}
						}
					}
				}

				if(t.id == S_RETURN)
				{
					if (runtime == 2)
					{
						TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
						if (TableItem == NULL)
						{
							fprintf(stderr, "PARSER: Unexpected situation happend. We cannot find find function in which we are.\n");
							return ERR_INTERN_FAULT;
						}

						Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
						if (Instruction == NULL)
						{
							fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
							return ERR_INTERN_FAULT;
						}
						Instruction->type_instr = I_RETURN;
						Instruction->adr1 = NULL;
						Instruction->adr2 = NULL;
						Instruction->adr3 = NULL;
						Instruction->next_instr = NULL;
					}
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

					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_IF;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					if (stack_int_push(s, 6, P_ELSE_EXISTANCE, P_I_ENDIF, P_IF_ELSE_SECTION, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					if (runtime == 2) // because of expr.
						TableItem = NULL;
					break; // goto case P_LEFT_PARE
				}

				if(t.id == S_WHILE)
				{
					token_got = false;

					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_WHILE;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					if (stack_int_push(s, 5, P_I_ENDWHILE, P_IF_ELSE_SECTION, S_RIGHT_PARE, P_EXPR, S_LEFT_PARE) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					if(runtime == 2) // because of expr.
						TableItem = NULL;
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
					Instruction = NULL; // no assigment
					token_got = false;
					break; // just stack_int_pop(s);
				}
				if (t.id == S_ASSIGNMENT)
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

				if (t.id == S_LEFT_PARE) // '(' // in runtime 2, this if will be not opened (expr_analyze)
				{
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_USE_ARGUMENTS
				}

				if (t.id == S_SEMICOMMA) // ';' - in runtime 2 nothing hapened with variable
				{
					Instruction = NULL; // no assigment
					token_got = false;
					break; // just stack_int_pop(s);
				}
				if (t.id == S_ASSIGNMENT)
				{
					token_got = false;
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_EXPR, in runtime 2 - TableItem has been inicialized
				}

				fprintf(stderr, "PARSER: On line %u expected '(', ';' or assignment.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;

			// ======================== P_RETURN_EXPR =======================

			case P_RETURN_EXPR:
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

				TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
				if (TableItem == NULL)
				{
					fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
					return ERR_INTERN_FAULT;
				}

				if (TableItem->data_type != S_VOID)
				{
					if (stack_int_push(s, 2, S_SEMICOMMA, P_EXPR) < 0)
					{
						fprintf(stderr, "Intern fault. Parser cannot push item into stack.\n");
						return ERR_INTERN_FAULT;
					}
					break; // goto case P_EXPR
				}
				else
				{
					if (t.id != S_SEMICOMMA)
					{
						fprintf(stderr, "PARSER:  On line %u expected semicomma after return in void function.\n", LINE_NUM);
						return ERR_SEM_COMPATIBILITY;
					}

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;

					token_got = false;
					break;
				}

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

					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_ELSE;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;

					if (stack_int_push(s, 2, P_I_ENDELSE, P_IF_ELSE_SECTION) < 0)
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
				if (runtime == 1)
				{
					expr_return = skip_expr(&t);
					if (expr_return != 0)
						return expr_return;
				}

				if (runtime == 2)
				{

					int expected_expr_data_type = 0;
					if (TableItem == NULL)
						expected_expr_data_type = S_BOOLEAN;
					else
					{
						expected_expr_data_type = (int) TableItem->data_type;
					}
					token *postfix_token_array;
					int token_count, expr_data_type;
					expr_return = expr_analyze(t, &t, class_name, error_6_flag, &postfix_token_array, &token_count, &expr_data_type, GlobalTableSymbols, LocalTableSymbols);
					if (expr_return != 0)
						return expr_return;
					//free(postfix_token_array); // FIXME xmatej52

					if (expected_expr_data_type != expr_data_type)
					{
						if (expected_expr_data_type == S_DOUBLE && expr_data_type == S_INT) ;
						else
						{
							fprintf(stderr, "PARSER: On line %u unexpected different data type of expresion.\n", LINE_NUM);
							return ERR_SEM_COMPATIBILITY;
						}
					}

					if (Instruction->type_instr == I_ASSIGMENT) // assigment of variable
					{
						TableItem->initialized = 1;

						Instruction->adr2 = postfix_token_array;

						if (error_6_flag == 1) // we are out of function
							Add_Instr(InstructionTape, Instruction);
						else
						{
							TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
							if (TableItem == NULL)
							{
								fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
								return ERR_INTERN_FAULT;
							}
							I_Instr * tmp_ptr = TableItem->instruction_tape;
							if (tmp_ptr == NULL)
							{
								TableItem->instruction_tape = Instruction;
							}
							else
							{
								while(tmp_ptr->next_instr != NULL)
							 		tmp_ptr = tmp_ptr->next_instr;
								tmp_ptr->next_instr = Instruction;
							}
						}
						Instruction = NULL;
					}
					else if (Instruction->type_instr == I_IF || Instruction->type_instr == I_RETURN || Instruction->type_instr == I_WHILE)
					{
						Instruction->adr1 = postfix_token_array;

						TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
						if (TableItem == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
							return ERR_INTERN_FAULT;
						}
						I_Instr * tmp_ptr = TableItem->instruction_tape;
						if (tmp_ptr == NULL)
						{
							TableItem->instruction_tape = Instruction;
						}
						else
						{
							while(tmp_ptr->next_instr != NULL)
						 		tmp_ptr = tmp_ptr->next_instr;
							tmp_ptr->next_instr = Instruction;
						}

						Instruction = NULL;
					}
					else if (Instruction->type_instr == I_FCE)
					{
						Instruction->adr2 = postfix_token_array;

						TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
						if (TableItem == NULL)
						{
							fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
							return ERR_INTERN_FAULT;
						}

						I_Instr * tmp_ptr = TableItem->instruction_tape;
						if (tmp_ptr == NULL)
						{
							TableItem->instruction_tape = Instruction;
						}
						else
						{
							while(tmp_ptr->next_instr != NULL)
						 		tmp_ptr = tmp_ptr->next_instr;
							tmp_ptr->next_instr = Instruction;
						}

						Instruction = NULL;
					}
					else
					{
						fprintf(stderr, "Intern fault. Something bad happend in expression.\n");
						return ERR_INTERN_FAULT;
					}

				}
				token_got = true;
				break;

			// ======================== P_I_END apod. ======================

			case P_I_END:
				stack_int_pop(s);
				if (runtime == 2)
				{
					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_END;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;
				}
				break;

			case P_I_ENDIF:
				stack_int_pop(s);
				if (runtime == 2)
				{
					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_ENDIF;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;
				}
				break;

			case P_I_ENDELSE:
				stack_int_pop(s);
				if (runtime == 2)
				{
					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_ENDELSE;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;
				}
				break;

			case P_I_ENDWHILE:
				stack_int_pop(s);
				if (runtime == 2)
				{
					Instruction = (I_Instr*) mem_alloc(sizeof(I_Instr));
					if (Instruction == NULL)
					{
						fprintf(stderr, "Intern fault. Instruction cannot be allocated.\n");
						return ERR_INTERN_FAULT;
					}
					Instruction->type_instr = I_ENDWHILE;
					Instruction->adr1 = NULL;
					Instruction->adr2 = NULL;
					Instruction->adr3 = NULL;
					Instruction->next_instr = NULL;

					TableItem = htab_find_item(GlobalTableSymbols, static_func_var_name);
					if (TableItem == NULL)
					{
						fprintf(stderr, "Intern fault. Parser cannot find a function that exists and should be there.\n");
						return ERR_INTERN_FAULT;
					}
					I_Instr * tmp_ptr = TableItem->instruction_tape;
					if (tmp_ptr == NULL)
					{
						TableItem->instruction_tape = Instruction;
					}
					else
					{
						while(tmp_ptr->next_instr != NULL)
					 		tmp_ptr = tmp_ptr->next_instr;
						tmp_ptr->next_instr = Instruction;
					}
					Instruction = NULL;
				}
				break;

			default:
				fprintf(stderr, "Intern fault. Unexpected situation.\n");
				return ERR_INTERN_FAULT;

		}
	}
	fprintf(stderr, "We are out of switch.\n");
	return ERR_SYNTACTIC_ANALYSIS;
}

int skip_expr(token * t)
{
	// token je nacteny
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
			else if (t->id == S_SEMICOMMA && number_pares != 0)
			{
				fprintf(stderr, "PARSER: On line %u is not enough pares.\n", LINE_NUM);
				return ERR_SYNTACTIC_ANALYSIS;
			}
			else if (t->id == S_SEMICOMMA)
				return 0;

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

int embedded_functions_into_hash_table(htab_t * global_table_symbols, array_string* all_class_names)
{
	if (class_name == NULL)
		class_name = (char *) mem_alloc(strlen("ifj16")+1);
	else
		class_name = (char *) mem_realloc(class_name, strlen("ifj16")+1);

	if (class_name == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for class_name.\n");
		return ERR_INTERN_FAULT;
	}
	strcpy(class_name, "ifj16");
	if(array_string_insert(all_class_names, class_name) != 0)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert class %s into array.\n", class_name);
		return ERR_INTERN_FAULT;
	}

	htab_item * TableItem = NULL;

	// int readInt ( ) ;
	TableItem = htab_insert_item(global_table_symbols, "ifj16.readInt");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_INT;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 0;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_EOF;

	// double readDouble ( ) ;
	TableItem = htab_insert_item(global_table_symbols, "ifj16.readDouble");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_DOUBLE;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 0;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_EOF;

	// String readString ( ) ;
	TableItem = htab_insert_item(global_table_symbols, "ifj16.readString");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_STRING;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 0;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_EOF;

	// void print ( term_nebo_konkatenace ) ;
	TableItem = htab_insert_item(global_table_symbols, "ifj16.print");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_VOID;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 1;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (2*sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_STRING;
	((int*) TableItem->data)[1] = S_EOF;

	// int length(String s);
	TableItem = htab_insert_item(global_table_symbols, "ifj16.length");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_INT;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 1;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (2*sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_STRING;
	((int*) TableItem->data)[1] = S_EOF;

	// String substr(String s, int i, int n);
	TableItem = htab_insert_item(global_table_symbols, "ifj16.substr");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_STRING;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 3;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (4*sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_STRING;
	((int*) TableItem->data)[1] = S_INT;
	((int*) TableItem->data)[2] = S_INT;
	((int*) TableItem->data)[3] = S_EOF;

	// int compare(String s1, String s2);
	TableItem = htab_insert_item(global_table_symbols, "ifj16.compare");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_INT;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 2;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (3*sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_STRING;
	((int*) TableItem->data)[1] = S_STRING;
	((int*) TableItem->data)[2] = S_EOF;

	// int find(String s, String search);
	TableItem = htab_insert_item(global_table_symbols, "ifj16.find");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_INT;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 2;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (3*sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_STRING;
	((int*) TableItem->data)[1] = S_STRING;
	((int*) TableItem->data)[2] = S_EOF;

	// String sort(String s);
	TableItem = htab_insert_item(global_table_symbols, "ifj16.sort");
	if(TableItem == NULL)
	{
		fprintf(stderr, "Intern fault. Parser cannot insert intern function into Table of symbols (malloc problem).\n");
		return ERR_INTERN_FAULT;
	}
	TableItem->data_type = S_STRING;
	TableItem->func_or_var = 2;
	TableItem->number_of_arguments = 1;
	TableItem->initialized = 1;
	TableItem->data = mem_alloc (2*sizeof(int));
	if(TableItem->data == NULL)
	{
		fprintf(stderr, "Intern fault. Parser allocate place for data of function.\n");
		return ERR_INTERN_FAULT;
	}
	((int*) TableItem->data)[0] = S_STRING;
	((int*) TableItem->data)[1] = S_EOF;

	return 0;
}
