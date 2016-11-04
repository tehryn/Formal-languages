#ifndef embedded_functions_h_INCLUDED
#define embedded_functions_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char * readstring(mem_list_t *L);
int readint( mem_list_t *L );
double readdouble(mem_list_t *L);
int length(char * string);
char * substring(char * s, int i, int n, mem_list_t *L);
char * shellsort(char * str, mem_list_t *L);

#endif 	// embedded_functions_h_INCLUDED
