//  error.h
//  IFJ project
//  Last update: 2$.10.2016
//  Author: Miroslava Misova, FIT 

#ifndef ERROR
#define ERROR
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// --------MAKRA--------------

#define ERR_WARNING 0
#define ERR_LEXICAL_ANALYSIS 1
#define ERR_SYNTACTIC_ANALYSIS 2
#define ERR_SEM_NDEF_REDEF 3
#define ERR_SEM_COMPATIBILITY 4
#define ERR_SEM_OTHERS 6
#define ERR_INPUT_NUMBER 7
#define ERR_UNINICIALIZED_VAR 8
#define ERR_DIVISION_ZERO 9
#define ERR_OTHERS 10
#define ERR_INTERN_FAULT 99

void error_msg(int err_number, const char *fmt, ...);  

#endif
