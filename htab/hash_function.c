/*
 * Author: Jiri Matejka
 * Login: xmatej52
 * School: VUT FIT, Brno
 * Date: 22-4-2016
 * Module: IJC-DU2 hash_function.c
 * gcc version 4.9.2 (Debian 4.9.2-10)
 */
#include <stdio.h>

unsigned int hash_function(const char *str, unsigned htab_size) {
	unsigned int h=0;
	const unsigned char *p;
	for( p = (const unsigned char*) str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % htab_size;
}
