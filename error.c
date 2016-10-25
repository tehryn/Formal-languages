//  error.c
//  IFJ project
//  Last update: 2$.10.2016
//  Author: Miroslava Misova, FIT 

#include <stdio.h>
#include <stdlib.h>
#include "error.h"


extern unsigned LINE_NUM;

void error_msg(int err_number, const char *fmt, ...)
{
    FILE *stream = stderr;
    if (err_number != 0)
    	fprintf(stream, "ERROR: line %d: ", LINE_NUM);
    else
    	fprintf(stream, "WARNING: line %d: ", LINE_NUM);

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
    va_end(args);

    if (err_number != 0)
        exit(err_number);
}


 /*   switch(err_number)
    {
    	case ERR_WARNING: break;
        case ERR_LEXICAL_ANALYSIS:
            fprintf(stream, "Problem with lexical analysis.\n");
            break;
        case ERR_SYNTACTIC_ANALYSIS:
            fprintf(stream, "Problem with syntactic analysis.\n");
            break;
        case ERR_SEM_NDEF_REDEF:
            fprintf(stream, "Problem with semantics. Object is not defined or cannot be redefined.\n");
            break;
        case ERR_SEM_COMPATIBILITY:
            fprintf(stream, "Problem with semantics. It could be type compatibility, arithmetic, string, relational expression or bad count or type of arguments.\n");
            break;
        case ERR_SEM_OTHERS:
            fprintf(stream, "Problem with semantics.\n");
            break;
        case ERR_INPUT_NUMBER:
            fprintf(stream, "Problem with import of number.\n");
            break;
        case ERR_UNINICIALIZED_VAR:
            fprintf(stream, "Problem with uninitialized variable.\n");
            break;
        case ERR_DIVISION_ZERO:
            fprintf(stream, "Problem with division by zero.\n");
            break;
        case ERR_OTHERS:
            fprintf(stream, "Problem with runtime error.\n");       // behova chyba - nelze prelozit lepe ???
            break;
        case ERR_INTERN_FAULT:
        default:
            fprintf(stream, "Problem with intern fault.\n");
            break;
    }
*/