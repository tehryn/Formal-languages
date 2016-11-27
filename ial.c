#include "ial.h"
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

    string = (char *) mem_alloc(sizeof(char)*(n+1), L);
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

int stack_int_create ( struct t_stack_int * stack, int n )
{
	stack->data = (int *) malloc(n * sizeof(int));

	if (stack->data == NULL)
		return -1;

	stack->top = -1;
	stack->size = n;

	return 0;
}


static int stack_int_resize ( struct t_stack_int* stack, int new_size )
{
	int * tmp = NULL;

	if (new_size <= stack->size)
		return 1;

	tmp = (int *) realloc(stack->data, (new_size * sizeof(int) ));

	if (tmp == NULL)
		return -1;

	stack->data = tmp;
	stack->size = new_size;

	return 0;
}


void stack_int_destroy( struct t_stack_int * stack )
{
	free(stack->data);
	stack->data = NULL;
	return;
}


int stack_int_push(struct t_stack_int* stack, int num, ...)
{

	va_list valist;
	va_start(valist, num);

	for (int i = 0; i < num; i++)
	{
		if (stack->top >= (stack->size-1) )
		{
			if(stack_int_resize(stack, stack->size*2) < 0) return -1;
		}
		stack->top += 1;
		stack->data[stack->top] = va_arg(valist, int);
	}
	va_end(valist);


	return 0;
}


int stack_int_pop(struct t_stack_int* stack)
{

	if (stack->top < 0)
		return -1;

	stack->top -= 1;

	return 0;
}

int stack_int_top(struct t_stack_int* stack, int* var)
{

	if (stack->top < 0)
		return -1;

	*var = stack->data[stack->top];

	return 0;
}

// decrements stack pointer by parameter "n"
// if parameter "n" is -1, stack pointer is set to -1 (equivalent to initialized state)
// parameter "n" must be >= -1
int stack_int_clean(struct t_stack_int* stack, int n)
{
	if (n == -1)
	{
		stack->top = -1;
		return 0;
	}

	if (n < -1)
		return -1;

	stack->top = (stack->top-n < -1)? -1 : stack->top-n;
	return 0;
}


int stack_int_is_empty( struct t_stack_int * stack )
{
	return (stack->top == -1);
}


int stack_int_is_full( struct t_stack_int * stack )
{
	return (stack->top == (stack->size-1));
}

unsigned hash_function(const char *str, unsigned htab_size)
{
	unsigned int h=0;
	const unsigned char *p;
	for( p = (const unsigned char*) str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % htab_size;
}



htab_t * htab_init(unsigned size)
{
	htab_t * htab = (htab_t *) malloc(sizeof(htab_t));
	if (htab == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_init)");
		return NULL;
	}

	htab->htab_size = size;
	htab->hash_fun_ptr = &hash_function;
	htab->number_items = 0;
	htab->ptr = (htab_item**) malloc(size * sizeof(htab_item*));
	for (unsigned i = 0; i < size; i++)
		htab->ptr[i] = NULL;
	return htab;
}

htab_t * htab_init2(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size))
{
	htab_t * htab;
	if ((htab = (htab_t *) malloc(sizeof(htab_t) + size * sizeof(htab_item*))) == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_init2)");
		return NULL;
	}
	htab->htab_size = size;
	htab->hash_fun_ptr = hash_fun;
	htab->number_items = 0;
	htab->ptr = (htab_item**) malloc(size * sizeof(htab_item*));
	for (unsigned i = 0; i < size; i++)
		htab->ptr[i] = NULL;
	return htab;
}



void htab_clear_items(htab_t * T)
{
	htab_item * item; // item that we will free
	for (unsigned i = 0; i < T->htab_size; i++)
	{
		while (T->ptr[i] != NULL)
		{
			item = T->ptr[i];
			T->ptr[i] = T->ptr[i]->next_item; // store next item
			free(item->key);
			free(item->data);
			free(item);
			item = NULL;
		}
	}
}

void htab_free_all(htab_t * T)
{
	if (T != NULL)
	{
		htab_clear_items(T);
		free(T->ptr);
		free(T);
		T = NULL;
	}
}


htab_item * htab_find_item(htab_t * T, const char * key) // NULL if not there
{
	unsigned index = T->hash_fun_ptr(key, T->htab_size);
	htab_item * item = T->ptr[index];

	while (item != NULL)
	{
		if (!strcmp(item->key, key))
			return item; // nalezeno
		else
			item = item->next_item; // cyklit
	}
	return NULL; // nenalezeno
}

htab_item * malloc_item(const char * key)
{
	htab_item * item = (htab_item *) malloc(sizeof(htab_item));
	if (item == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_insert_item)");
		return NULL;
	}

	unsigned key_len = strlen(key) + 1;
	item->key = (char *) malloc(key_len);
	if (item->key == NULL)
	{
		fprintf(stderr, "Memory could not be allocated! (func. htab_insert_item)");
		free(item);
		return NULL;
	}

	strncpy(item->key, key, key_len);
	item->data_type = 0;
	item->func_or_var = 0;
	item->data = NULL;
	item->number_of_arguments = 0;
	item->initialized = 0;
	item->next_item = NULL;

	return item;
}

htab_item * htab_insert_item(htab_t * T, const char * key)
{
	unsigned index = T->hash_fun_ptr(key, T->htab_size);
	htab_item * item = T->ptr[index];

	if (item == NULL)
	{
		T->ptr[index] = malloc_item(key);
		return T->ptr[index]; // could be NULL
	}

	// item != NULL
	while (item->next_item != NULL)
		item = item->next_item;

	// jsme na poslednim prvku
	item->next_item = malloc_item(key);
	return item->next_item; // could be NULL
}
