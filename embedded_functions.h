#ifndef embedded_functions_h_INCLUDED
#define embedded_functions_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int readint();
double readdouble();
char * readstring();
int length(char * string);
char * substring();
char * shellsort(char * str);

#endif 	// embedded_functions_h_INCLUDED
