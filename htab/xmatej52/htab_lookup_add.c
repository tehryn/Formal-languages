/*
 * Author: Jiri Matejka
 * Login: xmatej52
 * School: VUT FIT, Brno
 * Date: 22-4-2016
 * Module: IJC-DU2 htab_lookup_add.c
 * gcc version 4.9.2 (Debian 4.9.2-10)
 */
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"htable.h"

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
