/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: TODO
**/
#include "scanner.h"
#include "error.h"

#define SPEC_CHAR_FSEEK(spec) (((spec) == S_EQUAL || (spec) == S_LESS_EQUAL || (spec) == S_GREATER_EQUAL || (spec) == S_NOT_EQUAL)?-2:-1)

unsigned LINE_NUM = 1;
extern FILE *f;
char *SCANNER_WORD;


int is_keyword(char *word) {
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

int is_special_char(char c) {
	int state = 0;
	switch(c) {
		case ';': return S_SEMICOMMA;
		case '(': return S_LEFT_PARE;
		case ')': return S_RIGHT_PARE;
		case '{': return S_LEFT_BRACE;
		case '}': return S_RIGHT_BRACE;
		case '+': return S_PLUS;
		case '-': return S_MINUS;
		case '*': return S_MUL;
		case '/': return S_DIV;
		case ',': return S_COMMA;
		case '=': state = 1; break;
		case '<': state = 2; break;
		case '>': state = 3; break;
		case '!': state = 4; break;
		case '|':
			c = fgetc(f);
			if (c == '|') return S_OR;
			else {
				ungetc(c, f);
				return 0;
			}
		case '&':
			c = fgetc(f);
			if (c == '&') return S_AND;
			else {
				ungetc(c, f);
				return 0;
			}
		default: return 0;
	}
	c = fgetc(f);
		if( c == '=') {
			switch (state) {
				case 1: return S_EQUAL;
				case 2: return S_LESS_EQUAL;
				case 3: return S_GREATER_EQUAL;
				default: return S_NOT_EQUAL;
			}
		}
		else {
			ungetc(c, f);
			switch (state) {
				case 1: return S_ASSIGNMENT;
				case 2: return S_LESS;
				case 3: return S_GREATER;
				default: return 0;
			}
		}
		return 0;
}

int is_num_literal(char *word, unsigned len) {
	if (word == NULL || len == 0) return -1;
	int e = 0, dot = 0, sign = 0;
	if (!isdigit(word[0])) return 0;
	for (unsigned i = 1; i < len; i++) {
		if (sign && (word[i] == '+' || word[i] == '-')) {
			sign = 0;
			continue;
		}
		sign = 0;
		if (word[i] == 'e' || word[i] == 'E') {
			if (e) return 0;
			sign = 1;
			dot = e = 1;
		}
		else if (word[i] == '.') {
			if (dot) return 0;
			dot = 1;
		}
		else if ((!isdigit(word[i]))) {
			return 0;
		}
	}
	if(!isdigit(word[len-1])) return 0;
	if (dot) return TYPE_DOUBLE;
	return TYPE_INT;
}

int is_simple_ident(char *word, unsigned len) {
	if (word == NULL || len == 0) return 0;
	if ((word[0] != '$' && word[0] < 'A') || word[0] > 'z' || (word[0] > 'Z' && word[0] < 'a' && word[0] != '_'))
		return 0;
	for (unsigned i = 1; i < len; i++) {
		if(word[i] == '$' || word[i] == '_' || isdigit(word[i]) || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z'))
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
	temp = (char *) malloc(len);
	if (temp == NULL) {
		return -1;
	}
	for (i = 0; word[i] != '.' && i<len; i++)
		temp[i] = word[i];
	if (i == len || !is_simple_ident(temp, i)) {
		free(temp);
		return 0;
	}
	i++;
	for (j = 0; i < len; i++) {
		temp[j] = word[i];
		j++;
	}
	if (!is_simple_ident(temp, j)) {
		free(temp);
		return 0;
	}
	free(temp);
	return 1;
}

int skip_comment(unsigned comment_type) {
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

static char *write_back(char *word, int *num, unsigned *idx, unsigned *max, int *check) {
	for (unsigned i = 0; num[i] != '\0'; i++) {
		if (*idx >= *max) { // predpoklad dalsiho zapisu -> nutna realokace pameti
			char *tmp = word;
			*max *= 2; // preteceni ??? TODO - tolik pameti asi mit k dispozici nemuzeme...
			tmp = (char *) realloc(word, *max);
			if (tmp == NULL) {
				free(word);
				*max = 0;
				return NULL;
			}
			word = tmp;
		}
		if (num[i] == EOF) {
			*check = EOF;
		}
		word[*idx] = num[i];
		*idx += 1;
	}
	return word;
}

char *load_string(char *word, unsigned *max) {
	int c, num[4] = {'\0',};
	int check = '1';
	unsigned i = 0;
	while(((c = fgetc(f)) != EOF)) {
		if (c == '\\') {
			c = fgetc(f);
			if (c == '\\') {
				;
			}
			else if (c == 'n') {
				c = '\n';
			}
			else if (c == '"') {
				;
			}
			else if (c == '\'') {
				;
			}
			else if (c == 't') {
				c = '\t';
			}
			else if (isdigit(c)) {
				num[0] = c;
				num[1] = fgetc(f);
				if (isdigit(num[1])) {
					num[2] = fgetc(f);
					if (isdigit(num[2])) {
						c = (num[0] - '0')*64 + (num[1] - '0')*8 + (num[2] - '0');
					}
					else {
						word = write_back(word, num, &i, max, &check);
						if (word == NULL) {
							return NULL;
						}
						if (check == EOF) {
							break;
						}
					}
				}
				else {
					num[2] = '\0';
					word = write_back(word, num, &i, max, &check);
					if (word == NULL) {
						return NULL;
					}
					if (check == EOF) {
						break;
					}
				}
			}
			else {
				ungetc(c, f);
				c = '\\';
			}
		}
		else if (c == '"') {
			c = '\0';
		}
		if (i >= *max) { // predpoklad dalsiho zapisu -> nutna realokace pameti
			char *tmp = word;
			*max *= 2; // preteceni ??? TODO - tolik pameti asi mit k dispozici nemuzeme...
			tmp = (char *) realloc(word, *max);
			if (tmp == NULL) {
				*max = 0;
				free(word);
				return NULL;
			}
			word = tmp;
		}
		word[i++] = c;
		if (c == '\0') {
			return word;
		}
	}
	return NULL;
}

token get_token() {
	static unsigned size = 0; // size of SCANNER_WORD
	token new_token; // return value
	if (size == 0) { // if SCANNER_WORD is not allocated - allocate
		SCANNER_WORD = (char *) malloc(S_SIZE);
		if (SCANNER_WORD == NULL) {
			fprintf(stderr, "Memory allocation failed");
			new_token.id = -1;
			new_token.ptr = NULL;
			return new_token;
		}
		size = S_SIZE; // setting size into default value
	}
	int c = 0; // for storing chars from stream
	unsigned i = 0; // for indexing SCANNER_WORD
	int token_found = 0; // variable that sais if we found token or not
	int spec = 0; // storing special characters like == or <=...
	int skip = 0; // variable that sais if we should skip calling function that determinate special chars
	void *number; // variable for storing return value of function str2num TODO
	while (!token_found && (c = fgetc(f)) != EOF) { // reading stream until we reach its end or we found token
		if (isspace(c)) { // if we found space
			if (c == '\n')
				LINE_NUM++;
			if (i) // if i != 0 we found token
				token_found = 1;
			continue;
		}
		/* Are we at the begining of a comment? */
		if (c == '/') {
			c = fgetc(f);
			if (c == '/') { // if true, we found comment section
				skip_comment(LINE_COMMENT); // skips line comment
				if (i) // if i != 0 we found token
					token_found = 1;
				continue;
			}
			else if (c == '*') {  // if true, we found comment section
				if (skip_comment(BLOCK_COMMENT) == -1) { // skips comment and also checks if we found end of commet as well
					fprintf(stderr, "In line %d expected '*/' (endless comment)\n", LINE_NUM);
					new_token.id = 0; // lexical error
					new_token.ptr = NULL;
					return new_token;
				}
				if (i) // if i != 0 we found token
					token_found = 1;
				continue;
			}
			else { // we need to return readed char back to stream
				/* we have to go back by 1 char */
				if (fseek(f, -1, SEEK_CUR) != 0) {
					fprintf(stderr, "Can't set offset in file!\n");
					new_token.id = -1; // intern error
					new_token.ptr = NULL;
					return new_token;
				}
				c = '/'; // and also set c to previos value
			}
		}

		if (c == '"') { // if true, we found string
			if (i) { // if we found token, we need to return by one char in stream
				if (fseek(f, -1, SEEK_CUR) != 0) {
					ERROR_CHECK = (int) ERR_FSEEK;
					fprintf(stderr, "Can't set offset in file!\n");
					new_token.id = -1; // Intern error
					new_token.ptr = NULL;
					return new_token;
				}
				token_found = 1;
				continue;
			}
			else {
				SCANNER_WORD = load_string(SCANNER_WORD, &size);
				if (SCANNER_WORD == NULL) {
					if (size == 0) {
						fprintf(stderr, "Memory reallocation failed\n");
						new_token.id = -1;
						new_token.ptr = NULL;
						return new_token;
					}
					fprintf(stderr, "ERROR: line: %u: expected \" at the end of string\n", LINE_NUM);
					new_token.id = 0;
					new_token.ptr = NULL;
					return new_token;
				}
				new_token.id = TYPE_STRING;
				new_token.ptr = SCANNER_WORD;
				return new_token;
			}
		}

		if (is_num_literal(SCANNER_WORD, i-2) && (SCANNER_WORD[i-1] == 'e' || SCANNER_WORD[i-1] == 'e') && (c == '-' || c == '+')) {
			if (isdigit(fgetc(f))) {
					skip = 1;
			}
			if (fseek(f, -1, SEEK_CUR) != 0) {
				fprintf(stderr, "Can't set offset in file!\n");
				new_token.id = -1;
				new_token.ptr = NULL;
				return new_token;
			}
		}
		/* Now we are not in comment nor in sequence of white chars so lets
		 check special chars */
		if (!skip && ((spec = is_special_char(c)) != 0)) {
			if (i) { // if i != 0 we found token
				/* we have to go back by 1 or 2 chars */
				if (fseek(f, SPEC_CHAR_FSEEK(spec), SEEK_CUR) != 0) {
					ERROR_CHECK = (int) ERR_FSEEK;
					fprintf(stderr, "Can't set offset in file!\n");
					new_token.id = -1;
					new_token.ptr = NULL;
					return new_token;
				}
				token_found = 1;
				continue;
			}
			else {
				new_token.id = spec;
				new_token.ptr = NULL;
				return new_token;
			}
		}
		if (i < size) { // Je bezpecne pristoupit na index i
			SCANNER_WORD[i] = c;
			i++;
		}
		else { // nutna realokace pameti
			char *tmp = SCANNER_WORD;
			size *= 2; // preteceni???
			tmp = (char *) realloc(SCANNER_WORD, size);
			if (tmp == NULL) {
				fprintf(stderr, "Memory reallocation failed\n");
				free(SCANNER_WORD);
				SCANNER_WORD = NULL;
				new_token.id = -1;
				new_token.ptr = NULL;
				return new_token;
			}
			else {
				SCANNER_WORD = tmp;
				SCANNER_WORD[i] = c;
				i++;
			}
		}
	} // konec while

	if (i < size) { // Je bezpecne pristoupit na index i
		SCANNER_WORD[i] = '\0';
	}
	else { // nutna realokace pameti
		char *tmp = SCANNER_WORD;
		size *= 2; // preteceni???
		tmp = (char *) realloc(SCANNER_WORD, size);
		if (tmp == NULL) {
			free(SCANNER_WORD);
			SCANNER_WORD = NULL;
			fprintf(stderr, "Memory reallocation failed\n");
			new_token.id = -1;
			new_token.ptr = NULL;
			return new_token;
		}
		else {
			SCANNER_WORD = tmp;
			SCANNER_WORD[i] = '\0';
		}
	}
	if (token_found) {
		// token found! but what did we find???
		int id = is_keyword(SCANNER_WORD);
		if (id) {
			new_token.id = id;
			new_token.ptr = SCANNER_WORD;
			//new_token.ptr = NULL;
			return new_token;
		}
		/* Ok, we did not found key word... Did we found number?*/
		id = is_num_literal(SCANNER_WORD, i);
		if (id) {

			number = string_process(id, SCANNER_WORD);
			if (number == NULL) {
				fprintf(stderr, "Memory allocation failed\n");
				new_token.id = -1;
				new_token.ptr = NULL;
			}

			new_token.id = id;
			new_token.ptr = number;
			return new_token;
		}
		id = is_full_ident(SCANNER_WORD, i);
		if (id == -1) {
			fprintf(stderr, "Memory reallocation failed\n");
			new_token.id = -1;
			new_token.ptr = NULL;
			return new_token;
		}
		if (id) {
			new_token.id = S_FULL_IDENT;
			new_token.ptr = SCANNER_WORD;
			return new_token;
		}
		id = is_simple_ident(SCANNER_WORD, i);
		if (id) {
			new_token.id = S_SIMPLE_IDENT;
			new_token.ptr = SCANNER_WORD;
			return new_token;
		}
		fprintf(stderr, "ERROR: line: %u: '%s' is not valide identifikator, numeric constant nor keyword\n", LINE_NUM, SCANNER_WORD);
		new_token.id = 0;
		new_token.ptr = NULL;
		return new_token;
	}
	new_token.id = S_EOF;
	new_token.ptr = NULL;
	return new_token;
}

int reset() {
	return fseek(f, 0, SEEK_SET);
}
