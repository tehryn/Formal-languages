#include "scanner.h"
#include "garbage_collector.h"
#include "error.h"
#include "str_prcs.c" // TODO knihovna
#include "parser.h"
#include "htab.h"
//#define TEST_PARSER
//#define TEST_TABLE

#define TEST_SCANNER // dont forget to enable TEST_TOKEN or TEST_STR_PRCS
#define TEST_TOKEN
//#define TEST_STR_PRCS

FILE *f;

int ERROR_CHECK = 0;

int expr_analyze(token * t, stack_int_t *s)
{
	(void) s;
	if (t->id == S_RIGHT_PARE || t->id == S_RIGHT_BRACE || t->id == S_SEMICOMMA)
		return -1;
	else
		return 0;
}

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
	switch (analyze_syntax()) {
		case -1: printf("failed \n"); break;
		default: printf("%s\n", "succes");
	}
#endif
// testing htab
#ifdef TEST_TABLE
	char line[10], prev[10] = {0,};
	htab_t *table;
	table = htab_init(1001);
		while(fgets(line, 10, stdin) != NULL) {
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

#ifdef TEST_SCANNER
token *t;
do {
	t = (token *) mem_alloc(sizeof(token), &l);
	*t = get_token();
// testing scanner
#ifdef TEST_TOKEN

		if (t->ptr) {
			printf("+-------------------\n");
			printf("| line num: %d\n", LINE_NUM);
			printf("| token id: %u\n", t->id);
			printf("| token ptr: %s\n", (char *)t->ptr);
			printf("+-------------------\n");
		}
		else {
			printf("+-------------------\n");
			printf("| line num: %d\n", LINE_NUM);
			if (t->id == S_EQAL)
				printf("| token id: ==\n");
			else if (..)
				printf("| token id: EOF\n");
			printf("| token ptr: NULL\n");
			printf("+-------------------\n");
		}
#endif

// testing string_process
#ifdef TEST_STR_PRCS

		switch(t->id) {
			case TYPE_DOUBLE:
				printf("+-------------------\n");
				printf("| convert to double: %s\n", (char *)t->ptr);
				printf("| - - - - - - - - - \n");
				printf("| vysledek: %f\n", *((double *) string_process(t->id, (char *)t->ptr)));
				printf("+-------------------\n");
				break;
			case TYPE_INT:
				printf("+-------------------\n");
				printf("| convert to int: %s\n", (char *)t->ptr);
				printf("| - - - - - - - - - \n");
				printf("| vysledek: %i\n", *((int *) string_process(t->id, (char *)t->ptr)));
				printf("+-------------------\n");
				break;
			case TYPE_STRING:
				printf("+-------------------\n");
				printf("| convert string: [%s]\n", (char *)t->ptr);
				printf("| - - - - - - - - - \n");
				printf("| vysledek: [%s]\n", ((char *) string_process(t->id, (char *)t->ptr)));
				printf("+-------------------\n");
				break;
			default: break;
		}
#endif
	} while (t->id > 0 && t->id != S_EOF);
#endif
	free_memory(&l);
	free(SCANNER_WORD);
	fclose(f);
	printf("\nKONEC\n");
	return 0;
}
