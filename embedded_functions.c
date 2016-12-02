#include "embedded_functions.h"
#include "garbage_collector.h"
#include "error.h"

char * readString()
{
    char * tmp = NULL;
    char * string = (char *)mem_alloc(sizeof(char));
    if (string==NULL)
    {
        error_msg(99, "ERROR: Memory could not be allocated. (func. readstring)\n");	// code 99 -> internal error
        return NULL;
    }

    int size = 1;
    int index = 0;
    int c = getc(stdin);

    while (c != EOF && c != '\n')
    {
        string[index]=c;
        index += 1;

        if (index == size)
	{
            size *= 2;
            tmp = (char *)mem_realloc(string, size);
            if (tmp==NULL)
            {
                //free(string);
		        error_msg(99, "ERROR: Memory could not be allocated. (func. readstring)\n");	// code 99 -> internal error
		        return NULL;
            }
            else
                string = tmp;
        }

        c = getc(stdin);
    }

    string[size-1]='\0';
    return string;
}



int readInt()
{
    char * string=NULL;
    long int n1=0;
    char * err=NULL;

    string = readString();
    if (string == NULL)
	{
		//free(string);
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readint)\n");	// code 7 -> read number error
		return 0;
	}

    n1 = strtol(string, &err, 10);
    if (*err != 0)
	{
		//free(string);
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readint)\n");	// code 7 -> read number error
		return 0;
    	}

/*
	if (n1 > (long int)(INT_MAX))
	{
		//free(string);
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readint)\n");	// code 7 -> read number error
		return 0;
	}
	else if (n1 < (long int)(INT_MIN))
	{
		//free(string);
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readint)\n");	// code 7 -> read number error
		return 0;
	}
*/

    	//free(string);
	return (int) n1;
}



double readDouble()
{
    char * string=NULL;
    double n1=0;
    char * err=NULL;

    string = readString();
	if (string == NULL)
	{
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readdouble)\n");
		return 0.0;
	}

    n1 = strtod(string, &err);
    if (*err != 0)
    {
	free(string);
    error_msg(7, "ERROR: Number value could not be read from stdin, invalid symbol: \"%c\". (func. readdouble)\n", *err);
	return 0.0;
    }

    //free(string);
	return n1;
}

void print(char * string)
{
	printf("%s", string);
}
