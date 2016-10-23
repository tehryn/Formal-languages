#include "scanner.h"
#include "memory_keeper.h"
#include "error.h"
#define MAX_LEN 1000
FILE *f;
int ERROR_CHECK = 0;

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
	t->id = 1;
	while (t->id) {
		t = mem_alloc(sizeof(token), &l);
		*t = get_token();
		if (t->ptr) {
			printf("+-------------------\n");
			printf("| token id: %u\n", t->id);
			printf("| token ptr: %s\n", (char *)t->ptr);
			printf("+-------------------\n");
		}
		else {
			printf("+-------------------\n");
			printf("| token id: %c (%i)\n", t->id, t->id);
			printf("| token ptr: NULL\n");
			printf("+-------------------\n");
		}
	}
	free_memory(&l);
	fclose(f);
	printf("\nKONEC\n");
	return 0;
}
