#include "help.h"


int stack_expression_init ( struct stack_expresion* s, int size  )
{
	s->arr = (token *)malloc(size * sizeof(token));
	if (s->arr !=NULL )
	{
		s->top = -1;
		s->size = size;
		return 0;
	}
	else
		return -1;
}


int stack_expression_destroy ( struct stack_expresion* s )
{
	if (s->arr != NULL )
	{
		free(s->arr);
		s->top = -1;
		s->size = 0;
		s->arr=NULL;
		return 0;
	}
	else
		return -1;
}


int stack_expression_empty ( const struct stack_expresion* s )
{
	return (s->top == -1);
}


int stack_expression_full ( const struct stack_expresion* s )
{
	return (s->top == ( s->size - 1) );
}


int stack_expression_top ( struct stack_expresion* s, token* t  )
{
	if ( stack_expression_empty(s) )
		return 1;

	else
	{
		*t = s->arr[s->top];
		return 0;
	}
}




int stack_expression_pop ( struct stack_expresion* s, token* t )
{
	if ( stack_expression_empty(s) )
		return 1;
	else
	{
		if (t != NULL)
			*t = s->arr[s->top];

		s->top -= 1;
		return 0;
	}
}


int stack_expression_push ( struct stack_expresion* s, token t )
{
	if ( stack_expression_full(s) )
	{
		token *new_arr = NULL;
		new_arr = (token *)realloc(s->arr, 2 * s->size * sizeof(token));
		if (new_arr == NULL)
			return 1;
		else
		{
			s->arr=new_arr;
			s->size *= 2;
		}
	}

	s->top += 1;
	s->arr[s->top] = t;
	return 0;
}

unsigned hash_function(const char *str, unsigned htab_size)
{
	unsigned int h=0;
	const unsigned char *p;
	for( p = (const unsigned char*) str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % htab_size;
}



htab_t * htab_init(unsigned size)
{
	htab_t * htab = (htab_t *) malloc(sizeof(htab_t));
	if (htab == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_init)");
		return NULL;
	}

	htab->htab_size = size;
	htab->hash_fun_ptr = &hash_function;
	htab->number_items = 0;
	htab->ptr = (htab_item**) malloc(size * sizeof(htab_item*));
	for (unsigned i = 0; i < size; i++)
		htab->ptr[i] = NULL;
	return htab;
}

htab_t * htab_init2(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size))
{
	htab_t * htab;
	if ((htab = (htab_t *) malloc(sizeof(htab_t) + size * sizeof(htab_item*))) == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_init2)");
		return NULL;
	}
	htab->htab_size = size;
	htab->hash_fun_ptr = hash_fun;
	htab->number_items = 0;
	htab->ptr = (htab_item**) malloc(size * sizeof(htab_item*));
	for (unsigned i = 0; i < size; i++)
		htab->ptr[i] = NULL;
	return htab;
}



void htab_clear_items(htab_t * T)
{
	htab_item * item; // item that we will free
	for (unsigned i = 0; i < T->htab_size; i++)
	{
		while (T->ptr[i] != NULL)
		{
			item = T->ptr[i];
			T->ptr[i] = T->ptr[i]->next_item; // store next item
			free(item->key);
			free(item->data);
			free(item);
			item = NULL;
		}
	}
}

void htab_free_all(htab_t * T)
{
	if (T != NULL)
	{
		htab_clear_items(T);
		free(T->ptr);
		free(T);
		T = NULL;
	}
}


htab_item * htab_find_item(htab_t * T, const char * key) // NULL if not there
{
	unsigned index = T->hash_fun_ptr(key, T->htab_size);
	htab_item * item = T->ptr[index];

	while (item != NULL)
	{
		if (!strcmp(item->key, key))
			return item; // nalezeno
		else
			item = item->next_item; // cyklit
	}
	return NULL; // nenalezeno
}

htab_item * malloc_item(const char * key)
{
	htab_item * item = (htab_item *) malloc(sizeof(htab_item));
	if (item == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_insert_item)");
		return NULL;
	}

	unsigned key_len = strlen(key) + 1;
	item->key = (char *) malloc(key_len);
	if (item->key == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_insert_item)");
		free(item);
		return NULL;
	}

	strncpy(item->key, key, key_len);
	item->data_type = 0;
	item->func_or_var = 0;
	item->data = NULL;
	item->number_of_arguments = 0;
	item->initialized = 0;
	item->next_item = NULL;

	return item;
}

htab_item * htab_insert_item(htab_t * T, const char * key)
{
	unsigned index = T->hash_fun_ptr(key, T->htab_size);
	htab_item * item = T->ptr[index];

	if (item == NULL)
	{
		T->ptr[index] = malloc_item(key);
		return T->ptr[index]; // could be NULL
	}

	// item != NULL
	while (item->next_item != NULL)
		item = item->next_item;

	// jsme na poslednim prvku
	item->next_item = malloc_item(key);
	return item->next_item; // could be NULL
}


