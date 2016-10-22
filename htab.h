// Miroslava Misova
// zatim nepropojeno s htab.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- STRUCTURES ---
typedef struct htab_item {
	char* key; // string ID
	void* data;
	unsigned scope; // block_id, if (scope == -1) not allocated
	struct htab_item* next_item; // next variable
} htab_item;
// item->data = setData(cislo, int); ...

typedef struct htab_t {

	/* pointer to hash function, &hash_function by default */
	unsigned (*hash_fun_ptr) (const char *str, unsigned htab_size);
	unsigned htab_size; // number of lines
	unsigned n; // real number of items 
	htab_item * ptr[]; // lines
} htab_t;



// --- Inicialization of hash table --- TODO
htab_t_int *htab_init(unsigned size, unsigned (*hash_fun)(const char * str, unsigned htab_size));

void htab_clear_items(htab_t * T);
void htab_free_all(htab_t * T);

htab_item* htab_find_add_item(htab_t * T, const char * key); // if not found - make a place for it

int htab_set_item(htab_item * I, void* data, unsigned scope);

/*
void htab_foreach(htab_t *t, void (*func)(char*, unsigned));
*/