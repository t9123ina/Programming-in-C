#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mvm.h"

/* Initialize the m, and set its head and numkeys to 0 */
mvm* mvm_init(void)
{
	mvm* mvmhead;
   mvmhead = (mvm*) calloc(1,sizeof(mvm));
   if(mvmhead == NULL){ ON_ERROR("Creation of Set Failed\n"); }
   mvmhead->head = NULL;
   mvmhead->numkeys = 0;
   return mvmhead;
}

/* Number of key/value pairs stored */
int mvm_size(mvm* m)
{
	if(m==NULL){ return 0; }
	return m->numkeys;
}

/* Insert mvmcell (one key/value pair) into mvm */
void mvm_insert(mvm* m, char* key, char* data)
{
	/* use l to point a set and new mvmcell*/
	if(m!=NULL && key!=NULL && data!=NULL)
	{
	mvmcell *tempcell;
	tempcell = (mvmcell*) calloc(1, sizeof(mvmcell));
	/*for "/0"*/
	tempcell -> key = (char*) calloc(strlen(key)+1, sizeof(char));
	tempcell -> data = (char*) calloc(strlen(data)+1, sizeof(char));
	strcpy(tempcell -> key,key);
	strcpy(tempcell -> data,data);
	tempcell -> next = m -> head;
	m -> numkeys ++;
	m -> head = tempcell;
	}
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
	mvmcell *p=m->head;
	int length=1;
	char *result = (char*)calloc(length, sizeof(char));
	/* put key and data into one string */
	while(p!=NULL)
	{
		length += strlen(p->key)+strlen(p->data)+5;
		result = (char*)realloc(result, length);
		strcat(result,"[");
		strcat(result,p->key);
		strcat(result,"]");
		strcat(result,"(");
		strcat(result,p->data);
		strcat(result,")");
		strcat(result," ");
		p=p->next;
	}
	return result;
}

/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
	mvmcell *current,*previous;
	if(key == NULL || m == NULL){ return; }
	current=m->head;
	previous=NULL;
	while (current!= NULL && strcmp(current->key,key)!=0)
	{
        previous = current;
        current = current->next;
	}
	/* no value to delete */
    if (current==NULL){ return; }
    else{
		
		if (current == m->head){ /* delete for the first node */
        	m -> head = current->next;
			m->numkeys--;
			free(current->key);
			free(current->data);
        	free(current);
			current = NULL;
  		}else{	/* delete for other nodes */
        	previous->next = current->next;
			m->numkeys--;
			free(current->key);
			free(current->data);
        	free(current);
        	current = NULL;
  		}
	}
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
	mvmcell *p=m->head;
	while(p!=NULL){
		if(strcmp(p->key,key)==0){ return p->data; }
		p = p -> next;
	}
	return NULL;
}

/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
	int i=0,length=0;
	mvmcell *p=m->head;
	mvmcell *q=m->head;
	char **argv=NULL;
	while(p!=NULL) { /* count the size of *agrv[] */
	  if(strcmp(p->key,key)==0){ length++; }
	  p=p->next;
 	}
	argv = (char**)calloc(length, sizeof(char*));
	while(q!=NULL) /* put pointer into *agrv[] */
	{
	  if(strcmp(q->key,key)==0){
	    argv[i]=q->data;
	    i++;
		}
	 	q=q->next;
 	}
	*n = length;
	return argv;
}

/* Free & set p to NULL */
void mvm_free(mvm** p)
{
	mvm* a = *p;
	mvmcell* current=(*p)->head;
	mvmcell* temp;
	while(current!=NULL) /* free linked list */
  	{
    	temp = current->next;
		free(current->key);
		free(current->data);
    	free(current);
		current=temp;
  	}
	free(a);
	*p = NULL;
}
