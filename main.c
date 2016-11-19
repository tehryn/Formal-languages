#include "scanner.h"
#include "garbage_collector.h"
#include "error.h"
#include "str_prcs.c" // TODO knihovna
#include "parser.h"
#include "htab.h"

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
// testing parser
/*	switch (analyze_syntax()) {
		case -1: printf("failed \n"); break;
		default: printf("%s\n", "succes");
	}
*/
//	token *t = (token *) mem_alloc(sizeof(token), &l);
	// testing htab
	char line[11];
	htab_t *table;
	table = htab_init(1001);
		while(fgets(line, 10, stdin) != NULL) {
			htab_insert_item(table, line);
		}
		htab_free_all(table);
//do {
// testing scanner
/*		if (t->ptr) {
			printf("+-------------------\n");
			printf("| line num: %d\n", LINE_NUM);
			printf("| token id: %u\n", t->id);
			printf("| token ptr: %s\n", (char *)t->ptr);
			printf("+-------------------\n");
		}
		else {
			printf("+-------------------\n");
			printf("| line num: %d\n", LINE_NUM);
			printf("| token id: %c (%i)\n", t->id, t->id);
			printf("| token ptr: NULL\n");
			printf("+-------------------\n");
		}
*/
// testing string_process
/*
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
				printf("| convert string: %s\n", (char *)t->ptr);
				printf("| - - - - - - - - - \n");
				printf("| vysledek: %s\n", ((char *) string_process(t->id, (char *)t->ptr)));
				printf("+-------------------\n");
				break;
			default: break;
		}
*/
/*		t = (token *) mem_alloc(sizeof(token), &l);
		*t = get_token();
	} while (t->id > 0 && t->id != S_EOF);
*/
	free_memory(&l);
	free(SCANNER_WORD);
	fclose(f);
	printf("\nKONEC\n");
	return 0;
}
