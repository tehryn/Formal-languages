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
    if (id==TYPE_INT)
        {
            int *number = malloc(sizeof(int));
            if (!number) return NULL;
            if (str[0]=='0')
                *number = strtol(str,&eptr,8);
            else
                *number = strtol(str,&eptr,10);
            return (void*)number;
        }
    else if (id==TYPE_DOUBLE)
        {
            double *number = malloc(sizeof(double));
            if (!number) return NULL;
            *number = strtod(str,&eptr);
            return (void*)number;
        }
    else
        {
            int i=0, o=0, k;
            while (str[i]!='\0')
                {
                    if (str[i]=='\\')
                        {
                            i++;
                            switch(str[i])
                                {
                                    case '"':
                                        str[o++]='\"';
                                        i++;
                                        break;
                                    case 'n':
                                        str[o++]='\n';
                                        i++;
                                        break;
                                    case 't':
                                        str[o++]='\t';
                                        i++;
                                        break;
                                    case '\\':
                                        str[o++]='\\';
                                        i++;
                                        break;
                                    default:
                                        if (str[i]>='0'&&str[i]<='3'&&str[i+1]>='0'&&str[i+1]<='7'&&str[i+2]>='0'&&str[i+2]<='7')
                                            {
                                                k=(str[i]-'0')*64+(str[i+1]-'0')*8+(str[i+2]-'0');
                                                if (k==0)
                                                    {
                                                        str[o++]='\\';
                                                        break;
                                                    }
                                                else
                                                    {
                                                        str[o++]=k;
                                                        i+=3;
                                                        break;
                                                    }
                                            }
                                }
                        }
                    str[o++]=str[i++];
                }
            return(void*)str;
        }
}
