#include "scanner.h"

#define MAX_LEN 100
unsigned read_word(FILE *f, char *word, unsigned max) {
	int c;
	unsigned i = 0;
	
	while (((c = fgetc(f)) != EOF) && i < max) {
		if (c == ' ' || c == '\t' || c == '\n'  || c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == '"' || c == '*' || c == '/' || c == '+' || c == '-') {
			word[i] = '\0';
			ungetc(c, f);
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
	if (i == max && c != EOF) return max+1;
	word[i] = '\0';
	return UINT_MAX;
}

int main (int argc, char **argv) {
	if (argc != 2) error_msg(ERR_OSTATNI,'f');
	FILE *f;
	f = fopen(argv[1], "r");
	if (f == NULL) error_msg(ERR_OSTATNI,'f');
	char *word;
	int c;
	word = malloc(MAX_LEN);	
	unsigned chars = 1, temp;
	int check;
	while (chars != UINT_MAX) {
		chars = read_word(f, word, MAX_LEN);
		if (chars == MAX_LEN+1) {	
			free(word);
			fclose(f);
			error_msg(ERR_OSTATNI, 'f');
		}
		if (chars == LINE_COMMENT) {
			chars = strlen(word);
			check = skip_comment(LINE_COMMENT, f);
			if (check == 1); //TODO
		}
		if (chars == BLOCK_COMMENT) {
			chars = strlen(word);
			check = skip_comment(LINE_COMMENT, f);
			if (check == -1) {
				free(word);
				fclose(f);
				error_msg(ERR_SYNTAKTICKA_ANALYZA, 'f');				
			}
		}
		if (chars) {
			printf("%s\t:", word);
			temp = get_meaning(word);
			printf("\tkeyword %u", temp);
			temp = is_simple_ident(word, chars);
			printf(",\tsimple ident %u", temp);
			temp = is_full_ident(word, chars);
			printf(",\tfull ident %u", temp);
			temp = is_num_literal(word, chars);
			printf(",\tnum literal %u", temp);
		}
		c = fgetc(f);
		if (c != EOF && chars)
			printf(",\tended by '%c'\n", c);
		if (c == '"') {
			chars = load_string(f, word, MAX_LEN);
			if (chars == MAX_LEN+1) {	
				free(word);
				fclose(f);
				error_msg(ERR_OSTATNI, 'f');
			}
			printf("loaded string:\t'%s'\n", word);
		}
		
				
	}
	fclose(f);
	free(word);
	return 0;
}
