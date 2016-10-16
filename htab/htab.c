#include "htab.h"

unsigned int hash_function(const char *str, unsigned htab_size) {
	unsigned int h=0;
	const unsigned char *p;
	for( p = (const unsigned char*) str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % htab_size;
}


void htab_clear(htab_t *t) {
	htab_listitem *item; // item that will be free
	htab_listitem *temp; // store next item
	for (unsigned i = 0; i < t->htab_size; i++) {
		if (t->ptr[i] != NULL) {
			item = temp = t->ptr[i];
			while (temp->next != NULL) {
				temp = temp->next; //store next item
				free(item->key); // free key
				free(item); // free actual item
				item = temp; // next item
			}
			free(item->key);
			free(item);
			t->ptr[i] = NULL;
		}
	}
}

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

void htab_free(htab_t *t) {
	htab_clear(t);
	free(t);
}

htab_t *htab_init(unsigned size) {
	htab_t *htab;
	if ((htab = malloc(sizeof(htab_t) + size * sizeof(htab_listitem))) == NULL) 
		return NULL;

	htab->htab_size = size;
	htab->hash_fun_ptr = &hash_function;
	htab->n = 0;
	for (unsigned i = 0; i < size; i++)
		htab->ptr[i] = NULL;
	return htab;
}


htab_t *htab_init2(unsigned size, unsigned (*hashfn)(const char *str, unsigned htab_size)) {
	htab_t *htab;
	if ((htab = malloc(sizeof(htab_t) + size * sizeof(htab_listitem))) == NULL) 
		return NULL;
	htab->htab_size = size;
	htab->hash_fun_ptr = hashfn;
	htab->n = 0;
	for (unsigned i = 0; i < size; i++)
		htab->ptr[i] = NULL;
	return htab;
}

struct htab_listitem *htab_lookup_add(htab_t *t, const char *key) {
	unsigned index = t->hash_fun_ptr(key, t->htab_size);
	t->n++;
	/* for first item in list of items */
	if (t->ptr[index] == NULL) {
		/* Allocation for item */
		t->ptr[index] = malloc(sizeof(htab_listitem));
		if (t->ptr[index] == NULL)
			return NULL;
		
		/* Allocation for key */
		t->ptr[index]->key = malloc(strlen(key) +1);
		if (t->ptr[index]->key == NULL) {
			free(t->ptr[index]);
			return NULL;
		}
		
		/* Copy key into item */
		strcpy(t->ptr[index]->key, key);
		t->ptr[index]->data = 0;
		/* Its the first item, there is no second one */
		t->ptr[index]->next = NULL;
		return t->ptr[index];
	}

	/* There is at least one item on actual index */
	htab_listitem *temp = t->ptr[index];
	htab_listitem *l = temp;
	
	/* Find last item */
	while (l != NULL) {
		/* if key matches, return item */
		if (!strcmp(l->key, key))
			return l;
		l = l->next; // next item
		/* Last item was found */
		if (l == NULL)
			break;
		/* there is another item, so we need to keep previous item to set
		  item->next from NULL to l */
		temp = l;
	}
	
	/* Allocation for item */
	l =  malloc(sizeof(htab_listitem));
	if (l == NULL)
		return NULL;
	
	/* Allocation for key */
	l->key = malloc(strlen(key) +1);
	if (l->key == NULL) {
		free(l);
		return NULL;
	}
	
	/* setting pointer from NULL to l */
	if (temp->next == NULL) 
		temp->next = l;
	
	/* copy key to item */
	strcpy(l->key, key);
	l->data = 0;
	l->next = NULL;
	return l;
}
