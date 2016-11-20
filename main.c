#include "scanner.h"
#include "garbage_collector.h"
#include "error.h"
#include "parser.h"
#include "htab.h"
#define TEST_PARSER
//#define TEST_TABLE

//#define TEST_SCANNER
//#define TEST_TOKEN

FILE *f;

int ERROR_CHECK = 0;

extern char* SCANNER_WORD;

int main (int argc, char **argv) {
	mem_list_t l;
	mem_list_t_init(&l);
	if (argc != 2) {
		// TODO
	}
	f = fopen(argv[1], "r");
	if (f == NULL) {
		// TODO
	}
// testing
#ifdef TEST_PARSER
	switch (parser()) {
		case 0: printf("OK \n"); break;
		default: printf("%s\n", "ERROR");
	}
#endif
// testing htab
#ifdef TEST_TABLE
	char line[10], prev[10] = {0,};
	htab_t *table;
	table = htab_init(1001);
		while(fgets(line, 10, f) != NULL) {
			line[9] = '\0';
			printf("+-------------------\n");
			printf("| lookig for: %s\n", prev);
			if (htab_find_item(table, prev)) {
				printf("| returned: %s\n", htab_find_item(table, prev)->key);
				if (!strcmp(htab_find_item(table, prev)->key, prev))
					printf("| result: MATCH\n");
				else
					printf("| result: ERROR\n");
			}
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
	printf("\nKONEC\n");
	return 0;
}
