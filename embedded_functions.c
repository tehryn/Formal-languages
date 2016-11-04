#include "embedded_functions.h"


char * readstring()
{
    char * tmp = NULL;
    char * string = (char *)malloc(sizeof(char));
    if (string==NULL)
    {
        fprintf(stderr, "ERROR: Memory could not be allocated! (func. readstring)\n");
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
            tmp = (char *)realloc(string, size);
            if (tmp==NULL)
            {
                fprintf(stderr, "ERROR: Memory could not be allocated!  (func. readstring)\n");
                return string;
            }
            else
                string = tmp;
        }

        c = getc(stdin);
    }

    string[size-1]='\0';
    return string;
}



int readint()
{
    char * string=NULL;
    long int n1=0;
    char * err=NULL;

    string = readstring();
    if (string == NULL)
	{
		fprintf(stderr, "ERROR: Couldn't read from STDIN. (func. readint)\n");
		return 0;
	}

    n1 = strtol(string, &err, 10);
    if (*err != 0)
	{
		fprintf(stderr, "ERROR: Invalid integer value, invalid symbol: \"%c\". (func. readint)\n", *err);
        free(string);
		return 0;
    }

	if (n1 > (long int)(INT_MAX))
    {
        fprintf(stderr, "ERROR: number is greater than INT-MAX.\n");
        free(string);
		return 0;
    }
	else if (n1 < (long int)(INT_MIN))
    {
        fprintf(stderr, "ERROR: number is lesser than INT-MIN.\n");
        free(string);
		return 0;
    }

    free(string);
	return (int) n1;
}



double readdouble()
{
    char * string=NULL;
    double n1=0;
    char * err=NULL;

    string = readstring();
	if (string == NULL)
	{
		fprintf(stderr, "ERROR: Couldn't read from STDIN. (func. readdouble)\n");
		return 0.0;
	}

    n1 = strtod(string, &err);
    if (*err != 0)
    {
        fprintf(stderr, "ERROR: invalid double value, invalid symbol: \"%c\". (func. readdouble)\n", *err);
        free(string);
		return 0.0;
    }

    free(string);
	return n1;
}



int length(char * string)
{
    int len=0;
    for (len = 0; ; len++)
    {
        if (len==INT_MAX)
        {
            fprintf(stderr, "WARNING: Maximum integer value reached! (func. length)\n");
            break;
        }
        if (string[len]=='\0')
            break;
    }
    return len;
}



char * substring(char * s, int i, int n)
{
    char * string=NULL;

    string = malloc(sizeof(char)*(n+1));
    if (string==NULL)
    {
        fprintf(stderr, "ERROR: Memory could not be allocated! (func. substring)\n");
        return NULL;
    }

    string[n]='\0';
    int j = 0;
    for ( j=0; j<n; j++)
        string[j]=s[j+i];

    string[j+1]='\0';
    return string;
}



char * shellsort(char * str)
{
    int num = strlen(str);
    char * arr=NULL;
    arr=(char *)malloc( sizeof(char)*(num+1) );
    if (arr==NULL)
    {
        fprintf(stderr, "ERROR: not enough memory!\n");
        return NULL;
    }

    for (int zzz=0; zzz<num; zzz++)
        arr[zzz]=str[zzz];

    int i, j, k, tmp;
    for (i = num / 2; i > 0; i = i / 2)
    {
        for (j = i; j < num; j++)
        {
            for(k = j - i; k >= 0; k = k - i)
            {
                if (arr[k+i] >= arr[k])
                    break;
                else
                {
                    tmp = arr[k];

                    arr[k] = arr[k+i];

                    arr[k+i] = tmp;
                }
            }
        }
    }




/*
    int h, i, j, t;
    for (h = num; h /= 2;)
	{
        for (i = h; i < num; i++)
		{
            t = arr[i];
            for (j = i; j >= h && t < arr[j - h]; j -= h)
			{
                arr[j] = arr[j - h];
            }
            arr[j] = t;
        }
    }
*/

	arr[num]='\0';
    return arr;
}
