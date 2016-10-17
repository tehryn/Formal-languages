#include "scanner.h"
# define ERR_REACHED_MAX -100
#define MAX_LEN 1000
unsigned read_word(FILE *f, char *word, unsigned max, int *end_char) {
	int c;
	unsigned i = 0;
	
	while (((c = fgetc(f)) != EOF) && i < max) {
		if (isspace(c)  || c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == '"' || c == '*' || c == '+' || c == '-' || c == '=' || c == ',' || c == '<' || c == '>') {
			word[i] = '\0';
			*end_char = c;
			return i;
		}	
	
		if (c == '/') {
			c = fgetc(f);
			if (c == '/') {
				word[i] = '\0';
				*end_char = LINE_COMMENT;
				return i;
			}
			else if (c == '*') {
				word[i] = '\0';
				*end_char = BLOCK_COMMENT;
				return i;
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
	word[i] = '\0';
	if (c == EOF) {
		*end_char = EOF; 
		return EOF;
	}
	*end_char = ERR_REACHED_MAX;	
	return UINT_MAX;
}

int main (int argc, char **argv) {
	if (argc != 2) error_msg(ERR_OSTATNI,'f');
	FILE *f;
	f = fopen(argv[1], "r");
	if (f == NULL) error_msg(ERR_OSTATNI,'f');
	char *word;
	word = malloc(MAX_LEN);	
	unsigned chars = 1, temp;
	int end_char = 0, check;
	while (end_char != EOF) {
		chars = read_word(f, word, MAX_LEN, &end_char);
		if (!isspace(end_char)) {
			if (end_char == EOF) { printf("\t:\tEOF\n"); break;}
			else printf("\t:\t%c\n", end_char);
		}
		if (end_char == ERR_REACHED_MAX) {	
			free(word);
			fclose(f);
			error_msg(ERR_OSTATNI, 'f');
		}
		if (end_char == LINE_COMMENT) {
			check = skip_comment(LINE_COMMENT, f);
			if (check == 1) {}//TODO
		}
		if (chars == BLOCK_COMMENT) {
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
			printf(",\tnum literal %u\t len %u\n", temp, chars);
		}
		
		if (end_char == '"') {
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
	printf("\nKONEC\n");
	return 0;
}
