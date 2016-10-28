#include "scanner.h"
#include "memory_keeper.h"
#include "error.h"
#include "str_prcs.c"
#define MAX_LEN 1000
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
	token *t = mem_alloc(sizeof(token), &l);
	*t = get_token();
	char temp[1024];
	while (t->id > 0) {
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
		memset(temp, 0, 1024);
		switch(t->id) {
			case TYPE_DOUBLE:
				memcpy(temp, t->ptr, strlen(t->ptr));
				printf("convert to double: %s = %lf\n", temp, *((double *) string_process(t->id, (char *)t->ptr)));
				break;
			case TYPE_INT: 
				memcpy(temp, t->ptr, strlen(t->ptr));
				printf("convert to int: %s = %i\n", temp, *((int *) string_process(t->id, (char *)t->ptr)));
				break;
			case TYPE_STRING:
				memcpy(temp, t->ptr, strlen(t->ptr));
				printf("convert to string: %s = %s\n", temp, (char *) string_process(t->id, (char *)t->ptr));
				break;
			default: break;
		}
		t = mem_alloc(sizeof(token), &l);
		*t = get_token();
	}
	free_memory(&l);
	free(SCANNER_WORD);
	fclose(f);
	printf("\nKONEC\n");
	return 0;
}
