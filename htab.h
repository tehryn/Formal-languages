// Miroslava Misova
// zatim nepropojeno s htab.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
#define TYPE_DOUBLE 1100
#define TYPE_INT 1101
#define TYPE_STRING 1102
#define TYPE_BOOLEAN 1103
#define TYPE_STATIC_DOUBLE 1104
#define TYPE_STATIC_INT 1105
#define TYPE_STATIC_STRING 1106
#define TYPE_STATIC_BOOLEAN 1107
*/

// --- STRUCTURES ---
typedef struct htab_item {
	char* key; // string ID
	unsigned data_type;
	void* data; // pointer to the place with data
	unsigned scope; // block_id, if (scope == -1) not allocated ???
	struct htab_item* next_item; // next variable
} htab_item;

typedef struct htab_t {
	unsigned (*hash_fun_ptr) (const char *str, unsigned htab_size); // pointer to hash function, &hash_function by default
	unsigned htab_size; // number of lines
	unsigned number_items; // real number of items 
	htab_item * ptr[]; // lines
} htab_t;


htab_t * htab_init(unsigned size);
htab_t * htab_init2(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size));

void htab_clear_items(htab_t * T);
void htab_free_all(htab_t * T);

htab_item* htab_find_add_item(htab_t * T, const char * key, unsigned scope, unsigned data_type); // if not found - make a place for it
htab_item* add_item(const char * key, unsigned scope, unsigned data_type)
// int htab_set_item(htab_item * I, void* data, unsigned scope);
