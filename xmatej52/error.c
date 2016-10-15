//  error.c
//  Autor: Miroslava Misova, FIT 

#include <stdio.h>
#include <stdlib.h>
#include "error.h"


extern unsigned LINE_NUM;

void error_msg(int err_number, char type_of_fault)
{
    FILE *stream = stderr;
    fprintf(stream, "ERROR: line %d: ", LINE_NUM);

    switch(err_number)
    {
        case ERR_LEXIKALNI_ANALYZA:
            fprintf(stream, "Problem with lexical analysis.\n");
            break;
        case ERR_SYNTAKTICKA_ANALYZA:
            fprintf(stream, "Problem with syntactic analysis.\n");
            break;
        case ERR_SEM_NDEF_REDEF:
            fprintf(stream, "Problem with semantics. Object is not defined or cannot be redefined.\n");
            break;
        case ERR_SEM_KOMPATIBILITA:
            fprintf(stream, "Problem with semantics. It could be type compatibility, arithmetic, string, relational expression or bad count or type of arguments.\n");
            break;
        case ERR_SEM_OSTATNI:
            fprintf(stream, "Problem with semantics.\n");
            break;
        case ERR_VSTUP_CISLO:
            fprintf(stream, "Problem with import of number.\n");
            break;
        case ERR_NEINIC_PROM:
            fprintf(stream, "Problem with uninitialized variable.\n");
            break;
        case ERR_DELENI_NULOU:
            fprintf(stream, "Problem with division by zero.\n");
            break;
        case ERR_OSTATNI:
            fprintf(stream, "Problem with runtime error.\n");       // behova chyba - nelze prelozit lepe?
            break;
        case ERR_INTERPRET:
        default:
            fprintf(stream, "Problem with intern fault.\n");
            break;
    }
    if (type_of_fault == 'f')
        exit(err_number);
}
