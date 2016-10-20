#include "scanner.h"
#define MAX_LEN 1000

unsigned LINE_NUM = 0;

int main (int argc, char **argv) {
	if (argc != 2) error_msg(ERR_OSTATNI,'f');
	FILE *f;
	f = fopen(argv[1], "r");
	if (f == NULL) error_msg(ERR_OSTATNI,'f');
	char *word;
	char *string;
	if ((word = malloc(MAX_LEN)) == NULL) {
		fclose(f);
		error_msg(ERR_OSTATNI,'f');
	}	
	if ((string = malloc(MAX_LEN)) == NULL) {
		free(word);
		fclose(f);
		error_msg(ERR_OSTATNI,'f');
	}
	unsigned chars = 1, temp;
	int end_char = 0, check;
	while (end_char != EOF) {
		chars = read_word(f, word, MAX_LEN, &end_char);
		if (chars) {
			printf("%u:\t%s\t:", LINE_NUM, word);
			temp = get_meaning(word);
			printf("\tkeyword %u", temp);
			temp = is_simple_ident(word, chars);
			printf(",\tsimple ident %u", temp);
			temp = is_full_ident(word, chars);
			printf(",\tfull ident %u", temp);
			temp = is_num_literal(word, chars);
			printf(",\tnum literal %u\tlen %u\n", temp, chars);
		}
		switch (end_char) {
			case '\n':
				LINE_NUM++;
			case '\t':
			case '\r':
			case ' ': 
			break;
			case LINE_COMMENT: 
			case BLOCK_COMMENT:
				check = skip_comment(end_char, f);
				if (check == 1)
					end_char = EOF;
				else if (check == -1) {
					free(word);
					free(string);
					fclose(f);
					error_msg(ERR_SYNTAKTICKA_ANALYZA, 'f');
				}
				break;
			case EOF: 
				printf("%u:\t-----\t:\tEOF\n", LINE_NUM);
				break;
			case ';':
			case '(':
			case ')':
			case '{':
			case '}':
			case '+':
			case '-':
			case '=':
			case '*':
			case '/':
			case ',':
			case '<':
			case '>':
				printf("%u:\t-----\t:\t%c\n", LINE_NUM, end_char);
				break;
			case '"':
				check = load_string(f, string, MAX_LEN);
				if (chars == MAX_LEN+1) {	
					free(word);
					fclose(f);
					error_msg(ERR_OSTATNI, 'f');
				}
				printf("loaded string:\t'%s'\n", string);
				break;
			case ERR_REACHED_MAX:
				free(word);
				free(string);
				fclose(f);
				error_msg(ERR_OSTATNI, 'f');
			default:
				printf("\n\n\n\n\n FUCK FUCK FUCK FUCK\n Neznamy znak! znak == '%c' s hodnotou %i", end_char, end_char);
				return 1;
		}
	}
	fclose(f);
	free(word);
	free(string);
	printf("\nKONEC\n");
	return 0;
}
