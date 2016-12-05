#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>

#define I_STACKSIZE 20

int inter(Instr_List *L, stack_htab *I_Htable)
{
	struct stack_expresion *S=malloc(sizeof(struct stack_expresion));
	if (S==NULL)
		return ERR_INTERN_FAULT;
		
	stack_expression_init(S,I_STACKSIZE);
	htab_item * item_tmp1;
	htab_item * return_hitem;
	token *return_token;	
	int k=0;
	token **postfix_array;
	token *ptr, tmp1, tmp2, *new;	
	
	while (L->Active!=NULL)
    {
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
					switch (ptr->id)
					{
						case S_STRING:
						case S_INT:
						case S_DOUBLE:
							new=malloc(sizeof(token));
							if (new==NULL)
								return ERR_INTERN_FAULT;
								
							if (ptr->id==S_STRING)
								new->id=TYPE_STRING;
							else if (ptr->id==S_DOUBLE)
								new->id=TYPE_DOUBLE;
							else 
								new->id=TYPE_INT;
							
							
							item_tmp1=stack_htab_find_htab_item(I_Htable, ptr->ptr);
							if (item_tmp1->initialized==0)
							{
								fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
								//freeALL();
								return ERR_UNINICIALIZED_VAR ;					
							}
							new->ptr=item_tmp1->data;
							stack_expression_push(S,*new);
							free(new);
							break;
							
						case TYPE_DOUBLE:
						case TYPE_INT:
						case TYPE_STRING:
							stack_expression_push(S,*ptr);
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
				if (return_token->id==TYPE_INT && tmp1.id!=TYPE_INT)
				{
					return ERR_SEM_COMPATIBILITY;	
				}
				if(return_token->id==TYPE_DOUBLE && tmp1.id==TYPE_STRING)
				{
					return ERR_SEM_COMPATIBILITY;
				}
				break;
				
				if(return_token->id==TYPE_DOUBLE)
				{
					return_hitem->data=(double *)tmp1.ptr;
				}
				else if(return_token->id==TYPE_INT)
				{
					//free(return_hitem->data);
					return_hitem->data=(int *)tmp1.ptr;
					
				}
				else
				{
					if (tmp1.id==TYPE_INT)
					{
						char *str_data=IntToString((*((int *)tmp1.ptr)));
						free(tmp1.ptr);
						return_hitem->data=(char *)str_data;
					}
					else if (tmp1.id==TYPE_DOUBLE)
					{
						char *str_data=DoubleToString((*((int *)tmp1.ptr)));
						free(tmp1.ptr);
						return_hitem->data=(char *)str_data;
					}
					else
					{
						//free(return_hitem->data);
						return_hitem->data=(char *)tmp1.ptr;
					}
				}break;
				L->Active=L->Active->next_instr;
				
			
			case I_IF:
			case I_WHILE:
				return_token=(token *)L->Active->adr1;
				return_hitem=(htab_item *)return_token->ptr;
				k=0;
				postfix_array=(token **)L->Active->adr2;
				while(postfix_array[k]->id!=-2)
				{
					ptr=postfix_array[k++];
					switch (ptr->id)
					{
						case S_STRING:
						case S_INT:
						case S_DOUBLE:
							new=malloc(sizeof(token));
							if (new==NULL)
								return ERR_INTERN_FAULT;
								
							if (ptr->id==S_STRING)
								new->id=TYPE_STRING;
							else if (ptr->id==S_DOUBLE)
								new->id=TYPE_DOUBLE;
							else 
								new->id=TYPE_INT;
							
							
							item_tmp1=stack_htab_find_htab_item(I_Htable, ptr->ptr);
							if (item_tmp1->initialized==0)
							{
								fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
								//freeALL();
								return ERR_UNINICIALIZED_VAR ;					
							}
							new->ptr=item_tmp1->data;
							stack_expression_push(S,*new);
							free(new);
							break;
							
						case TYPE_DOUBLE:
						case TYPE_INT:
						case TYPE_STRING:
							stack_expression_push(S,*ptr);
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
					L->Active=L->Active->next_instr;
					int count=0;
					while(count>0)
					{
						if (L->Active==NULL)
							return ERR_OTHERS;
						if (L->Active->type_instr==I_END)
							count--;
						else 
							count++;
						L->Active=L->Active->next_instr;
					}					
				}
				
				
				
				
				
				
				
				
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
