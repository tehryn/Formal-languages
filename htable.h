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
typedef struct htab_int {
	char  *key; // string ID
	unsigned allocaded;
	int data; // number of matches of key
	struct htab_int *next; // next variable
} htab_int;

typedef struct htab_double {
	char  *key; // string ID
	unsigned allocaded;
	double data; // number of matches of key
	struct htab_int *next; // next variable
} htab_double;

/*
  +--------------+
  | htab_size    | // array size
  +--------------+
  | hash_fun_ptr | // pointer to hash function, &hash_function by default
  +--------------+
  | n            | // real number of items
  +--------------+
  +---+
  |ptr|-->[word,meaning,value,position,next]-->[key,data,next]-->[key,data,next]--|
  +---+
  |ptr|-->[key,data,next]-->[key,data,next]--|
  +---+
  |ptr|--|
  +---+
 */
typedef struct htab_t_int {
	unsigned htab_size; // array size
	/* pointer to hash function, &hash_function by default */
	unsigned (*hash_fun_ptr) (const char *str, unsigned htab_size);
	unsigned n; // real number of items
	htab_int *ptr[];
} htab_t_int;

typedef struct htab_t_double {
	unsigned htab_size; // array size
	/* pointer to hash function, &hash_function by default */
	unsigned (*hash_fun_ptr) (const char *str, unsigned htab_size);
	unsigned n; // real number of items
	htab_double *ptr[];
} htab_t_double;

unsigned int hash_function(const char *str, unsigned htab_size);

/*
 * Inicialization of hash table
 */
htab_t_double *htab_init(unsigned size, unsigned (*hash_fun)(const char *str, unsigned htab_size));
htab_t_int *htab_init(unsigned size, unsigned (*hash_fun)(const char *str, unsigned htab_size));
/*
 * Erase all data in Hash table
 */
void htab_clear(htab_t_int *t);
void htab_clear(htab_t_double *t);

/*
 * Destroy Hash table
 */
void htab_free(htab_t_int *t);
void htab_free(htab_t_double *t);

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
