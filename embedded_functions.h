#ifndef embedded_functions_h_INCLUDED
#define embedded_functions_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "garbage_collector.h"

char * readString();
int readInt();
double readDouble();
void print(char * string);

#endif 	// embedded_functions_h_INCLUDED
