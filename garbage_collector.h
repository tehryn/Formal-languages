/**
* Author: Matejka Jiri
* Login: xmatej52
* School: VUT FIT, BRNO
* Project: Interpret for IFJ16
* gcc version: 5.4.0 (ubuntu 16.04.2)
* Date: 2016-12-03
**/
#ifndef MEMORY
#define MEMORY
#include <stdio.h>
#include <stdlib.h>
/** @defgroup garbage_collector Garbage collector */
/** @addtogroup garbage_collector
  * @brief Garbage collector is group of functions that allocate memory and
  * store pointers into list to prevent memory leaks
  * @{
  */

/**
 * List of items, that holds allocated memory
 */
typedef struct mem_list_t {
	/** Pointer to first item */
	struct mem_item_t *first;
	/** Pointer to last item */
	struct mem_item_t *last;
} mem_list_t;

/**
 * Item that holds one pointer to allocated memory
 */
typedef struct mem_item_t {
	/** Pointer to allocated memory */
	void *ptr;
	/** Pointer to nex item */
	struct mem_item_t *next;
} mem_item_t;

/*! Global list of items that holds allocated memory */
extern mem_list_t GARBAGE_COLLECTOR;

/**
 * Initialize @var GARBAGE_COLLECTOR
 */
void mem_list_t_init();

/**
 * Allocate memory
 * @param  size Number of memory that will be allocated in bytes
 * @return      Pointer to allocated memory, NULL when allocation fails
 * @pre         Function mem_list_t_init was called before
 */
void * mem_alloc(size_t size);

/**
 * Free all memory allocated with this module
 * @pre Function mem_list_t_init was called before
 */
void free_memory();

/**
 * Reallocate memory for new size
 * @param  ptr  Pointer to memory that will be reallocated
 * @param  size New size of memory for allocation
 * @return      Pointer to allocated memory, NULL when allocation fails
 * @pre         Function mem_list_t_init was called before
 */
void * mem_realloc(void *ptr, size_t size);

/** @} */
#endif
