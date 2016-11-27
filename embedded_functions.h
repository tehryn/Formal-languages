#ifndef embedded_functions_h_INCLUDED
#define embedded_functions_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "garbage_collector.h"

char * readString(mem_list_t *L);
int readInt( mem_list_t *L );
double readDouble(mem_list_t *L);
int length(char * string);
void print(char * string);

#endif 	// embedded_functions_h_INCLUDED
