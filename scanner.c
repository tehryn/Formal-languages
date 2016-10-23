#include "scanner.h"
#include "error.h"

unsigned LINE_NUM = 0;
extern FILE *f;

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
	switch(c) {
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
		case '>': return c;
		default: return 0;
	}
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

char *load_string(char *word, unsigned *max) {
	int c, prev = -1;
	unsigned i = 0;
	while(((c = fgetc(f)) != EOF)) {
		if (c == '"' && prev != '\\') {
			word[i] = '\0';
			return word;
		}
		word[i] = prev = c;
		i++;
		if (i >= *max) { // predpoklad dalsiho zapisu -> nutna realokace pameti
			char *tmp = word;
			*max *= 2; // preteceni ???
			tmp = realloc(word, *max);
			if (tmp == NULL) {
				*max = 0;
				free(word);
				return NULL;
			}
			word = tmp;
		}
	}
	return NULL;
}

token get_token() {
	static char *word;
	static unsigned size = 0;
	if (size == 0) {
		word = malloc(S_SIZE);
		if (word == NULL) {
			//TODO
		}
		size = S_SIZE;
	}
	token new_token;
	int c = 0;
	unsigned i = 0;
	int token_found = 0;
	while (!token_found && (c = fgetc(f)) != EOF) {
		if (isspace(c)) {
			if (i) // if i != 0 we found token
				token_found = 1;
			continue;
		}	
		/* Are we at the begining of a comment? */
		if (c == '/') {
			c = fgetc(f);
			if (c == '/') {
				skip_comment(LINE_COMMENT);
				if (i) // if i != 0 we found token
					token_found = 1;
				continue;
			}
			else if (c == '*') {
				if (skip_comment(BLOCK_COMMENT) == -1) {
					ERROR_CHECK = ERR_WRONG_COMMENT_SYNTAX;
					continue;
				}
				if (i) // if i != 0 we found token
					token_found = 1;
				continue;	
			}
			else {
				/* we have to go back by 1 char */
				if (fseek(f, -1, SEEK_CUR) != 0)
					ERROR_CHECK = (int) ERR_FSEEK;
				c = '/'; // and also set c to prev value
			}	
		}
		
		if (c == '"') {
			if (i) {
				if (fseek(f, -1, SEEK_CUR) != 0)
					ERROR_CHECK = (int) ERR_FSEEK;
				token_found = 1;
				continue;
			}
			else {
				word = load_string(word, &size);
				if (word == NULL) {
					if (size == 0) {
						// TODO - realokace pameti selhala
					}
					// TODO - retezec nebyl validne ukoncen
				}
				
				new_token.id = TYPE_STRING;
				new_token.ptr = word;
				return new_token;
			}
		}
		/* Now we are not in comment nor in sequence of white chars so lets
		 check special chars */
		if (is_special_char(c)) {
			if (i) { // if i != 0 we found token
				/* we have to go back by 1 char */
				if (fseek(f, -1, SEEK_CUR) != 0)
					ERROR_CHECK = (int) ERR_FSEEK;
				token_found = 1;
				continue;
			}
			else {
				new_token.id = c;
				new_token.ptr = NULL;
				return new_token;
			}
		}
		if (i < size) { // Je bezpecne pristoupit na index i
			word[i] = c;
			i++;
		}
		else { // nutna realokace pameti
			char *tmp = word;
			size *= 2; // preteceni???
			tmp = realloc(word, size);
			if (tmp == NULL) {
				//TODO - dont forget to free word
			}
			else {
				word = tmp;
				word[i] = c;
				i++;
			}
		}
	}
	
	if (i < size) { // Je bezpecne pristoupit na index i
		word[i] = '\0';
	}
	else { // nutna realokace pameti
		char *tmp = word;
		size *= 2; // preteceni???
		tmp = realloc(word, size);
		if (tmp == NULL) {
			//TODO - dont forget to free word
		}
		else {
			word = tmp;
			word[i] = '\0';
		}
	}
	if (token_found) {	
		// token found! but what did we find???
		unsigned id = is_keyword(word);
		if (id) {
			new_token.id = id;
			new_token.ptr = word;
			//new_token.ptr = NULL;
			return new_token;
		}
		/* Ok, we did not found key word... Did we found number?*/
		id = is_num_literal(word, i);
		if (id) {
			new_token.id = id;
			new_token.ptr = word;
			return new_token;
		}
		id = is_full_ident(word, i);
		if (id) {
			new_token.id = FULL_IDENT;
			new_token.ptr = word;
			// new_token.ptr = najdi_polozku(word);
			return new_token;
		}
		id = is_simple_ident(word, i);
		if (id) {
			new_token.id = SIMPLE_IDENT;
			new_token.ptr = word;
			// new_token.ptr = najdi_polozku(word);
			return new_token;
		}
	}
	new_token.id = 0;
	new_token.ptr = NULL;
	return new_token;
}
