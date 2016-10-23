#include "scanner.h"
#include "memory_keeper.h"
#include "error.h"
#define MAX_LEN 1000
FILE *f;
int ERROR_CHECK = 0;

int main (int argc, char **argv) {
	if (argc != 2) {
		// TODO
	}
	f = fopen(argv[2], "r");
	if (f == NULL) {
		// TODO
	}
	
	fclose(f);
	printf("\nKONEC\n");
	return 0;
}
