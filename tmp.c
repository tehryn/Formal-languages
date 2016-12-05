/**
 * Search for item of local or global hash table in the stack
 * @param  stack	stack where item will be searched
 * @param  key		name of variable or function which will be searched
 * @return			pointer to item of hash. table where searched thing is, or NULL if the search was not successful
 * @pre				stack has been inicializated
 */
htab_item * stack_htab_find_htab_item(stack_htab * stack, char * key)
{
	htab_t * Table = NULL;
	htab_item * TableItem = NULL;
	if (is_full_ident(key, strlen(key)))
	{
		Table = stack_htab_get_first(stack)
		if (Table == NULL)
			return NULL;
		TableItem = htab_find_item(Table, key);
		return TableItem; // could be NULL
	}
	else if (is_simple_ident(key, strlen(key)))
	{
		Table = stack_htab_get_item(stack, 0);
		if (Table == NULL)
			return NULL;
		TableItem = htab_find_item(Table, key);
		return TableItem; // could be NULL
	}
	return NULL;
}
