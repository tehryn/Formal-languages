#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>

	
	
int main()
{
	htab_t *I_Htable= htab_init(20);

	htab_item *new_item;
	int bbb=15;
	char *key1;
	token *first=malloc(sizeof(token));
	first->id=TYPE_INT;
	
	key1=IntToString(bbb++);
	
	new_item=htab_insert_item(I_Htable, key1);
	new_item->initialized=1;
	int *i=malloc(sizeof(int));
	*i=10;
	new_item->data=(void *)(i);

	
	
	first->ptr=(void *)key1;
	
	
	//    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa	
	htab_item *new_item1;
	token *second=malloc(sizeof(token));
	second->id=TYPE_INT;
	
	key1=IntToString(bbb++);
	
	new_item1=htab_insert_item(I_Htable, key1);
	new_item1->initialized=1;
	int *j=malloc(sizeof(int));
	*j=5;
	new_item1->data=(void *)(j);
		
	second->ptr=(void *)key1;	
	
	token *third=malloc(sizeof(token));	
	third->id=S_PLUS;
	
	
// NEWWWW PART

	htab_item *new_item2;
	token *first1=malloc(sizeof(token));
	first1->id=TYPE_INT;
	
	key1=IntToString(bbb++);
	
	new_item2=htab_insert_item(I_Htable, key1);
	new_item2->initialized=1;
	int *v=malloc(sizeof(int));
	*v=6;
	new_item2->data=(void *)(v);

	
	
	first1->ptr=(void *)key1;
	
	
	//    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa	
	htab_item *new_item3;
	token *second1=malloc(sizeof(token));
	second1->id=TYPE_INT;
	
	key1=IntToString(bbb++);
	
	new_item3=htab_insert_item(I_Htable, key1);
	new_item3->initialized=1;
	int *n=malloc(sizeof(int));
	*n=3;
	new_item3->data=(void *)(n);
		
	second1->ptr=(void *)key1;
	
	token *third1=malloc(sizeof(token));	
	third1->id=S_MUL;
	
	token *third2=malloc(sizeof(token));	
	third2->id=S_MINUS;	
		
// END OF NEW PART	
	
	
	token *end=malloc(sizeof(token));	
	end->id=-2;	
	
	token *token_array[8];
	token_array[0]=first;
	token_array[1]=second;
	token_array[2]=third;
	token_array[3]=first1;
	token_array[4]=second1;
	token_array[5]=third1;	
	token_array[6]=third2;		
	
	
	token_array[7]=end;
		
	token *return_token=malloc(sizeof(token));
	return_token->id=TYPE_INT;
	
	key1=IntToString(bbb++);
	
	new_item1=htab_insert_item(I_Htable, key1);
	new_item1->initialized=1;
	int *m=malloc(sizeof(int));
	new_item1->data=(void *)(m);
		
	return_token->ptr=(void *)new_item1;	
	Instr_List *L=malloc(sizeof(Instr_List));
	
	I_Instr Instr1;
	Instr1.type_instr=I_IF;
	Instr1.adr1=(void*)return_token;
	Instr1.adr2=(void*)token_array;
	Instr1.adr3=NULL;
	Instr1.next_instr=NULL;
	L->Active=(&Instr1);
	L->Last=(&Instr1);
	printf("START\n");
	inter(L,I_Htable);
	printf("END\n");
	return_token=(token*)Instr1.adr1;
	htab_item *return_value=(htab_item *)return_token->ptr;
	int zz=(*((int *)return_value->data));
	printf("aaaa %d\n",zz);	
	
	
	free(i);free(j);free(v);free(n);free(m);free(L);free(return_token);
	for (int l=0;l<8;l++)
		free(token_array[l]);
	
	htab_free_all(I_Htable);	
	
	
	return 0;
}


int inter(Instr_List *L, htab_t *I_Htable)
{
	struct stack_expresion *S=malloc(sizeof(struct stack_expresion));
	stack_expression_init(S,I_STACKSIZE);
	int key=0;
	htab_item * item_tmp1;
	htab_item * item_tmp2;
	htab_item * result_item;
	htab_item * return_hitem;
	token *return_token;	
	int k=0;
	token **postfix_array;
	token *ptr, tmp1, tmp2, *new;	
	

    switch (L->Active->type_instr)
    {

		case I_ASSIGMENT:
			return_token=(token *)L->Active->adr1;
			return_hitem=(htab_item *)return_token->ptr;
			k=0;
			postfix_array=(token **)L->Active->adr2;
			while(postfix_array[k]->id!=-2)
			{
				ptr=postfix_array[k++];
				printf("id :%d\n",ptr->id);
				switch (ptr->id)
				{
					default:
						stack_expression_push(S,*ptr);
						
						break;
					
					case S_PLUS:					// ----------- PLUS
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
						
						printf("A1 %s\n",(const char*)tmp1.ptr);
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}

						printf("A1\n");
						if (return_token->id==TYPE_DOUBLE)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))+(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							
							break;
						}
						else if (return_token->id==TYPE_INT)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))+(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}	

						if (return_token->id==TYPE_STRING)            //     STRING CONCATENATE
						{
							char *str1,*str2;
							if (tmp1.id == TYPE_DOUBLE)
							{
								str1=DoubleToString((*((double *)item_tmp1->data)));
							}
							else if (tmp1.id == TYPE_INT)
							{
								str1=IntToString((*((int *)item_tmp1->data)));
							}
							else 
								str1=(char *)item_tmp1->data;
							
							
							if (tmp2.id == TYPE_DOUBLE)
							{
								str2=DoubleToString((*((double *)item_tmp2->data)));

							}
							else if (tmp2.id == TYPE_INT)
							{
								str2=IntToString((*((int *)item_tmp1->data)));
							}
							else 
								str2=(char *)item_tmp2->data;	
								
							char *str3=Conc_Str(str1,str2);
							new=Create_New_token(TYPE_STRING,I_Htable,key++);
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)str3;
					
							result_item->initialized=1;
							stack_expression_push(S,*new);							
							
					
							
							break;
						}						
										
						
						
					case S_MINUS:					//     ----------------------  MINUS
						
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
			
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}
						
						if (return_token->id==TYPE_DOUBLE)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))-(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
					
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}
						
						
						
						
						
						if (return_token->id==TYPE_INT)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))-(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break; 
						}	

					case S_MUL:					//     ----------------------  Multiplication
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
								
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}

						
						if (return_token->id==TYPE_DOUBLE)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))*(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}
						
						
						
						
						
						if (return_token->id==TYPE_INT)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))*(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}	

					case S_DIV:					//     ----------------------  Division 
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
								
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}

						
						if (return_token->id==TYPE_DOUBLE)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							if ((*((double *)item_tmp2->data))==0.0)
							{
								fprintf(stderr, "In line %d division by zero.\n", LINE_NUM);
								//freeALL();
								return 9;								
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))/(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}
						
						
						
						
						
						if (return_token->id==TYPE_INT)
						{
							if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
							{
								fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
								//freeALL();
								return ERR_SEM_COMPATIBILITY;
							}
							if ((*((int *)item_tmp2->data))==0)
							{
								fprintf(stderr, "In line %d division by zero.\n", LINE_NUM);
								//freeALL();
								return 9;								
							}
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))/(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;  
						}
						
				}
				
			}
			
			
			stack_expression_pop(S,&tmp1);
			item_tmp1=htab_find_item(I_Htable,(const char *)tmp1.ptr);
			void *tmp_void=return_hitem->data;
			return_hitem->data=item_tmp1->data;
			item_tmp1->data=tmp_void;
			L->Active=L->Active->next_instr;
			break;
		
		case I_IF:
		case I_WHILE:
			return_token=(token *)L->Active->adr1;
			return_hitem=(htab_item *)return_token->ptr;
			k=0;
			postfix_array=(token **)L->Active->adr2;
			printf("IF WHILE\n");
			while(postfix_array[k]->id!=-2)
			{
				printf("a1\n");
				ptr=postfix_array[k++];
				printf("IF WHILE %d\n",ptr->id);
				switch (ptr->id)
				{
					default:
						stack_expression_push(S,*ptr);
						break;
					
					case S_PLUS:					// ----------- PLUS
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
						
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}

						if (tmp1.id==TYPE_STRING || tmp2.id==TYPE_STRING)            //     STRING CONCATENATE
						{
							char *str1,*str2;
							if (tmp1.id == TYPE_DOUBLE)
							{
								str1=DoubleToString((*((double *)item_tmp1->data)));
							}
							else if (tmp1.id == TYPE_INT)
							{
								str1=IntToString((*((int *)item_tmp1->data)));
							}
							else 
								str1=(char *)item_tmp1->data;
							
							
							if (tmp2.id == TYPE_DOUBLE)
							{
								str2=DoubleToString((*((double *)item_tmp2->data)));

							}
							else if (tmp2.id == TYPE_INT)
							{
								str2=IntToString((*((int *)item_tmp1->data)));
							}
							else 
								str2=(char *)item_tmp2->data;	
								
							char *str3=Conc_Str(str1,str2);
							new=Create_New_token(TYPE_STRING,I_Htable,key++);
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)str3;
					
							result_item->initialized=1;
							stack_expression_push(S,*new);							
							break;
						}	
						
						else if (tmp1.id==TYPE_DOUBLE || tmp2.id==TYPE_DOUBLE)
						{
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))+(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							break;
						}
						else
						{
							new=Create_New_token(TYPE_INT,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))+(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}	

					
										
						
						
					case S_MINUS:					//     ----------------------  MINUS
						
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
			
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}
						if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
						{
							fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
							//freeALL();
							return ERR_SEM_COMPATIBILITY;
						}			
									
						if (tmp1.id==TYPE_DOUBLE || tmp2.id==TYPE_DOUBLE)
						{
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))-(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
					
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}
						
						else
						{
							new=Create_New_token(TYPE_INT,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))-(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break; 
						}	

					case S_MUL:					//     ----------------------  Multiplication

						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
			
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}
						if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
						{
							fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
							//freeALL();
							return ERR_SEM_COMPATIBILITY;
						}			
									
						if (tmp1.id==TYPE_DOUBLE || tmp2.id==TYPE_DOUBLE)
						{
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));
							
							(*tmp_value)=(*((double *)item_tmp1->data))*(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
					
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}
						
						else
						{
							new=Create_New_token(TYPE_INT,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))*(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break; 
						}	

					case S_DIV:					//     ----------------------  Division 
						stack_expression_pop(S,&tmp2);
						stack_expression_pop(S,&tmp1);
						item_tmp1=htab_find_item(I_Htable, (const char*)tmp1.ptr);
						item_tmp2=htab_find_item(I_Htable, (const char*)tmp2.ptr);	
			
						if (item_tmp1->initialized==0 || item_tmp2->initialized==0)
						{
							fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
							//freeALL();
							return ERR_UNINICIALIZED_VAR ;					
						}
						if ((tmp1.id==TYPE_STRING) ||  (tmp2.id==TYPE_STRING))
						{
							fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
							//freeALL();
							return ERR_SEM_COMPATIBILITY;
						}	
								
						if ((*((double *)item_tmp2->data))==0)
						{
							fprintf(stderr, "Error! Divsion by zero.\n");
							//freeALL();
							return ERR_DIVISION_ZERO;							
						}	
														
						if (tmp1.id==TYPE_DOUBLE || tmp2.id==TYPE_DOUBLE)
						{
							new=Create_New_token(TYPE_DOUBLE,I_Htable,key++);
							double *tmp_value=malloc(sizeof(double));

							(*tmp_value)=(*((double *)item_tmp1->data))-(*((double *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
					
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break;
						}
						
						else
						{
							new=Create_New_token(TYPE_INT,I_Htable,key++);
							int *tmp_value=malloc(sizeof(int));
							
							(*tmp_value)=(*((int *)item_tmp1->data))-(*((int *)item_tmp2->data));
							
							result_item=htab_find_item(I_Htable,(const char *)new->ptr);
							result_item->data=(void*)tmp_value;
							result_item->initialized=1;
							stack_expression_push(S,*new);
							
							break; 
						}
						
				}
			}		
			stack_expression_pop(S,&tmp1);
			item_tmp1=htab_find_item(I_Htable,(const char *)tmp1.ptr);
			int rez=(*((int *)item_tmp1->data));
			I_Instr *tmp=L->Active;
			if (L->Active->type_instr==I_IF)
			{
				printf("rezultat :%d\n",rez);
				if (rez==0)
				{				
					while (L->Active->type_instr!=I_END)
					{
						L->Active=tmp->next_instr;
						tmp=L->Active;
						if (L->Active==NULL)
							break;
					}
					L->Active->next_instr=tmp->next_instr;
				}
				else
					if (L->Active->type_instr==I_IF)
						L->Active->next_instr=tmp->next_instr;					
			}

			
			
			
			
			
			
			
			
		default:
		break;
		

	}
//	stack_expression_destroy(S);
	free(S);
	return 0;
}




token *Create_New_token(int id, htab_t *T, int key)
{
	token *new=malloc(sizeof(token));
	new->id=id;
	char *str_key=IntToString(key);
	htab_item *TableItem = htab_insert_item(T, str_key);
	new->ptr=(void *)TableItem->key;
	return new;
}







char *DoubleToString(double x)
{
    int req_size=8*sizeof(double)+3;
	char *str=malloc(req_size);
	snprintf(str, req_size, "%f", x);
	return str;
}

char *IntToString(int x)
{
    int req_size=8*sizeof(int)+3;
	char *str=malloc(req_size);
	snprintf(str, req_size, "%d", x);
	return str;
}

char *Conc_Str(char *s1, char *s2)
{
    int req_size=strlen(s1)+strlen(s2);
    char *str=malloc(req_size+1);
    int k=strlen(s1)-1;
    int i=0;
    while(k>=0)
    {
        str[i]=s1[i++];
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









