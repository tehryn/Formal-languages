#include "scanner.h"
#include "garbage_collector.h"
#include "error.h"
#include "parser.h"
#include "ial.h"
#include "structures.h"
//#define TEST_PARSER
//#define TEST_TABLE
//#define TEST_STRUCTURES
#define TEST_SCANNER

FILE *f;

/*
(fprintf[ ]*\(stderr, ["])([a-z| |\.|:|%|'|{|\\|(|}|)|;|_|]+["])(,[ ]*LINE_NUM\))
$1 In %s on line %u: $2, __FILE__, __LINE__$3
 */

/**
 * Make simple ident from full ident
 * @param  str Pointer to full ident
 * @return     Pointer to new string. If input string is full ident, new string \
 *             equal to a string after first dot otherwise return copy of input string
 * @post       Return value is not NULL
 */
char *full_ident2simple(char *str) {
	unsigned i = 0;
	while (str[i] && str[i++] != '.');
	if (!str[i]) {
		char *result = (char *) malloc(strlen(str)+1);
		if (result == NULL) {
			return NULL;
		}
		return strcpy(result, str);
	}
	size_t size = strlen(&str[i]) + 1;
	char *result = (char *) malloc(size);
	if (result == NULL) {
		return NULL;
	}
	return strcpy(result, &str[i]);
}

int ERROR_CHECK = 0;

extern char* SCANNER_WORD;

int main (int argc, char **argv) {
	int return_value = 0;
	mem_list_t l;
	mem_list_t_init(&l);
	if (argc != 2) {
		fprintf(stderr, "Invalid arguments\n");
		return 99;
	}
	f = fopen(argv[1], "r");
	if (f == NULL) {
		fprintf(stderr, "Could not open file '%s'\n", argv[1]);
		return 99;
	}
#ifdef TEST_STRUCTURES
	htab_t* table;
	htab_t *tables[19];
	array_htab array;
	stack_htab stack;
	array_string arr_str;
	stack_htab_init(&stack);
	array_htab_init(&array);
	array_string_init(&arr_str);
	const char *chars[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s"}; //19
		for (int i = 0; i < 19; i++) {
			tables[i] = htab_init(7);
			htab_insert_item(tables[i], chars[i]);
			stack_htab_push(&stack, tables[i]);
			array_htab_insert(&array, tables[i]);
			array_string_insert(&arr_str, chars[i]);
		}
		printf("======--ARRAY--======\n");
		for (int i = 0; i < 19; i++) {
			table = array_htab_get_item(&array, i);
			if (htab_find_item(table, chars[i]) != NULL) {
				printf("| Succes\n");
			}
			else {
				printf("| ERROR\n");
			}
		}
		printf("======--STACK--======\n");
		for (int i = 0; i < 19; i++) {
			table = stack_htab_get_item(&stack, i);
			if (htab_find_item(table, chars[18-i]) != NULL) {
				printf("| Succes\n");
			}
			else {
				printf("| ERROR\n");
			}
		}
		printf("======--ARR_STR--======\n");
		char *str;
		for (int i = 0; i < 19; i++) {
			str = array_string_find(&arr_str, chars[i]);
			if (!strcmp(chars[i], str)) {
				printf("| Succes\n");
			}
			else {
				printf("| ERROR\n");
			}
		}
		array_string_destroy(&arr_str);
		array_htab_destroy(&array);
		stack_htab_destroy(&stack);
#endif

// testing Parser
#ifdef TEST_PARSER
	return_value = parser();
#endif
// testing htab
#ifdef TEST_TABLE
	char line[10], prev[10] = {0,};
	htab_t *table;
	table = htab_init(17);
		while(fgets(line, 10, f) != NULL) {
			printf("+-------------------\n");
			printf("| lookig for: [%s]\n", prev);
			if (htab_find_item(table, prev)) {
				printf("| returned: [%s]\n", htab_find_item(table, prev)->key);
				if (!strcmp(htab_find_item(table, prev)->key, prev))
					printf("| result: MATCH\n");
				else
					printf("| result: ERROR\n");
			}
			else printf("| returned nil\n| result: MATCH\n");
			printf("+-------------------\n");
			strcpy(prev, line);
			htab_insert_item(table, line);
		}
		htab_free_all(table);
#endif

// testing scanner
#ifdef TEST_SCANNER
token *t;
do {
	t = (token *) mem_alloc(sizeof(token), &l);
	*t = get_token();
		if (t->ptr) {
			printf("+-------------------\n");
			printf("| line num: %d\n", LINE_NUM);
			printf("| token id: %u\n", t->id);
			if (t->id == TYPE_INT)
				printf("| token ptr: %i\n", *((int *)t->ptr));
			else if (t->id == TYPE_DOUBLE)
				printf("| token ptr: %f\n", *((double *)t->ptr));
			else
				printf("| token ptr: %s\n", (char *)t->ptr);
			printf("+-------------------\n");
		}
		else {
			printf("+-------------------\n");
			printf("| line num: %d\n", LINE_NUM);
			if (t->id == S_EQUAL)
				printf("| token id: ==\n");
			else if (t->id == S_LESS_EQUAL)
				printf("| token id: <=\n");
			else if (t->id == S_GREATER_EQUAL)
				printf("| token id: >=\n");
			else if (t->id == S_LESS)
				printf("| token id: <\n");
			else if (t->id == S_GREATER)
				printf("| token id: >\n");
			else if (t->id == S_OR)
				printf("| token id: ||\n");
			else if (t->id == S_AND)
				printf("| token id: &&\n");
			else if (t->id == S_NOT_EQUAL)
				printf("| token id: !=\n");
			else if (t->id == S_LEFT_PARE)
				printf("| token id: (\n");
			else if (t->id == S_RIGHT_PARE)
				printf("| token id: )\n");
			else if (t->id == S_LEFT_BRACE)
				printf("| token id: {\n");
			else if (t->id == S_RIGHT_BRACE)
				printf("| token id: }\n");
			else if (t->id == S_COMMA)
				printf("| token id: ,\n");
			else if (t->id == S_SEMICOMMA)
				printf("| token id: ;\n");
			else if (t->id == S_PLUS)
				printf("| token id: +\n");
			else if (t->id == S_MINUS)
				printf("| token id: -\n");
			else if (t->id == S_DIV)
				printf("| token id: /\n");
			else if (t->id == S_MUL)
				printf("| token id: *\n");
			else if (t->id == S_ASSIGNMENT)
				printf("| token id: =\n");
			else if (t->id == S_EOF)
				printf("| token id: EOF\n");
			else
				printf("WTF\n");
			printf("| token ptr: NULL\n");
			printf("+-------------------\n");
		}
	} while (t->id > 0 && t->id != S_EOF);
#endif
	free_memory(&l);
	free(SCANNER_WORD);
	fclose(f);
	return return_value;
}
