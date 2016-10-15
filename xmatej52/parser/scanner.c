#include "scanner.h"
#include "error.h"

unsigned LINE_NUM = 0;

int is_key_word(char *word) {
	if (word == NULL) return -1;
	if (!strcmp(word, "boolean")) return BOOLEAN;
	if (!strcmp(word, "break")) return BREAK;
	if (!strcmp(word, "class")) return CLASS;
	if (!strcmp(word, "continue")) return CONTINUE;
	if (!strcmp(word, "do")) return DO;
	if (!strcmp(word, "double")) return DOUBLE;
	if (!strcmp(word, "else")) return ELSE;
	if (!strcmp(word, "false")) return FALSE;
	if (!strcmp(word, "for")) return FOR;
	if (!strcmp(word, "if")) return IF;
	if (!strcmp(word, "int")) return INT;
	if (!strcmp(word, "return")) return RETURN;
	if (!strcmp(word, "String")) return STRING;
	if (!strcmp(word, "static")) return STATIC;
	if (!strcmp(word, "true")) return TRUE;
	if (!strcmp(word, "void")) return VOID;
	if (!strcmp(word, "while")) return WHILE;
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
		if (word[i] < '0' || word[i] > '9')
			return 0;
	}
	if (dot) return TYPE_DOUBLE;
	return TYPE_INT;
}

int is_simple_ident(char *word, unsigned len) {
	if (word == NULL || len == 0) return -1;
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
	if (word == NULL || len == 0) return -1;
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
int skip_comment(int comment_type, FILE *f) {
	int c;
	if (comment_type == LINE_COMMENT) {
		while ((c = fgetc(f)) != EOF) {
			if (c == '\n') return 0;
		}
	}
	else {
		char expected;
		while ((c = fgetc(f)) != EOF) {
			if (c == '*') expected = 1;
			if (c == '/' && expected) return 0;
		}
		error_msg(ERR_SYNTAKTICKA_ANALYZA, 'f');
	}
	return 1; // reached EOL
}

int run_scanner(FILE *f) {
	return 0;
}
