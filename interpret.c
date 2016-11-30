#include <stdio.h>
#include "str.h"
#include "stable.h"
#include "ilist.h"

int inter(tListOfInstr *instrList,token **postfix_token_array,token *return_value);
{
	int i=0;
	token *ptr,tmp1,tmp2;
  listFirst(instrList);
  tInstr *I;
  while (1)
  {
    I = listGetData(instrList);
    ptr=(*postfix_token_array)[i++];
	
	
	
    switch (I->instType)
    {

	case I_ASSIGMENT:
		switch (ptr->id)
		{
			case S_PLUS:					// ----------- PLUS
				tmp2=ListPop(S);
				tmp1=ListPop(S);
				
				if (tmp1->data->initialized==0 || tmp2->data->initialized==0)
				{
					fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
					freeALL();
					return ERR_UNINICIALIZED_VAR ;					
				}

				
				if (return_value->id==TYPE_DOUBLE)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					
					tmp1->data->data=((double)tmp1->data->data)+((double)tmp2->data->data);
					tmp1->id=TYPE_DOUBLE;
					ListPush(S,tmp1);
					break;
				}
				if (return_value->id==TYPE_INT)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					tmp1->data->data=((int)tmp1->data->data)+((int)tmp2->data->data);
					tmp1->id=TYPE_INT;
					ListPush(S,tmp1);
					break;
				}	

				
				if (return_value->id==TYPE_STRING)            //     STRING CONCATENATE
				{
					void *ptr;
					if (tmp1->id == TYPE_DOUBLE)
					{
						ptr=tmp1->data->data;
						ptr->data->data=DoubleToString(tmp1->data->data);
						free(ptr);
					}
					else if (tmp1->id == TYPE_INT)
					{
						ptr=tmp1->data->data;
						ptr->data->data=IntToString(tmp1->data->data);
						free(ptr);
					}
					if (tmp2->id == TYPE_DOUBLE)
					{
						ptr=tmp2->data->data;
						ptr->data->data=DoubleToString(tmp2->data->data);
						free(ptr);
					}
					else if (tmp2->id == TYPE_INT)
					{
						ptr=tmp2->data->data;
						ptr->data->data=IntToString(tmp2->data->data);
						free(ptr);
					}					
					ptr=tmp1->data->data;
					tmp1->data->data=Conc_Str((char *)tmp1->data->data,(char * )tmp2->data->data);
					free(ptr);
					tmp1->id=TYPE_STRING;
					ListPush(S,tmp1);
					break;
				}						
								
				
				
			case S_MINUS:					//     ----------------------  MINUS
				tmp2=ListPop(S);
				tmp1=ListPop(S);

				if (tmp1->data->initialized==0 || tmp2->data->initialized==0)
				{
					fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
					freeALL();
					return ERR_UNINICIALIZED_VAR ;					
				}
				
				if (return_value->id==TYPE_DOUBLE)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					tmp1->data->data=((double)tmp1->data->data)-((double)tmp2->data->data);
					tmp1->id=TYPE_DOUBLE;
					ListPush(S,tmp1);
				}				
				else if (return_value->id==TYPE_INT)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					tmp1->data->data=((int)tmp1->data->data)-((int)tmp2->data->data);
					tmp1->id=TYPE_INT;
					ListPush(S,tmp1);
					break;
				}	

			case S_MUL:					//     ----------------------  Multiplication
				tmp2=ListPop(S);
				tmp1=ListPop(S);

				if (tmp1->data->initialized==0 || tmp2->data->initialized==0)
				{
					fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
					freeALL();
					return ERR_UNINICIALIZED_VAR ;					
				}				
				if (return_value->id==TYPE_DOUBLE)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					tmp1->data->data=((double)tmp1->data->data)*((double)tmp2->data->data);
					tmp1->id=TYPE_DOUBLE;
					ListPush(S,tmp1);
				}				
				else if (return_value->id==TYPE_INT)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					tmp1->data->data=((int)tmp1->data->data)*((int)tmp2->data->data);
					tmp1->id=TYPE_INT;
					ListPush(S,tmp1);
					break;
				}	

			case S_DIV:					//     ----------------------  Division 
				tmp2=ListPop(S);
				tmp1=ListPop(S);
				
				if (tmp1->data->initialized==0 || tmp2->data->initialized==0)
				{
					fprintf(stderr, "In line %d variable is not initialized.\n", LINE_NUM);
					freeALL();
					return ERR_UNINICIALIZED_VAR ;					
				}
				
				if (return_value->id==TYPE_DOUBLE)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT)
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					if (((double)tmp2->data->data)==0.0)
					{
						fprintf(stderr, "In line %d division by zero.\n", LINE_NUM);
						freeALL();
						return ERR_DIVISION_ZERO ;
					}
					tmp1->data->data=((double)tmp1->data->data)/((double)tmp2->data->data);
					tmp1->id=TYPE_DOUBLE;
					ListPush(S,tmp1);
				}				
				else if (return_value->id==TYPE_INT)
				{
					if (tmp1->id != TYPE_DOUBLE && tmp1->id != TYPE_INT || tmp2->id != TYPE_DOUBLE && tmp2->id != TYPE_INT )
					{
						fprintf(stderr, "In line %d variable is string. Expected double or integer.\n", LINE_NUM);
						freeALL();
						return ERR_SEM_COMPATIBILITY;
					}
					if (((double)tmp2->data->data)==0)
					{
						fprintf(stderr, "In line %d division by zero.\n", LINE_NUM);
						freeALL();
						return ERR_DIVISION_ZERO ;
					}
					tmp1->data->data=((int)tmp1->data->data)/((int)tmp2->data->data);
					tmp1->id=TYPE_INT;
					ListPush(S,tmp1);
					break;
				}
				case I_END:
					return_value->data->data=tmp1->data->data;
					break;
				
		}
    
	
	
	}
    listNext(instrList);

  }
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









