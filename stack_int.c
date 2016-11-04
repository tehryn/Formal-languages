#inlclude "stack_int.h"


int stack_int_create ( struct t_stack_int ** stack, int n )		
{
	*stack = (struct t_stack_int*)malloc(sizeof(struct t_stack_int) + n*sizeof(int) );	// flexible array member

	if (stack == NULL)
		return -1;

	(*stack)->top = -1;
	(*stack)->size = n;

	return 0;
}


int stack_int_resize ( struct t_stack_int** stack, int new_size )
{
	struct t_stack_int * tmp = NULL;
	
	if (new_size <= (*stack)->size)
		return 1;

	tmp = (struct t_stack_int*)realloc(*stack, (sizeof(struct t_stack_int) + new_size*sizeof(int)) );

	if (tmp == NULL)
		return -1;
	else
		(*stack)->size = new_size;
	
	return 0;
}


void stack_int_destroy( struct t_stack_int * stack )
{
	free(stack);
	return;
}


int stack_int_push(struct t_stack_int* stack, int num, ...)
{

	va_list valist;
	va_start(valist, num);

	for (int i = 0; i < num; i++)
	{

		if (stack->top < (stack->size-1) )
		{
			stack->top += 1;
			stack->data[stack->top] = va_arg(valist, int);
		}
		else
			return 1;
	}

	va_end(valist);

	return 0;
}


int stack_int_pop(struct t_stack_int* stack, int* var)
{

	if (stack->top < 0)
		return 1;

	*var = stack->data[stack->top];
	stack->top -= 1;

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
		return 1;

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

