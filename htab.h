// Miroslava Misova
// zatim nepropojeno s htab.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    num_int = 1, num_double, string, boolean, static_int, static_double, static_string
} data_type_t;

// --- STRUCTURES ---
typedef struct htab_item {
	char* key; // string ID
	data_type_t data_type;
	void* data; // pointer to the place with data
	unsigned scope; // block_id, if (scope == -1) not allocated
	struct htab_item* next_item; // next variable
} htab_item;

typedef struct htab_t {
	unsigned (*hash_fun_ptr) (const char *str, unsigned htab_size); // pointer to hash function, &hash_function by default
	unsigned htab_size; // number of lines
	unsigned n; // real number of items 
	htab_item * ptr[]; // lines
} htab_t;


htab_t * htab_init(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size));

void htab_clear_items(htab_t * T);
void htab_free_all(htab_t * T);

htab_item* htab_find_add_item(htab_t * T, const char * key); // if not found - make a place for it

// int htab_set_item(htab_item * I, void* data, unsigned scope);