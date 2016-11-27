#include "embedded_functions.h"
#include "garbage_collector.h"
#include "error.h"

char * readstring(mem_list_t *L)
{
    char * tmp = NULL;
    char * string = (char *)mem_alloc(sizeof(char), L);
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
            tmp = (char *)mem_realloc(string, size, L);
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



int readint(mem_list_t *L)
{
    char * string=NULL;
    long int n1=0;
    char * err=NULL;

    string = readstring(L);
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



double readdouble(mem_list_t *L)
{
    char * string=NULL;
    double n1=0;
    char * err=NULL;

    string = readstring(L);
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



int length(char * string)
{
    int len=0;
/*
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
*/
	while (string[len]!='\0')
		len++;
    return len;
}



char * substring(char * s, int i, int n, mem_list_t *L)
{
    char * string=NULL;

    string = mem_alloc(sizeof(char)*(n+1), L);
    if (string==NULL)
    {
        error_msg(99, "ERROR: Memory could not be allocated. (func. substring)\n");	// code 99 -> internal error
        return NULL;
    }

	// strlen ???
	if ( (i+n) > length(s) )
	{
		error_msg(10, "ERROR: Substring exceeds input string. (func. substring)\n");	// code 10 -> substring error
		return NULL;
	}
	
    string[n]='\0';
    int j = 0;
    for ( j=0; j<n; j++)
        string[j]=s[j+i];

    string[j+1]='\0';
    return string;
}



char * shellsort(char * str, mem_list_t *L)
{
    int num = strlen(str);
    char * arr=NULL;
    arr=(char *)mem_alloc( sizeof(char)*(num+1), L);
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



// The preprocessing function for Boyer Moore's bad character heuristic
void computeJumps( char *string, unsigned int str_size, int charjump[])
{
    unsigned int i;

    // Initialize all occurrences as lenght of string
    for (i = 0; i < NO_OF_CHARS; i++)
         charjump[i] = str_size;

    // Fill the actual value of last occurrence of a character
    for (i = 0; i < str_size; i++)
         charjump[(int) string[i]] = str_size-i;
}

void computeMatchJumps(char *string, unsigned int str_size, int match_jump[])
{
    int pole[str_size+1];
    for (int i=0; i<=str_size; i++)
    {
        pole[i]=0;
        match_jump[i]=0;
    }

    int k=str_size+1;

    for (int i=str_size-1; i<=0; i--)
    {
        pole[i]=k;
        while ((k <= str_size) && (string[i-1] != string[k-1]))
        {
            if (match_jump[k] == 0)
                match_jump[k] = k - i;
            k = pole[k];
        }
        k--;
    }

    k=pole[0];
    for (int i=0; i <= str_size; i++)
    {
        if (match_jump[i] == 0)
            match_jump[i]==k;
        if (i==k)
            k=pole[k];
    }
}


/* A pattern searching function that uses Bad Character Heuristic of
   Boyer Moore Algorithm */
int find( char *s,  char *search)
{
    unsigned int search_len = strlen(search);
    unsigned int s_len = strlen(s);

    int charjump[NO_OF_CHARS];
    int matchjump[s_len-1];


    computeJumps(search, search_len, charjump);
    computeMatchJumps(search, search_len, matchjump);

    int unsigned shift = 0;  // s is shift of the pattern with respect to text
    while(shift <= (s_len - search_len))
    {
        int j = search_len-1;

        /* Keep reducing index j of pattern while characters of
           pattern and text are matching at this shift s */
        while(j >= 0 && search[j] == s[shift+j])
            j--;

        /* If the pattern is present at current shift, then index j
           will become -1 after the above loop */
        if (j < 0)
            return shift+1;

        else
            shift += max(charjump[(int)search[shift+s_len-1]], matchjump[j+1]);
    }
    return -1;
}


char * print(char * string)
{
	printf("%s", string);
}




