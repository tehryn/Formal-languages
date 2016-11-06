#include "stack_int.h"


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
