//  str_prcs.c
//  IFJ project
//  Last update: 28.10.2016
//  Author: Krystof Michal, FIT
//  Login: xmicha64
#include "str_prcs.h"
void* string_process(int id, char* str)
{
    char *eptr;
    if (id == TYPE_INT)
        {
            int *number = (int *) malloc(sizeof(int));
            if (!number) return NULL;
            if (str[0]=='0') //jestlize cislo zacina nulou, jedna se o cislo v osmickove soustave, v opacnem pripade v desitkove
                *number = strtol(str,&eptr,8);
            else
                *number = strtol(str,&eptr,10);
            return (void*)number;
        }
    else if (id == TYPE_DOUBLE)
        {
            double *number = (double *) malloc(sizeof(double));
            if (!number) return NULL;
            *number = strtod(str,&eptr);
            return (void*)number;
        }
    else
        {
            int octal = 0;
            size_t len = strlen(str);
            unsigned j = 0;
//input a output sleduji retezec - input jej projede cely, jestlize narazi na specialni pripad, probehne prislusna akce, output dostane vysledek
//octal slouzi jako pomocna promena pri prevodu cisla v osmickove soustave z retezce do desitkove soutavy
            for (unsigned i = 0; i < len; i++)
                {
                    if (str[i] == '\\') {
                        if ((i+1) < len) {
                            if (str[i+1] == 'n') {
                                str[j] = '\n';
                                j++;
                                i++;
                                continue;
                            }
                            else if (str[i+1] == '"') {
                                str[j] = '"';
                                j++;
                                i++;
                                continue;
                            }
                            else if (str[i+1] == '\'') {
                                str[j] = '\'';
                                j++;
                                i++;
                                continue;
                            }
                            else if (str[i+1] == '\\') {
                                str[j] = '\\';
                                j++;
                                i++;
                                continue;
                            }
                            else if (str[i+1] == 't') {
                                str[j] = '\t';
                                j++;
                                i++;
                                continue;
                            }
                            else if ((i+3) < len) {
                                if (isdigit(str[i+1]) && isdigit(str[i+2]) && isdigit(str[i+3]) && str[i+1]<='3' && str[i+2]<='7' && str[i+3]<='7') {
                                    octal = (str[i+1] - '0')*64 + (str[i+2] - '0')*8 + (str[i+3] - '0');
                                    str[j] = octal;
                                    j++;
                                    i+=3;
                                    continue;
                                }
                            }
                        }
                    }
                    str[j] = str[i];
                    j++;
                }
            str[j] = '\0';
            return (void*)str;
        }
}
