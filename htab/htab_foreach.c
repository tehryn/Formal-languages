/*
 * Author: Jiri Matejka
 * Login: xmatej52
 * School: VUT FIT, Brno
 * Date: 22-4-2016
 * Module: IJC-DU2 htab_foreach.c
 * gcc version 4.9.2 (Debian 4.9.2-10)
 */
#include<stdio.h>
#include<stdlib.h>
#include"htable.h"

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
