//  str_prcs.c
//  IFJ project
//  Last update: 28.10.2016
//  Author: Krystof Michal, FIT
//  Login: xmicha64
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
//#define TYPE_DOUBLE 1100
//#define TYPE_INT 1101
//#define TYPE_STRING 1102
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
            int input=0, output=0, octal;
//input a output sleduji retezec - input jej projede cely, jestlize narazi na specialni pripad, probehne prislusna akce, output dostane vysledek
//octal slouzi jako pomocna promena pri prevodu cisla v osmickove soustave z retezce do desitkove soutavy
            while (str[input] != '\0')
                {
                    if (str[input] == '\\')//vsechny escape sekvence zacinaji znakem '\'
                        {
                            input++; //zjisitme, zda a ktera escape sekvence nastala
                            switch(str[input])
                                {
                                    case '"':
                                        str[output++] = '\"';
                                        input++;
                                        break;
                                    case 'n':
                                        str[output++] = '\n';
                                        input++;
                                        break;
                                    case 't':
                                        str[output++] = '\t';
                                        input++;
                                        break;
                                    case '\\':
                                        str[output++] = '\\';
                                        input++;
                                        break;
                                    default:
                                        /*
                                        if (str[input]>='0'&&str[input]<='3'&&str[input+1]>='0'&&str[input+1]<='7'&&str[input+2]>='0'&&str[input+2]<='7')
                                            {
                                                octal=(str[input]-'0')*64+(str[input+1]-'0')*8+(str[input+2]-'0');
                                                */
                                        if (str[input]>='0' && str[input]<='3') //kontrola, zda je za znakem '\' trojmistne cislo v povolenem rozsahu provedena tak, aby nebyla prekrocena hranice stringu
                                            if (str[input+1]>='0' && str[input+1]<='7')
                                                if (str[input+2]>='0' && str[input+2]<='7')
                                                    octal = (str[input]-'0')*64 + (str[input+1]-'0')*8 + (str[input+2]-'0');
                                                else
                                                    break;
                                            else
                                                break;
                                        else
                                            break;
                                        if (octal == 0)
                                            {
                                                str[output++] = '\\';
                                                break;
                                            }
                                        else
                                            {
                                                str[output++] = octal;
                                                input += 3;
                                                break;
                                            }
                                }
                        }
                    str[output++] = str[input++];
                }
            return (void*)str;
        }
}
