//  error.h
//  Autor: Miroslava Misova, FIT

#include <stdio.h>
#include <stdlib.h>

#ifndef ERROR
#define ERROR



// --------MAKRA--------------

#define ERR_LEXIKALNI_ANALYZA 1
#define ERR_SYNTAKTICKA_ANALYZA 2
#define ERR_SEM_NDEF_REDEF 3
#define ERR_SEM_KOMPATIBILITA 4
#define ERR_SEM_OSTATNI 6
#define ERR_VSTUP_CISLO 7
#define ERR_NEINIC_PROM 8
#define ERR_DELENI_NULOU 9
#define ERR_OSTATNI 10
#define ERR_INTERPRET 99

void error_msg(int err_number, char type_of_fault);  
//  type_of_fault: 
//      'w' = warning
//      'f' = fatal (with exit) */

#endif