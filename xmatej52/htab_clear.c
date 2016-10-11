/*
 * Author: Jiri Matejka
 * Login: xmatej52
 * School: VUT FIT, Brno
 * Date: 22-4-2016
 * Module: IJC-DU2 htab_clear.c
 * gcc version 4.9.2 (Debian 4.9.2-10)
 */
#include<stdio.h>
#include<stdlib.h>
#include"htable.h"

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
