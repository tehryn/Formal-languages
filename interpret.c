#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include "embedded_functions.h"
#define I_STACKSIZE 20

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
		Table = stack_htab_get_first(stack);
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

int inter(Instr_List *L, stack_htab *I_Htable)
{
	struct stack_expresion *S=malloc(sizeof(struct stack_expresion));
	if (S==NULL)
		return ERR_INTERN_FAULT;

	stack_expression_init(S,I_STACKSIZE);
	htab_item * item_tmp1;
	htab_item * return_hitem;
	htab_t *loc_table;
	token *return_token;
	int k=0;
	token *postfix_array;
	token ptr, tmp1, tmp2, *new;
	while (L->Active!=NULL)
    {
		printf("type: %d\n",L->Active->type_instr);
		switch (L->Active->type_instr)
		{

			case I_ASSIGMENT:
				return_token=(token *)L->Active->adr1;
				if (return_token->id!=TYPE_STRING)
				{
					return_hitem=stack_htab_find_htab_item(I_Htable, (char *)return_token->ptr);
				}
				k=0;
				postfix_array=(token *)L->Active->adr2;
				ptr=postfix_array[k];
				
				while(postfix_array[k].id!=END_EXPR)
				{	
					ptr=postfix_array[k++];
					switch (ptr.id)
					{
						case S_SIMPLE_IDENT:
						case S_FULL_IDENT:
							
							item_tmp1=stack_htab_find_htab_item(I_Htable, ptr.ptr);
							if (item_tmp1==NULL)
							{
								stack_expression_destroy(S);
								free(S);
								fprintf(stderr,"Interpret: Item wasn't found.\n");
								return ERR_OTHERS;
							}
							if (item_tmp1->func_or_var==2)
							{
								if(is_emb_fce(item_tmp1,postfix_array,return_token,I_Htable))
								{
									stack_expression_destroy(S);
									free(S);
									return 0;
								}
								else 				// TODO
									return -1;
							
							}
							if (item_tmp1->initialized==0)
							{
								fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
								//freeALL();
								return ERR_UNINICIALIZED_VAR ;
							}
							if (item_tmp1->func_or_var==1)
							{
								new=malloc(sizeof(token));
								if (new==NULL)
									return ERR_INTERN_FAULT;

								if (item_tmp1->data_type==S_STRING)
									new->id=TYPE_STRING;
								else if (item_tmp1->data_type==S_DOUBLE)
									new->id=TYPE_DOUBLE;
								else
									new->id=TYPE_INT;							
								
								
								
								new->ptr=item_tmp1->data;
								stack_expression_push(S,*new);
							}
							//free(new);
							break;

						case TYPE_DOUBLE:
						case TYPE_INT:
						case TYPE_STRING:
							stack_expression_push(S,ptr);
							break;

						case S_PLUS:					// ----------- PLUS
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_plus(tmp1,tmp2);
							stack_expression_push(S,*new);
							break;

						case S_MINUS:					//     ----------------------  MINUS

							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_arm_op(tmp1,tmp2,1);
							stack_expression_push(S,*new);
							break;


						case S_MUL:					//     ----------------------  Multiplication
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_arm_op(tmp1,tmp2,2);
							stack_expression_push(S,*new);
							break;


						case S_DIV:					//     ----------------------  Division
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_arm_op(tmp1,tmp2,2);
							if (new->id==-8)
							{
								fprintf(stderr, "Divison by zero!.\n");
								//freeALL();
								return ERR_DIVISION_ZERO ;
							}

							stack_expression_push(S,*new);
							break;


					}

				}


				stack_expression_pop(S,&tmp1);
				
				if(L->Active->adr1==NULL) // ifj16.print
				{
					if (tmp1.id==TYPE_INT)
					{
						char *str_data=IntToString((*((int *)tmp1.ptr)));
						//(tmp1.ptr);
						print(str_data);
						free(str_data);
					}
					else if (tmp1.id==TYPE_DOUBLE)
					{
						char *str_data=DoubleToString((*((int *)tmp1.ptr)));
						//free(tmp1.ptr);
						print(str_data);
						free(str_data);
					}
					else
					{
						//free(return_hitem->data);
						char *str_data=(char *)tmp1.ptr;
						print(str_data);
						free(str_data);
					}
					I_Instr *tmp_instr=L->Active->next_instr;
					I_Instr *prev_instr=L->Active->adr3;
					prev_instr->next_instr=tmp_instr;
					free(L->Active);
					L->Active=tmp_instr;
					break;
				}
				
				
				if (return_hitem->data_type==S_INT && tmp1.id!=TYPE_INT)
				{
					return ERR_SEM_COMPATIBILITY;
				}
				if(return_hitem->data_type==S_DOUBLE && tmp1.id==TYPE_STRING)
				{
					return ERR_SEM_COMPATIBILITY;
				}
				

				if(return_hitem->data_type==S_DOUBLE)
				{
					return_hitem->data=(double *)tmp1.ptr;
				}
				else if(return_hitem->data_type==S_INT)
				{
					//free(return_hitem->data);
					return_hitem->data=(int *)tmp1.ptr;

				}
				else
				{
					if (tmp1.id==TYPE_INT)
					{
						char *str_data=IntToString((*((int *)tmp1.ptr)));
						//(tmp1.ptr);
						return_hitem->data=(char *)str_data;
					}
					else if (tmp1.id==TYPE_DOUBLE)
					{
						char *str_data=DoubleToString((*((int *)tmp1.ptr)));
						//free(tmp1.ptr);
						return_hitem->data=(char *)str_data;
					}
					else
					{
						//free(return_hitem->data);
						return_hitem->data=(char *)tmp1.ptr;
					}
				}
				L->Active=L->Active->next_instr;


			case I_IF:
			case I_WHILE:
				return_token=(token *)L->Active->adr1;
				return_hitem=stack_htab_find_htab_item(I_Htable, (char *)return_token->ptr);
				k=0;
				postfix_array=(token *)L->Active->adr2;
				ptr=postfix_array[k];
				while(postfix_array[k].id!=END_EXPR)
				{
					ptr=postfix_array[k++];
					switch (ptr.id)
					{
						case S_SIMPLE_IDENT:
						case S_FULL_IDENT:
							
							item_tmp1=stack_htab_find_htab_item(I_Htable, ptr.ptr);
							if (item_tmp1==NULL)
							{
								fprintf(stderr,"Interpret: Item wasn't found.\n");
								stack_expression_destroy(S);
								free(S);
								return ERR_OTHERS;
							}
							
							if (item_tmp1->initialized==0)
							{
								fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
								//freeALL();
								return ERR_UNINICIALIZED_VAR ;
							}
							if (item_tmp1->func_or_var==1)
							{
								new=malloc(sizeof(token));
								if (new==NULL)
									return ERR_INTERN_FAULT;

								if (item_tmp1->data_type==S_STRING)
									new->id=TYPE_STRING;
								else if (item_tmp1->data_type==S_DOUBLE)
									new->id=TYPE_DOUBLE;
								else
									new->id=TYPE_INT;							
								
								
								
								new->ptr=item_tmp1->data;
								stack_expression_push(S,*new);
							}
							//free(new);
							break;

						case TYPE_DOUBLE:
						case TYPE_INT:
						case TYPE_STRING:
							stack_expression_push(S,ptr);
							break;


						case S_EQUAL:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);
							new=inter_bool_op(tmp1,tmp2,1);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;
						case S_NOT_EQUAL:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,2);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_GREATER_EQUAL:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,3);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_GREATER:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,4);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_LESS_EQUAL:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,5);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_LESS:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,6);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_AND:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,7);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_OR:
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_bool_op(tmp1,tmp2,8);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;


						case S_PLUS:					// ----------- PLUS
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_plus(tmp1,tmp2);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;

						case S_MINUS:					//     ----------------------  MINUS

							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_arm_op(tmp1,tmp2,1);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;


						case S_MUL:					//     ----------------------  Multiplication
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_arm_op(tmp1,tmp2,2);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							stack_expression_push(S,*new);
							break;


						case S_DIV:					//     ----------------------  Division
							stack_expression_pop(S,&tmp2);
							stack_expression_pop(S,&tmp1);

							new=inter_arm_op(tmp1,tmp2,2);
							if (new==NULL)
								return ERR_INTERN_FAULT;
							if (new->id==-8)
							{
								fprintf(stderr, "Divison by zero!.\n");
								//freeALL();
								return ERR_DIVISION_ZERO ;
							}

							stack_expression_push(S,*new);
							break;
						}

					}

				stack_expression_pop(S,&tmp1);
				if (tmp1.id==S_FALSE)
				{
					if (L->Active->type_instr==I_IF)
					{
						L->Active=L->Active->next_instr;
						int count=0;
						while(count>0)
						{
							if (L->Active==NULL)
								return ERR_OTHERS;
							if (L->Active->type_instr==I_ENDIF)
								count--;
							else if (L->Active->type_instr==I_IF)
								count++;
							L->Active=L->Active->next_instr;
						}
					}
					else 
					{	
						L->Active=L->Active->next_instr;
						int count=0;
						while(count>0)
						{
							if (L->Active==NULL)
								return ERR_OTHERS;
							if (L->Active->type_instr==I_ENDWHILE)
								count--;
							else if (L->Active->type_instr==I_WHILE)
								count++;
							L->Active=L->Active->next_instr;
						}					
					}
				}
				L->Active=L->Active->next_instr;
				break;



			case I_FCE:
				return_hitem=(htab_item *)L->Active->adr1;
				k=0;	
				if (return_hitem==NULL)
					printf("null je\n");
				printf("%s\n",return_hitem->key);
				postfix_array=(token *)L->Active->adr2;
				
				if (strcmp(return_hitem->key,"ifj16.print")==0)
				{
					while(postfix_array[k].id!=END_EXPR)
						k++;
					postfix_array[k-1].id=END_EXPR;
					k=0;
					
					token *ret_value=malloc(sizeof(token));
					ret_value->id=TYPE_STRING;
					I_Instr *new1,*new2,*tmp1;
					new1=malloc(sizeof(I_Instr));
					new1->type_instr=I_ASSIGMENT;
					new1->adr1=ret_value;
					new1->adr2=postfix_array;
					new1->adr3=L->Active;
					
					tmp1=L->Active->next_instr;
					
					L->Active=new1;
					
					break;
				}
				
				//loc_table=(htab_t *)return_hitem->local_table;
			
				loc_table=htab_copy((htab_t *)return_hitem->local_table); 
				if(loc_table==NULL)
					printf("NULL\n");
					
				if (postfix_array!=NULL)
				{
					while(postfix_array[k].id!=END_EXPR)
					{	
						if (postfix_array[k].ptr==NULL)
							printf("ITS NULL! id: %d\n",postfix_array[k].id);
						else
							printf("K: %d  key:%s\n",k,(char *)postfix_array[k].ptr);
						ptr=postfix_array[k++];
						switch (ptr.id)
						{
							case S_SIMPLE_IDENT:
							case S_FULL_IDENT:
								
								item_tmp1=stack_htab_find_htab_item(I_Htable,(char*) ptr.ptr);
								if (item_tmp1==return_hitem)
									break;
								if (item_tmp1==NULL)
								{
									stack_expression_destroy(S);
									free(S);
									fprintf(stderr,"Interpret: Item wasn't found.\n");
									return ERR_OTHERS;
								}
								
								if (item_tmp1->initialized==0)
								{
									fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
									//freeALL();
									return ERR_UNINICIALIZED_VAR ;
								}
								if (item_tmp1->func_or_var==1)
								{
									new=malloc(sizeof(token));
									if (new==NULL)
										return ERR_INTERN_FAULT;

									if (item_tmp1->data_type==S_STRING)
										new->id=TYPE_STRING;
									else if (item_tmp1->data_type==S_DOUBLE)
										new->id=TYPE_DOUBLE;
									else
										new->id=TYPE_INT;							
									
									
									
									new->ptr=item_tmp1->data;
									stack_expression_push(S,*new);
								}
								//free(new);
								break;

							case TYPE_DOUBLE:
							case TYPE_INT:
							case TYPE_STRING:
								stack_expression_push(S,ptr);
								break;

							case S_PLUS:					// ----------- PLUS
								stack_expression_pop(S,&tmp2);
								stack_expression_pop(S,&tmp1);

								new=inter_plus(tmp1,tmp2);
								stack_expression_push(S,*new);
								break;

							case S_MINUS:					//     ----------------------  MINUS

								stack_expression_pop(S,&tmp2);
								stack_expression_pop(S,&tmp1);

								new=inter_arm_op(tmp1,tmp2,1);
								stack_expression_push(S,*new);
								break;


							case S_MUL:					//     ----------------------  Multiplication
								stack_expression_pop(S,&tmp2);
								stack_expression_pop(S,&tmp1);

								new=inter_arm_op(tmp1,tmp2,2);
								stack_expression_push(S,*new);
								break;


							case S_DIV:					//     ----------------------  Division
								stack_expression_pop(S,&tmp2);
								stack_expression_pop(S,&tmp1);

								new=inter_arm_op(tmp1,tmp2,2);
								if (new->id==-8)
								{
									fprintf(stderr, "Divison by zero!.\n");
									//freeALL();
									return ERR_DIVISION_ZERO ;
								}

								stack_expression_push(S,*new);
								break;


						}
						
					}
					
					int i=return_hitem->number_of_arguments;
					while (i>0)
					{
						i--;
						char *tab_key=IntToString(i);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(loc_table,tab_key);
						int *par_types=(int *)item_tmp1->data;
						
						if (par_types[i]==S_INT)
						{
							if (tmp1.id==TYPE_INT)
							{
								int *variable=malloc(sizeof(int));
								*variable=(*((int*)tmp1.ptr));
								item_tmp1->data=(int *)variable;
							}
						}
						else if (par_types[i]==S_DOUBLE)
						{	
							double *variable=malloc(sizeof(int));
							if (tmp1.id==TYPE_INT)
							{
								*variable=(*((int*)tmp1.ptr));
							}
							else
								*variable=(*((double*)tmp1.ptr));
							item_tmp1->data=(double *)variable;
						}
						else 
						{
							
							if (tmp1.id==TYPE_INT)
							{
								char *str_val=IntToString((*((int*)tmp1.ptr)));
								item_tmp1->data=(char *)str_val;
							}
							else if (tmp1.id==TYPE_DOUBLE)
							{
								char *str_val=DoubleToString((*((int*)tmp1.ptr)));
								item_tmp1->data=(char *)str_val;
							}
							else
							{ 
								char *str_val=mem_alloc(strlen("Main.run")+1);
								strcpy((char*)str_val,((char *) tmp1.ptr));
								item_tmp1->data=(char *)str_val;
							}
						}	
					}
					
					
				}	
				
				stack_htab_push(I_Htable, loc_table);
				L->Active=(I_Instr *)return_hitem->instruction_tape;
				break;
				
				
			
				
			case I_ENDIF:
				L->Active=L->Active->next_instr;
				if (L->Active->type_instr==I_ELSE)
				{
					int count=0;
					while (count>0)
					{
						L->Active=L->Active->next_instr;
						if (L->Active->type_instr==I_ENDELSE)
							count--;
						if (L->Active->type_instr==I_ELSE)	
							count++;
					}
					L->Active=L->Active->next_instr;
				}
				break;	
				
				
			default:
			break;


		}
		L->Active=L->Active->next_instr;
	}

	stack_expression_destroy(S);
	free(S);
	return 0;
}



int Add_Instr(Instr_List *L, I_Instr *new)
{
	if (new==NULL)
		return -1;
	if (L->Last==NULL)
	{
		L->Last=new;
		L->Active=new;
		return 0;
	}
	
	
	I_Instr *tmp=L->Last;
	tmp->next_instr=new;
	L->Last=new;
	tmp=L->Last->next_instr;
	while(tmp!=NULL)
	{
		L->Last=tmp;
		tmp=L->Last->next_instr;

	}
	return 0;
}


int is_emb_fce(htab_item *item_tmp1,token *postfix_array,token *return_token, stack_htab *I_Htable)
{
	if (strcmp(item_tmp1->key,"ifj16.readInt")==0)
	{
		free(return_token->ptr);
		int *new_val=malloc(sizeof(int));
		*new_val=readInt();
		return_token->ptr=(int *)new_val;
		return 1;
	}	
	if (strcmp(item_tmp1->key,"ifj16.readDouble")==0)
	{
		free(return_token->ptr);
		double *new_val=malloc(sizeof(double));
		*new_val=readInt();
		return_token->ptr=(double *)new_val;
		return 1;
	}	
	if (strcmp(item_tmp1->key,"ifj16.readString")==0)
	{
		free(return_token->ptr);
		char *new_val=readString();
		return_token->ptr=(char *)new_val;
		return 1;
	}		
	if (strcmp(item_tmp1->key,"ifj16.substr")==0)
	{
		free(return_token->ptr);
		char *s2,*s1;
		int val1,val2;
		if (postfix_array[0].id==S_STRING)
		{
			htab_item *s1_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[0].ptr);
			s1=s1_item->data;
		}
		else
			s1=(char *)postfix_array[0].ptr;
		
		if (postfix_array[1].id==S_INT)
		{
			htab_item *s2_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[1].ptr);
			val1=(*(int *)s2_item->data);
		}
		else
			val1=(*(int *)postfix_array[1].ptr);	
				
		if (postfix_array[2].id==S_INT)
		{
			htab_item *s2_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[1].ptr);
			val2=(*(int *)s2_item->data);
		}
		else
			val2=(*(int *)postfix_array[1].ptr);		
		
		s2=substring(s1,val1,val2);
		return_token->ptr=(char *)s2;
		return 1;
	}	
	if (strcmp(item_tmp1->key,"ifj16.compare")==0)
	{
		free(return_token->ptr);
		char *s2,*s1;
		if (postfix_array[0].id==S_STRING)
		{
			htab_item *s1_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[0].ptr);
			s1=s1_item->data;
		}
		else
			s1=(char *)postfix_array[0].ptr;
		
		if (postfix_array[1].id==S_STRING)
		{
			htab_item *s2_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[1].ptr);
			s2=s2_item->data;
		}
		else
			s2=(char *)postfix_array[1].ptr;
			
		int *val=malloc(sizeof(int));	
		*val=strcmp(s1,s2);
		return_token->ptr=(int *)val;
		return 1;
		
		
	}
	if (strcmp(item_tmp1->key,"ifj16.find")==0)
	{
		free(return_token->ptr);
		int *new_val=malloc(sizeof(int));
		char *s2,*s1;
		if (postfix_array[0].id==S_STRING)
		{
			htab_item *s1_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[0].ptr);
			s1=s1_item->data;
		}
		else
			s1=(char *)postfix_array[0].ptr;
		
		if (postfix_array[1].id==S_STRING)
		{
			htab_item *s2_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[1].ptr);
			s2=s2_item->data;
		}
		else
			s2=(char *)postfix_array[1].ptr;		
		
		*new_val=find(s1,s2);
		return_token->ptr=(int *)new_val;
		return 1;
	}
	if (strcmp(item_tmp1->key,"ifj16.sort")==0)
	{
		char *s2,*s1;
		if (postfix_array[0].id==S_STRING)
		{
			htab_item *s1_item=stack_htab_find_htab_item(I_Htable,(char *) postfix_array[0].ptr);
			s1=s1_item->data;
		}
		else
			s1=(char *)postfix_array[0].ptr;
		
		s2=shellsort(s1);	
			
		free(return_token->ptr);
		return_token->ptr=(char *)s2;	
		return 1;
	}
	return 0;
}
char *DoubleToString(double x)
{
    int req_size=8*sizeof(double)+3;
	char *str=malloc(req_size);
	if (str==NULL)
		return str;
	snprintf(str, req_size, "%f", x);
	return str;
}

char *IntToString(int x)
{
    int req_size=8*sizeof(int)+3;
	char *str=malloc(req_size);
	if (str==NULL)
		return str;
	snprintf(str, req_size, "%d", x);
	return str;
}

char *Conc_Str(char *s1, char *s2)
{
    int req_size=strlen(s1)+strlen(s2);
    char *str=malloc(req_size+1);
   	if (str==NULL)
		return str;
    int k=strlen(s1)-1;
    int i=0;
    while(k>=0)
    {
        str[i]=s1[i];
        i++;
        k--;
    }
    k=strlen(s2)-1;
    int j=0;
    while(k>=0)
    {
        str[i++]=s2[j++];
        k--;
    }
	return str;
}




token *inter_plus(token tmp1,token tmp2)
{
	token *new;
	if ((tmp1.id==TYPE_STRING) || tmp2.id==TYPE_STRING)            //     STRING CONCATENATE
	{
		char *str1,*str2;
		if (tmp1.id == TYPE_DOUBLE)
		{
			str1=DoubleToString((*((double *)tmp1.ptr)));
		}
		else if (tmp1.id == TYPE_INT)
		{
			str1=IntToString((*((int *)tmp1.ptr)));
		}
		else
			str1=(char *)tmp1.ptr;


		if (tmp2.id == TYPE_DOUBLE)
		{
			str2=DoubleToString((*((double *)tmp2.ptr)));

		}
		else if (tmp2.id == TYPE_INT)
		{
			str2=IntToString((*((int *)tmp2.ptr)));
		}
		else
			str2=(char *)tmp2.ptr;

		char *str3=Conc_Str(str1,str2);
		new=malloc(sizeof(token));
		if (new==NULL)
			return NULL;
		new->id=TYPE_STRING;
		new->ptr=(char *)str3;

		return new;
	}
	else if (tmp1.id==TYPE_DOUBLE && tmp2.id==TYPE_DOUBLE)
	{
		new=malloc(sizeof(token));
		if (new==NULL)
			return NULL;

		new->id=TYPE_DOUBLE;
		double *tmp_value=malloc(sizeof(double));
		if (tmp_value==NULL)
			return NULL;

		(*tmp_value)=(*((double *)tmp1.ptr))+(*((double *)tmp2.ptr));
		new->ptr=(double *)tmp_value;
		return new;
	}
	else if (tmp1.id==TYPE_DOUBLE && tmp2.id==TYPE_INT)
	{
		new=malloc(sizeof(token));
		if (new==NULL)
			return NULL;
		new->id=TYPE_DOUBLE;
		double *tmp_value=malloc(sizeof(double));
		if (tmp_value==NULL)
			return NULL;
		(*tmp_value)=(*((double *)tmp1.ptr))+(*((int *)tmp2.ptr));
		new->ptr=(double *)tmp_value;
		return new;
	}
	else if (tmp1.id==TYPE_INT && tmp2.id==TYPE_DOUBLE)
	{
		new=malloc(sizeof(token));
		if (new==NULL)
			return NULL;

		new->id=TYPE_DOUBLE;
		double *tmp_value=malloc(sizeof(double));
		if (tmp_value==NULL)
			return NULL;

		(*tmp_value)=(*((int *)tmp1.ptr))+(*((double *)tmp2.ptr));
		new->ptr=(double *)tmp_value;
		return new;
	}
	else
	{
		new=malloc(sizeof(token));
		if (new==NULL)
			return NULL;

		new->id=TYPE_INT;
		int *tmp_value=malloc(sizeof(int));
		if (tmp_value==NULL)
			return NULL;

		(*tmp_value)=(*((int *)tmp1.ptr))+(*((int *)tmp2.ptr));
		new->ptr=(int *)tmp_value;
		return new;
	}
}

token *inter_arm_op(token tmp1,token tmp2, int i)
{
	token *new=malloc(sizeof(token));
	if (new==NULL)
		return NULL;

	if (tmp1.id==TYPE_DOUBLE && tmp2.id==TYPE_DOUBLE)
	{
		new->id=TYPE_DOUBLE;
		double *tmp_value=malloc(sizeof(double));
		if (tmp_value==NULL)
			return NULL;
		if (i==1)
			(*tmp_value)=(*((double *)tmp1.ptr))-(*((double *)tmp2.ptr));
		else if (i==2)
			(*tmp_value)=(*((double *)tmp1.ptr))*(*((double *)tmp2.ptr));
		else
		{
			if ((*((double *)tmp2.ptr))==0.0)
			{
				free(tmp_value);
				new->id=-8;
				return new;
			}

			(*tmp_value)=(*((double *)tmp1.ptr))/(*((double *)tmp2.ptr));


		}
		new->ptr=(double *)tmp_value;
		return new;
	}
	else if (tmp1.id==TYPE_DOUBLE && tmp2.id==TYPE_INT)
	{
		new->id=TYPE_DOUBLE;
		double *tmp_value=malloc(sizeof(double));
		if (tmp_value==NULL)
			return NULL;
		if (i==1)
			(*tmp_value)=(*((double *)tmp1.ptr))-(*((int *)tmp2.ptr));
		else if (i==2)
			(*tmp_value)=(*((double *)tmp1.ptr))*(*((int *)tmp2.ptr));
		else
		{
			if ((*((int *)tmp2.ptr)==0))
			{
				free(tmp_value);
				new->id=-8;
				return new;
			}

			(*tmp_value)=(*((double *)tmp1.ptr))/(*((int *)tmp2.ptr));


		}
		new->ptr=(double *)tmp_value;
		return new;
	}
	else if (tmp1.id==TYPE_INT && tmp2.id==TYPE_DOUBLE)
	{
		new->id=TYPE_DOUBLE;
		double *tmp_value=malloc(sizeof(double));
		if (tmp_value==NULL)
			return NULL;
		if (i==1)
			(*tmp_value)=(*((int *)tmp1.ptr))-(*((double *)tmp2.ptr));
		else if (i==2)
			(*tmp_value)=(*((int *)tmp1.ptr))*(*((double *)tmp2.ptr));
		else
		{
			if ((*((double *)tmp2.ptr)==0.0))
			{
				free(tmp_value);
				new->id=-8;
				return new;
			}

			(*tmp_value)=(*((double *)tmp1.ptr))/(*((double *)tmp2.ptr));


		}

		new->ptr=(double *)tmp_value;
		return new;
	}
	else
	{
		new->id=TYPE_INT;
		int *tmp_value=malloc(sizeof(int));
		if (tmp_value==NULL)
			return NULL;
		if (i==1)
			(*tmp_value)=(*((int *)tmp1.ptr))-(*((int *)tmp2.ptr));
		else if (i==2)
			(*tmp_value)=(*((int *)tmp1.ptr))*(*((int *)tmp2.ptr));
		else
		{
			if ((*((int *)tmp2.ptr)==0))
			{
				free(tmp_value);
				new->id=-8;
				return new;
			}

			(*tmp_value)=(*((double *)tmp1.ptr))/(*((double *)tmp2.ptr));

		}


		new->ptr=(int *)tmp_value;
		return new;
	}
}

token *inter_bool_op(token tmp1,token tmp2, int i)     // i- 1 (==)  2 (!=) 3 (>=) 4 (<=)
{
	token *new=malloc(sizeof(token));
	if (new==NULL)
		return NULL;
	if (tmp1.id==TYPE_DOUBLE && tmp2.id==TYPE_DOUBLE)
	{
		if (i==1)
			if((*((double *)tmp1.ptr))==(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		else if (i==2)
		{
			if((*((double *)tmp1.ptr))!=(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==3)
		{
			if((*((double *)tmp1.ptr))>=(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==4)
		{
			if((*((double *)tmp1.ptr))>(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}

		else if(i==5)
		{
			if((*((double *)tmp1.ptr))<=(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==6)
		{
			if((*((double *)tmp1.ptr))<(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}

	}
	else if (tmp1.id==TYPE_DOUBLE && tmp2.id==TYPE_INT)
	{
		if (i==1)
			if((*((double *)tmp1.ptr))==(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		else if (i==2)
		{
			if((*((double *)tmp1.ptr))!=(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==3)
		{
			if((*((double *)tmp1.ptr))>=(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==4)
		{
			if((*((double *)tmp1.ptr))>(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}

		else if(i==5)
		{
			if((*((double *)tmp1.ptr))<=(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==6)
		{
			if((*((double *)tmp1.ptr))<(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
	}
	else if (tmp1.id==TYPE_INT && tmp2.id==TYPE_DOUBLE)
	{
		if (i==1)
			if((*((int *)tmp1.ptr))==(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		else if (i==2)
		{
			if((*((int *)tmp1.ptr))!=(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==3)
		{
			if((*((int *)tmp1.ptr))>=(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==4)
		{
			if((*((int *)tmp1.ptr))>(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}

		else if(i==5)
		{
			if((*((int *)tmp1.ptr))<=(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==6)
		{
			if((*((int *)tmp1.ptr))<(*((double *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
	}
	else if (tmp1.id==TYPE_INT && tmp2.id==TYPE_INT)
	{
		if (i==1)
			if((*((int *)tmp1.ptr))==(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		else if (i==2)
		{
			if((*((int *)tmp1.ptr))!=(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==3)
		{
			if((*((int *)tmp1.ptr))>=(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==4)
		{
			if((*((int *)tmp1.ptr))>(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}

		else if(i==5)
		{
			if((*((int *)tmp1.ptr))<=(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if(i==6)
		{
			if((*((int *)tmp1.ptr))<(*((int *)tmp2.ptr)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
	}
	else
	{
		if (i==1)
		{
			if ((tmp1.id==S_TRUE && tmp2.id==S_TRUE) || (tmp1.id==S_FALSE && tmp2.id==S_FALSE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==2)
		{
			if (((tmp1.id==S_TRUE) && (tmp1.id==S_FALSE)) || ((tmp1.id==S_FALSE) && (tmp2.id==S_TRUE)))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==3)
		{
			if ((tmp1.id==S_TRUE) || (tmp1.id==S_FALSE && tmp2.id==S_FALSE) || (tmp1.id==S_TRUE && tmp2.id==S_TRUE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==4)
		{
			if ((tmp1.id==S_TRUE) && (tmp2.id==S_FALSE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==5)
		{
			if ((tmp1.id==S_FALSE) || (tmp1.id==S_FALSE && tmp2.id==S_FALSE) || (tmp1.id==S_TRUE && tmp2.id==S_TRUE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==6)
		{
			if ((tmp1.id=S_FALSE) && (tmp2.id==S_TRUE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==7)
		{
			if ((tmp1.id=S_TRUE) && (tmp2.id==S_TRUE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}
		else if (i==8)
		{
			if ((tmp1.id=S_TRUE) || (tmp2.id==S_TRUE))
				new->id=S_TRUE;
			else
				new->id=S_FALSE;
		}

	}
	return new;


}
