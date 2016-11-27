#ifndef embedded_functions_h_INCLUDED
#define embedded_functions_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "garbage_collector.h"
#define max(a,b) ((a)   > (b) ? a : b)
#define NO_OF_CHARS 256


char * readstring(mem_list_t *L);
int readint( mem_list_t *L );
double readdouble(mem_list_t *L);
int length(char * string);
int find( char *s,  char *search);
void computeMatchJumps(char *string, unsigned int str_size, int match_jump[]);
void computeJumps( char *string, unsigned int str_size, int charjump[]);
void print(char * string);

#endif 	// embedded_functions_h_INCLUDED
