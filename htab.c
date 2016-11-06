#include "htab.h"
#include "garbage_collector.h"
#include "error.h"


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
	htab_t * htab;
	if ((htab = (htab_t *) malloc(sizeof(htab_t) + size * sizeof(htab_item*))) == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_init)");
		return NULL;
	}

	htab->htab_size = size;
	htab->hash_fun_ptr = &hash_function;
	htab->number_items = 0;
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
			T->ptr[i] = T->ptr[i]->next_item; //store next item
			free(item->key);
			free(item->data);
			free(item);
		}
	}
}

void htab_free_all(htab_t * T)
{
	htab_clear_items(T);
	free(T);
}



htab_item* htab_find_add_item(htab_t * T, const char * key, unsigned scope, unsigned data_type)
{
	unsigned index = T->hash_fun_ptr(key, T->htab_size);
	htab_item * item = T->ptr[index];

	if (item == NULL)
	{
		T->ptr[index] = add_item(key, scope, data_type);
		return T->ptr[index]; // could be NULL
	}

	// item != NULL
	while (item->next_item != NULL)
	{
		if (!strcmp(item->key, key)) // porovnani scope ???
			return item; // nalezen
		else
			item = item->next_item; // cyklit
	}
	// jsme na poslednim prvku
	if (!strcmp(item->key, key)) // porovnani scope ???
		return item; // nalezen
	else
	{
		item->next_item = add_item(key, scope, data_type);
		return item->next_item;
	}
}
htab_item* add_item(const char * key, unsigned scope, unsigned data_type)
{
	htab_item * item = (htab_item *) malloc(sizeof(htab_item));
	if (item == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. add_item)");
		return NULL;
	}

	unsigned key_len = strlen(key);
	item->key = (char *) malloc(sizeof(key_len));
	if (key == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. add_item)");
		free(item);
		return NULL;
	}

	strncpy(item->key, key, key_len);
	item->scope = scope;
	item->data_type = data_type;
	item->next_item = NULL;
	return item;
}


/*
void htab_foreach(htab_t *t, void (*func)(char*, unsigned)) {
	htab_listitem *item;
	for (unsigned i = 0; i < t->htab_size; i++) {
		item = t->ptr[i];
		while (item != NULL) {
			func(item->key, item->data);
			item = item->next;
		}
	}
}
*/
