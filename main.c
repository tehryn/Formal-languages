#include "scanner.h"
#include <ctype.h>
#include <limits.h>
#define MAX_LEN 100
unsigned read_word(FILE *f, char *word, unsigned max) {
	int c;
	unsigned i = 0;
	
	while (((c = fgetc(f)) != EOF) && i < max) {
		if (c == ' ' || c == '\t' || c == '\n') {
			word[i] = '\0';
			return i;
		}	
		
		if (c == '/') {
			c = fgetc(f);
			if (c == '/') {
				word[i] = '\0';
				return LINE_COMMENT;
			}
			else if (c == '*') {
				word[i] = '\0';
				return BLOCK_COMMENT;
			}
			else {
				ungetc(c, f);
				word[i] = '/';
			}
		}
		
		else {
			word[i] = c;
			i++;		
		}
	}
	if (i == max) return max;
	word[i] = 0;
	return UINT_MAX;
}
int main (int argc, char **argv) {
//	if (argc != 2) error_msg();
	FILE *f;
	f = fopen(argv[1], "r");
	char *word;
	word = malloc(MAX_LEN);	
	unsigned lines = 1, temp;
	while (lines != MAX_LEN && lines != UINT_MAX) {
		lines = read_word(f, word, MAX_LEN);
		if (lines) {
			printf("%s\t:", word);
			temp = is_key_word(word);
			printf("\tkeyword %u", temp);
			temp = is_simple_ident(word, lines);
			printf(",\tsimple ident %u", temp);
			temp = is_full_ident(word, lines);
			printf(",\tfull ident %u", temp);
			temp = is_num_literal(word, lines);
			printf(",\tnum literal %u\n", temp);
		}
	}
	free(word);
	return 0;
}
