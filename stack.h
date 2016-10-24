/**

*/


#ifndef STACK
#define STACK
#include <stdio.h>
#include <stdlib.h>
#include "htable.h"
#define STACK_INIT_SIZE 16

/**

*/
typedef struct stack_t {
	int top;
	size_t size;
	htab_t *data;
} stack_t;

/**

*/
stack_t *stack_t_init(stack_t *stack; size_t size);

/**

*/

int stack_t_push(stack_t *stack, htab_t table);

/**

*/

int stack_t_pop(stack_t *stack);

/**

*/

htab_t *stack_t_get_item(stack_t *stack, unsigned bactrack);


#endif
