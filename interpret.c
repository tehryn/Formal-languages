#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include "embedded_functions.h"
#define I_STACKSIZE 32

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

int inter(Instr_List *L, stack_htab *I_Htable,token *fce_token, int void_flag)
{
	struct stack_expresion *S=malloc(sizeof(struct stack_expresion));
	if (S==NULL)
		return ERR_INTERN_FAULT;

	I_Instr *while_instr;
	stack_instr while_stack;
	stack_instr_init(&while_stack);
	stack_instr fce_stack;
	stack_instr_init(&fce_stack);
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
		//printf("type of instr interpret: %d\n",L->Active->type_instr);
		//if (L->Active->next_instr!=NULL)
			//printf("type of next instr interpret: %d\n",L->Active->next_instr->type_instr);

		switch (L->Active->type_instr)
		{
			case I_ASSIGMENT:
				return_token=(token *)L->Active->adr1;
				return_hitem=stack_htab_find_htab_item(I_Htable,(char *) return_token->ptr);
				k=0;
				postfix_array=(token *)L->Active->adr2;
				ptr=postfix_array[k];
				
				
				new=do_expression(postfix_array,I_Htable,S,L,0);
				if (new->id < 0)
				{
					return -(new->id);
					
				}
				if(new->id==I_FCE)
				{
					stack_instr_push(&fce_stack,L->Active->next_instr);
					break;
					
				}

				if(return_hitem->data_type==S_DOUBLE && new->id==TYPE_DOUBLE)
				{
					return_hitem->data=(double *)new->ptr;
				}
				else if (return_hitem->data_type==S_DOUBLE && new->id==TYPE_INT)
				{
					double *new_val1=malloc(sizeof(double));
					double help_val=(*((int*)new->ptr));
					*new_val1=help_val;
					free(new->ptr);
					return_hitem->data=(double *)new_val1;
					
					
				}
				else if(return_hitem->data_type==S_INT)
				{
					//free(return_hitem->data);
					return_hitem->data=(int *)new->ptr;
				}
				else if(return_hitem->data_type==S_STRING)
				{
					if (new->id==TYPE_INT)
					{
						char *str_data=IntToString((*((int *)new->ptr)));
						//(new->ptr);
						return_hitem->data=(char *)str_data;
					}
					else if (new->id==TYPE_DOUBLE)
					{
						char *str_data=DoubleToString((*((int *)new->ptr)));
						//free(new->ptr);
						return_hitem->data=(char *)str_data;
					}
					else
					{
						//free(return_hitem->data);
						return_hitem->data=(char *)new->ptr;
					}
				}
				else
				{
					return_hitem->data_type=new->id;
				}
				
				//printf("INITIALIZED ITEM var or fce value : %d\n",return_hitem->func_or_var);
				return_hitem->initialized=1;
				L->Active=L->Active->next_instr;
				break;

			case I_IF:
				k=0;
				postfix_array=(token *)L->Active->adr1;
				new=do_expression(postfix_array,I_Htable,S,L,0);
				if (new->id < 0)
				{
					return -(new->id);
					
				}
				if (new->id==S_FALSE)
				{
					int count=0;
					while(count>=0)
					{
						L->Active=L->Active->next_instr;
						if (L->Active==NULL)
							return ERR_OTHERS;
						if (L->Active->type_instr==I_ENDIF)
							count--;
						if(count<0)
							break;
						else if (L->Active->type_instr==I_IF)
							count++;
					}
					if (L->Active->next_instr->type_instr==I_ELSE)
						L->Active=L->Active->next_instr;
				}				
				L->Active=L->Active->next_instr;
				break;

			case I_WHILE:
				k=0;
				postfix_array=(token *)L->Active->adr1;
				new=do_expression(postfix_array,I_Htable,S,L,0);
				
				
				if (new->id < 0)
				{
					return -(new->id);
					
				}
				
				if (new->id==S_FALSE)
				{
					int count=0;
					while(count>=0)
					{
						L->Active=L->Active->next_instr;
						if (L->Active==NULL)
							return ERR_OTHERS;
						if (L->Active->type_instr==I_ENDWHILE)
							count--;
						else if (L->Active->type_instr==I_WHILE)
							count++;
					}
				}
				else
				{
					stack_instr_push(&while_stack,L->Active);// TODO PUSH				
										
				}
				L->Active=L->Active->next_instr;
				break;
				
			
			case I_ENDWHILE:
				
				while_instr=stack_instr_pop(&while_stack);
				if (while_instr!=NULL)
					L->Active=while_instr;
				else
					L->Active=L->Active->next_instr;
				break;

			case I_FCE:
				item_tmp1=(htab_item *)L->Active->adr1;
							
					
					
				k=0;	
				postfix_array=(token *)L->Active->adr2;
				
				if (strcmp(item_tmp1->key,"ifj16.print")==0)
				{
					char *help_tmp;
					token *str_token=NULL;
					str_token=do_expression(postfix_array,I_Htable,S,L,0);
					
					if (str_token==NULL)
					{
						L->Active=L->Active->next_instr;
						break;
					}
					
					
					
					if(str_token->id<0)
					{
						return -(str_token->id);
					}
					
					
					
					if (str_token->id < 0)
					{
					return -(new->id);
					
					}

					//printf("str token id: %s\n",(char *)str_token->ptr);
					
					if (str_token->id==TYPE_DOUBLE || str_token->id==S_DOUBLE )
						help_tmp=DoubleToString(*((double*)str_token->ptr));
					else if (str_token->id==TYPE_INT || str_token->id==S_INT)
						help_tmp=IntToString(*((int*)str_token->ptr));
					else 
						help_tmp=str_token->ptr;
					
					print(help_tmp);
//					print("\n");
					free(str_token->ptr);
					free(str_token);
					L->Active=L->Active->next_instr;
					break;
					//printf("%d\n",str
					
				}
				//htab_t *loc_table=htab_copy((htab_t *)item_tmp1->local_table);
			
				
			
				
				htab_item *parametr;
				int par_type;
				int void_func_flag=0;
				
				
				if(item_tmp1->data_type==S_VOID)
					void_func_flag=1;
				
				
				//stack_htab_push(I_Htable, loc_table);
				new=do_expression(postfix_array,I_Htable,S,L,void_func_flag);
				
				
				//printf("express vration :%d\n",new->id);
				if (new->id < 0)
				{
					return -(new->id);
				}	
				
			
				
				return 0;	
				break;
				
				
			
				
			case I_ENDIF:
				if (L->Active->next_instr->type_instr==I_ELSE)
				{
					L->Active=L->Active->next_instr;
					
					int count=0;
					while (count>=0)
					{
						L->Active=L->Active->next_instr;
						if (L->Active->type_instr==I_ENDELSE)
							count--;
						if (L->Active->type_instr==I_ELSE)	
							count++;
					}
						
				}
				L->Active=L->Active->next_instr;
				break;	
				
			case I_END:
				
				//printf("VOID FLAG U ENDU: %d\n",void_flag);
				if (void_flag==1)
					return 0;
				else 
					return -8;
				
				
			case I_RETURN:
				
				if (L->Active->adr1==NULL)
					return 0;
				postfix_array=L->Active->adr1;				
				
				new=do_expression(postfix_array,I_Htable,S,L,0);
				
				
				if (new->id < 0)
				{
					return -(new->id);
					
				}
				
				if(fce_token->id==S_INT)
				{
					
					*(int*)fce_token->ptr=(*(int*)new->ptr);
					
				
				}
				else if(fce_token->id==S_DOUBLE && new->id==S_DOUBLE)
					*(double*)fce_token->ptr=(*(double*)new->ptr);
				else if(fce_token->id==S_DOUBLE && new->id==S_INT)
					*(double*)fce_token->ptr=(*(int*)new->ptr);
				else if(fce_token->id==S_STRING)
				{
					char *new_val=malloc(sizeof(char)*strlen((char*)new->ptr)+1);
					memcpy(new_val,(char*)new->ptr,strlen((char*)new->ptr)+1);
					fce_token->ptr=(char*)new_val;	
				}
				else 
					fce_token->id=new->id;
				
				return 0;
				
			case I_ELSE:
				
				
			default:
				L->Active=L->Active->next_instr;
			break;


		}
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

int is_emb_fce(htab_item *item_tmp1,struct stack_expresion *S,token *return_token)
{

	if (strcmp(item_tmp1->key,"ifj16.readInt")==0)
	{
		return_token->id=TYPE_INT;
		int *new_val=malloc(sizeof(int));
		*new_val=readInt();
		return_token->ptr=(int *)new_val;
		return 1;
	}	
	else if (strcmp(item_tmp1->key,"ifj16.readDouble")==0)
	{
		return_token->id=TYPE_DOUBLE;
		double *new_val=malloc(sizeof(double));
		*new_val=readDouble();
		return_token->ptr=(double *)new_val;
		return 1;
	}	
	else if (strcmp(item_tmp1->key,"ifj16.readString")==0)
	{
		return_token->id=TYPE_STRING;
		//free(return_token->ptr);
		char *new_val=readString();
		return_token->ptr=(char *)new_val;
		return 1;
	}		
	else if (strcmp(item_tmp1->key,"ifj16.substr")==0)
	{
		return_token->id=TYPE_STRING;
		token tmp1,tmp2,tmp3;
		stack_expression_pop(S,&tmp3);
		stack_expression_pop(S,&tmp2);
		stack_expression_pop(S,&tmp1);
		
		char *s2=substring(((char*)tmp1.ptr),(*(int*)tmp2.ptr),(*(int*)tmp3.ptr));
		return_token->ptr=(char *)s2;
		return 1;
	}	
	else if (strcmp(item_tmp1->key,"ifj16.compare")==0)
	{
		return_token->id=TYPE_INT;
		token tmp1,tmp2;
		stack_expression_pop(S,&tmp2);
		stack_expression_pop(S,&tmp1);
			
		int *val=malloc(sizeof(int));	
		*val=strcmp(((char*)tmp1.ptr),((char*)tmp2.ptr));
		
		if (*val > 0 )
			*val=1;
		else if (*val < 0 )
			*val=-1;
		
		return_token->ptr=(int *)val;
		return 1;
		
		
	}
	else if (strcmp(item_tmp1->key,"ifj16.find")==0)
	{
		return_token->id=TYPE_INT;
		int *new_val=malloc(sizeof(int));
		
		token tmp1,tmp2;
		stack_expression_pop(S,&tmp2);
		stack_expression_pop(S,&tmp1);
					
		
		*new_val=find((char*)tmp1.ptr,(char*)tmp2.ptr);
		return_token->ptr=(int *)new_val;
		return 1;
	}
	else if (strcmp(item_tmp1->key,"ifj16.sort")==0)
	{
		return_token->id=TYPE_STRING;
				
		token tmp1;
		stack_expression_pop(S,&tmp1);
				
		char *s2=shellsort((char*)tmp1.ptr);	
			
		//free(return_token->ptr);
		return_token->ptr=(char *)s2;	
		return 1;
	}
	else if (strcmp(item_tmp1->key,"ifj16.print")==0)
	{
		token tmp1;
		stack_expression_pop(S,&tmp1);
		
		if (tmp1.id==TYPE_INT || tmp1.id==S_INT )
			printf("%d",(*(int*)tmp1.ptr));
		else if (tmp1.id==TYPE_DOUBLE || tmp1.id==S_DOUBLE)
			printf("%g",(*(double*)tmp1.ptr));
		else if (tmp1.id==TYPE_STRING || tmp1.id==S_STRING)
			printf("%s",((char*)tmp1.ptr));
		return -1;
	}
	return 0;
}
char *DoubleToString(double x)
{
    int req_size=8*sizeof(double)+3;
	char *str=malloc(req_size);
	if (str==NULL)
		return str;
	snprintf(str, req_size, "%g", x);
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
    str[req_size]='\0';
	return str;
}




token *inter_plus(token tmp1,token tmp2)
{
	
	token *new;
	if ((tmp1.id==TYPE_STRING || tmp1.id==S_STRING) || (tmp2.id==TYPE_STRING || tmp2.id==S_STRING))            //     STRING CONCATENATE
	{
		char *str1,*str2;
		if (tmp1.id == TYPE_DOUBLE || tmp1.id == S_DOUBLE)
		{
			str1=DoubleToString((*((double *)tmp1.ptr)));
		}
		else if (tmp1.id == TYPE_INT || tmp1.id == S_INT)
		{
			str1=IntToString((*((int *)tmp1.ptr)));
		}
		else
			str1=(char *)tmp1.ptr;


		if (tmp2.id == TYPE_DOUBLE || tmp2.id == S_DOUBLE)
		{
			str2=DoubleToString((*((double *)tmp2.ptr)));

		}
		else if (tmp2.id == TYPE_INT || tmp2.id == S_INT)
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
	else if ((tmp1.id==S_DOUBLE || tmp1.id==TYPE_DOUBLE) && (tmp2.id==S_DOUBLE || tmp2.id==TYPE_DOUBLE))
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
	else if ((tmp1.id==S_DOUBLE || tmp1.id==TYPE_DOUBLE) && (tmp2.id==S_INT || tmp2.id==TYPE_INT))
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
	else if ((tmp1.id==S_INT || tmp1.id==TYPE_INT) && (tmp2.id==S_DOUBLE || tmp2.id==TYPE_DOUBLE))
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

	if ((tmp1.id==S_DOUBLE || tmp1.id==TYPE_DOUBLE) && (tmp2.id==TYPE_DOUBLE || tmp2.id==S_DOUBLE))
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
				//free(tmp_value);
				new->id=-8;
				return new;
			}

			(*tmp_value)=(*((double *)tmp1.ptr))/(*((double *)tmp2.ptr));


		}
		new->ptr=(double *)tmp_value;
		return new;
	}
	else if ((tmp1.id==S_DOUBLE || tmp1.id==TYPE_DOUBLE) && (tmp2.id==TYPE_INT || tmp2.id==S_INT))
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
	else if ((tmp1.id==TYPE_INT || tmp1.id==S_INT) && (tmp2.id==TYPE_DOUBLE || tmp2.id==S_DOUBLE))
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

			(*tmp_value)=(*((int *)tmp1.ptr))/(*((int *)tmp2.ptr));

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
	if ((tmp1.id==S_DOUBLE || tmp1.id==TYPE_DOUBLE) && (tmp2.id==S_DOUBLE || tmp2.id==TYPE_DOUBLE))
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
	else if ((tmp1.id==S_DOUBLE || tmp1.id==TYPE_DOUBLE) && (tmp2.id==S_INT || tmp2.id==TYPE_INT))
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
	else if ((tmp1.id==S_INT || tmp1.id==TYPE_INT) && (tmp2.id==S_DOUBLE || tmp2.id==TYPE_DOUBLE))
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
	else if ((tmp1.id==S_INT || tmp1.id==TYPE_INT) && (tmp2.id==S_INT || tmp2.id==TYPE_INT))
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
void I_Instr_null_elements(I_Instr * Instruction)
{
	Instruction->type_instr = 0;
	Instruction->adr1 = NULL;
	Instruction->adr2 = NULL;
	Instruction->adr3 = NULL;
	Instruction->next_instr = NULL;
}

token *do_expression(token *postfix_array, stack_htab *I_Htable,struct stack_expresion *S,Instr_List *L,int void_flag)
{
	token *new_token=malloc(sizeof(token));
	token ptr,tmp1,tmp2;
	token *new;
	int k=0;
	htab_item *item_tmp1;
	while(postfix_array[k].id!=END_EXPR)
	{	
		
		ptr=postfix_array[k++];
		//printf("ptr.id: %d\n",ptr.id);
		switch (ptr.id)
		{
			
			case S_FULL_IDENT:
				//printf("full ident key: %s\n",(char *)ptr.ptr);
			case S_SIMPLE_IDENT:
				item_tmp1=stack_htab_find_htab_item(I_Htable, ptr.ptr);
				
				if (item_tmp1==NULL)
				{
					stack_expression_destroy(S);
					free(S);
					fprintf(stderr,"Interpret: Item wasn't found.\n");
					new_token->id=-ERR_OTHERS;
					return new_token;
				}
				
				if (item_tmp1->func_or_var==2)
				{
					new=malloc(sizeof(token));
					int result=is_emb_fce(item_tmp1,S,new);
			
					if (result>0)
					{
						stack_expression_push(S,*new);
						break;
					}
					else if (result==-1)
					{
						return NULL;
					}
					htab_t *loc_table=htab_copy((htab_t *)item_tmp1->local_table);
				
					
					htab_item *parametr;
					int par_type;
					for (int i=item_tmp1->number_of_arguments;i>0;i--)
					{
						par_type=((int*)item_tmp1->data)[i-1];
						parametr=htab_find_item_by_argument_index(loc_table, i-1); 
						parametr->initialized=1;
						
						stack_expression_pop(S,&tmp2);
						
						
						if (par_type==S_DOUBLE)
						{
							double *par_value=malloc(sizeof(double));
							if (tmp2.id==TYPE_DOUBLE)
							{
								*par_value=(*((double*)tmp2.ptr));
								parametr->data=(double*)par_value;
								
							}
							else
							{
								*par_value=(*((int*)tmp2.ptr));
								parametr->data=(double*)par_value;
								
							}							
						}
						else if (par_type==S_INT)
						{
							int *par_value=malloc(sizeof(int));
							*par_value=(*((int*)tmp2.ptr));
							parametr->data=(int*)par_value;
						}
						else if (par_type==S_BOOLEAN)
							parametr->data_type=tmp2.id;
						else if (par_type==S_STRING)
						{
							char *new_val=malloc(sizeof(char)*strlen((char*)tmp2.ptr)+1);
							memcpy(new_val,(char*)tmp2.ptr,strlen((char*)tmp2.ptr)+1);
							parametr->data=(char*)new_val;
							
								
						}
						
				
					}

					
					stack_htab_push(I_Htable, loc_table);
					int result_of_interpret=0;
					new=malloc(sizeof(token));
					new->id=item_tmp1->data_type;
					if (item_tmp1->data_type==S_INT)
					{
						
						int *new_data=malloc(sizeof(int));
						new->ptr=new_data;
						I_Instr *tmp=L->Active;
						L->Active=item_tmp1->instruction_tape;
						result_of_interpret=inter(L, I_Htable,new,void_flag);
						L->Active=tmp;
					}
					else if (item_tmp1->data_type==S_DOUBLE)
					{
						
						double *new_data=malloc(sizeof(double));
						new->ptr=new_data;
						I_Instr *tmp=L->Active;
						L->Active=item_tmp1->instruction_tape;
						result_of_interpret=inter(L, I_Htable,new,void_flag);
						L->Active=tmp;
					}
					else if (item_tmp1->data_type==S_STRING)
					{
						I_Instr *tmp=L->Active;
						L->Active=item_tmp1->instruction_tape;
						result_of_interpret=inter(L, I_Htable,new,void_flag);
						
						L->Active=tmp;
					}
					else
					{
						I_Instr *tmp=L->Active;
						L->Active=item_tmp1->instruction_tape;
						result_of_interpret=inter(L, I_Htable,NULL,void_flag);
						L->Active=tmp;						
					}
					stack_htab_pop(I_Htable);
					stack_expression_push(S,*new);
					
					if (result_of_interpret!=0)
					{
						new->id=-result_of_interpret;
						return new;
					}
					
					new_token=new;
					
					break;
				}
				
				if (item_tmp1->initialized==0)
				{
					fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
					//freeALL();
					new_token->id=-ERR_UNINICIALIZED_VAR;
					return new_token;
				}
				if (item_tmp1->func_or_var==1)
				{
					new=malloc(sizeof(token));
					//printf("I in do express: %d\n",(*((int*)item_tmp1->data)));
					if (item_tmp1->data_type==S_STRING)
						new->id=TYPE_STRING;
					else if (item_tmp1->data_type==S_DOUBLE)
						new->id=TYPE_DOUBLE;
					else if (item_tmp1->data_type==S_INT)
						new->id=TYPE_INT;							
					else 
						new->id=item_tmp1->data_type;
					
					if (new->id!=S_TRUE || new->id!=S_FALSE) 
						new->ptr=item_tmp1->data;
						
					stack_expression_push(S,*new);
					
				}
				//free(new);
				break;

			case TYPE_DOUBLE:
			case TYPE_INT:
			case TYPE_STRING: 
			case S_TRUE:
			case S_FALSE:
				
				stack_expression_push(S,ptr);
				
				break;
			
			case S_EQUAL:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);
				new=inter_bool_op(tmp1,tmp2,1);
				stack_expression_push(S,*new);
				break;
			case S_NOT_EQUAL:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);

				new=inter_bool_op(tmp1,tmp2,2);
				stack_expression_push(S,*new);
				break;

			case S_GREATER_EQUAL:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);

				new=inter_bool_op(tmp1,tmp2,3);
				stack_expression_push(S,*new);
				break;

			case S_GREATER:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);

				new=inter_bool_op(tmp1,tmp2,4);
				stack_expression_push(S,*new);
				break;

			case S_LESS_EQUAL:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);

				new=inter_bool_op(tmp1,tmp2,5);
				stack_expression_push(S,*new);
				break;

			case S_LESS:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);
				new=inter_bool_op(tmp1,tmp2,6);
				stack_expression_push(S,*new);
				break;

			case S_AND:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);

				new=inter_bool_op(tmp1,tmp2,7);
				stack_expression_push(S,*new);
				break;

			case S_OR:
			
				stack_expression_pop(S,&tmp2);
				stack_expression_pop(S,&tmp1);

				new=inter_bool_op(tmp1,tmp2,8);
				stack_expression_push(S,*new);
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

				
				new=inter_arm_op(tmp1,tmp2,3);
				if (new->id==-8)
				{
					fprintf(stderr, "Divison by zero!.\n");
					//freeALL();
					new_token->id=-ERR_DIVISION_ZERO;
					return new_token ;
				}
				stack_expression_push(S,*new);
				break;


		}
	}
	
	if(stack_expression_pop(S,&tmp1))
	{	
		return NULL;
	}	
	new_token->id=tmp1.id;
	
	if (new_token->id==TYPE_INT)
	{
		int *new_val=malloc(sizeof(int));
		*new_val=*((int*)tmp1.ptr);
		new_token->ptr=(int *)new_val;
	}
	else if (new_token->id==TYPE_DOUBLE)
	{
		double *new_val=malloc(sizeof(double));
		*new_val=*((double*)tmp1.ptr);
		new_token->ptr=(double *)new_val;
	}
	else if (new_token->id==TYPE_STRING)
	{
		char *new_val=malloc(sizeof(char)*strlen((char*)tmp1.ptr)+1);
		memcpy(new_val,(char*)tmp1.ptr,strlen((char*)tmp1.ptr)+1);
		new_token->ptr=(char *)new_val;		
	}
	
	
	
	return new_token;
}

 
