#include "scanner.h"
#include "error.h"

int get_meaning(char *word) {
	if (word == NULL) return -1;
	if (!strcmp(word, "boolean")) return S_BOOLEAN;
	if (!strcmp(word, "break")) return S_BREAK;
	if (!strcmp(word, "class")) return S_CLASS;
	if (!strcmp(word, "continue")) return S_CONTINUE;
	if (!strcmp(word, "do")) return S_DO;
	if (!strcmp(word, "double")) return S_DOUBLE;
	if (!strcmp(word, "else")) return S_ELSE;
	if (!strcmp(word, "false")) return S_FALSE;
	if (!strcmp(word, "for")) return S_FOR;
	if (!strcmp(word, "if")) return S_IF;
	if (!strcmp(word, "int")) return S_INT;
	if (!strcmp(word, "return")) return S_RETURN;
	if (!strcmp(word, "String")) return S_STRING;
	if (!strcmp(word, "static")) return S_STATIC;
	if (!strcmp(word, "true")) return S_TRUE;
	if (!strcmp(word, "void")) return S_VOID;
	if (!strcmp(word, "while")) return S_WHILE;
	return 0;
}

int is_num_literal(char *word, unsigned len) {
	if (word == NULL || len == 0) return -1;
	int e = 0, dot = 0;
	for (unsigned i = 0; i<len; i++) {
		if (word[i] == 'e' || word[i] == 'E') {
			if (e) return 0;
			e = 1;
		}
		if (word[i] == '.') {
			if (dot || e) return 0;
			dot = 1;
		}
		else if ((word[i] < '0' || word[i] > '9') && word[i] != 'e')
			return 0;
	}
	if (dot) return TYPE_DOUBLE;
	return TYPE_INT;
}

int is_simple_ident(char *word, unsigned len) {
	if (word == NULL || len == 0) return 0;
	if ((word[0] != '$' && word[0] < 'A') || word[0] > 'z' || (word[0] > 'Z' && word[0] < 'a' && word[0] != '_'))
		return 0;
	for (unsigned i = 1; i < len; i++) {
		if(word[i] == '$' || word[i] == '_' || (word[i] >= '0' && word[i] <= '9') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z'))
			continue;
		else
			return 0;
	}
	return 1;
}

int is_full_ident(char *word, unsigned len) {
	if (word == NULL || len < 3) return 0;
	char *temp;
	unsigned i, j;
	temp = malloc(len);
	if (temp == NULL)
		error_msg(ERR_INTERPRET, 'f');
	for (i = 0; word[i] != '.' && i<len; i++)
		temp[i] = word[i];
	if (i == len || !is_simple_ident(temp, i-1)) {
		free(temp);
		return 0;
	}
	i++;
	for (j = 0; i < len; i++) {
		temp[j] = word[i];
		j++;
	}
	if (!is_simple_ident(temp, j-1)) {
		free(temp);
		return 0;
	}
	free(temp);
	return 1;
}

// TODO neukoncujici zna */ --> bere se to jako chyba nebo je zbytek souboru komentar?
int skip_comment(unsigned comment_type, FILE *f) {
	int c;
	if (comment_type == LINE_COMMENT) {
		while ((c = fgetc(f)) != EOF) {
			if (c == '\n') {
				LINE_NUM++;
				return 0;
			}
		}
	}
	else {
		char expected = 0;
		while ((c = fgetc(f)) != EOF) {
			if (c == '\n') LINE_NUM++;
			if (c == '*') { 
				expected = 1;
				continue;
			}
			if (c == '/' && expected) return 0;
			expected = 0;
		}
		return -1;
	}
	return 1; // reached EOF
}

unsigned load_string(FILE *f, char *word, unsigned max) {
	int c, prev = -1;
	unsigned i = 0;
	while(((c = fgetc(f)) != EOF) && i < max) {
		if (c == '"' && prev != '\\') {
			word[i] = '\0';
			return i;
		}
		word[i] = prev = c;
		i++;
	}
	if (i == max && c != EOF) return max+1;
	return UINT_MAX;
}

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
				*end_char = (int)LINE_COMMENT;
				return i;
			}
			else if (c == '*') {
				word[i] = '\0';
				*end_char = (int)BLOCK_COMMENT;
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
		return i;
	}
	*end_char = ERR_REACHED_MAX;	
	return UINT_MAX;
}
