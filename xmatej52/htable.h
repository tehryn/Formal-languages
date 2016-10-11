/*
 * Author: Jiri Matejka
 * Login: xmatej52
 * School: VUT FIT, Brno
 * Date: 22-4-2016
 * Module: IJC-DU2 htable.h
 * gcc version 4.9.2 (Debian 4.9.2-10)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * List of items with data
 */
typedef struct htab_listitem {
	char  *key; // string ID
	unsigned data; // number of matches of key
	struct htab_listitem *next; // next item
} htab_listitem;

/*
  +--------------+
  | htab_size    | // array size
  +--------------+
  | hash_fun_ptr | // pointer to hash function, &hash_function by default
  +--------------+
  | n            | // real number of items
  +--------------+
  +---+
  |ptr|-->[key,data,next]-->[key,data,next]-->[key,data,next]--|
  +---+
  |ptr|-->[key,data,next]-->[key,data,next]--|
  +---+
  |ptr|--|
  +---+
 */
typedef struct htab_t {
	unsigned htab_size; // array size
	/* pointer to hash function, &hash_function by default */
	unsigned (*hash_fun_ptr) (const char *str, unsigned htab_size);
	unsigned n; // real number of items
	htab_listitem *ptr[];
} htab_t;

unsigned int hash_function(const char *str, unsigned htab_size);

/*
 * Inicialization of hash table
 */
htab_t *htab_init(unsigned size);
htab_t *htab_init2(unsigned size, unsigned (*hash_fun)(const char *str, unsigned htab_size));

/*
 * Erase all data in Hash table
 */
void htab_clear(htab_t *t);

/*
 * Destroy Hash table
 */
void htab_free(htab_t *t);

/*
 * Try to find key in hash table, add new item if failed
 */
struct htab_listitem *htab_lookup_add(htab_t *t, const char *key);

/*
 * Call function pointed by func for every item form hash table t.
 */
void htab_foreach(htab_t *t, void (*func)(char*, unsigned));

/*
 * Load word form file into array s. Load max-1 characters.
 */
int get_word(char *s, int max, FILE *f);
