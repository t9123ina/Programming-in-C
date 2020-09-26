#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mvm.h"

#define N 200

void processnumberofPh(mvm *map, char *agrv[]);
void findallPhon(mvm *map, char *agrv[]);
void create_map(FILE *fp,mvm* map);
void cut_map(int space,mvm* map,mvm* map_one,mvm* map_two);
void reversemap(mvm* map_two);
void print_out(char** allwords,int i);

int main(int argc,char * agrv[])
{
  FILE *fp = fopen("cmudict.txt","r");
  if(fp == NULL){ ON_ERROR("Can't open file\n");}
  mvm *map = mvm_init();
  create_map(fp,map);
  fclose(fp);
  if(argc==1){ ON_ERROR("Can't find words\n");}

  /*the number of phonemes*/
  if(strcmp(agrv[1],"-n")==0){ processnumberofPh(map,agrv); }
  /*for words to find all phonemes*/
  if(strcmp(agrv[1],"-n")!=0){ findallPhon(map,agrv); }
  return 0;
}

void processnumberofPh(mvm *map, char *agrv[]){
  int i,keynum=0,j=3;
  mvm *map_one = mvm_init(), *map_two = mvm_init();
  char* target=NULL;
  char** allwords=NULL;
  keynum = atoi(agrv[2]);
  cut_map(keynum,map,map_one,map_two);
  while(agrv[j]!=NULL)/*keep searching when there are words*/
  {
    target = mvm_search(map_one,agrv[j]);;
    printf("%s (%s): ",agrv[j],target);
    allwords = mvm_multisearch(map_two,target, &i);
    print_out(allwords,i);
    free(allwords);
    j++;
  }
  mvm_free(&map);
  mvm_free(&map_one);
  mvm_free(&map_two);
}

void findallPhon(mvm *map, char *agrv[]){
  int i,targetlen,cntsp=0;
  mvm *map_one = mvm_init(), *map_two = mvm_init();
  char* target=NULL;
  char** allwords=NULL;
  target = mvm_search(map,agrv[1]);
  targetlen=strlen(target)+1;
  for(i=0;i<targetlen;i++)/*count space for cut_map*/
  {
    if(target[i]==' '){ cntsp++;}
  }
  cut_map(cntsp,map,map_one,map_two);
  allwords = mvm_multisearch(map_two,target, &i);
  print_out(allwords,i);
  free(allwords);
  mvm_free(&map);
  mvm_free(&map_one);
  mvm_free(&map_two);
}

/*read file into a map*/
void create_map(FILE *fp,mvm* map)
{
  int count=0,b=0,n=0,i;
  char k;
  char word[N]={0},phonemes[N]={0};
  while((k = getc(fp)) != EOF)
	{
      if(k!='\n'&&k!='\r')
			{
        if(count==0 && k!='#')/*record word[N] before '#'*/
        {
          word[b]=k;
          b++;
        }
        if(k=='#')
        {
          word[b]='\0';
          count = 1;
        }
        if(count==1 && k!='#')/*record phonemes[N] after '#'*/
        {
          phonemes[n]=k;
          n++;
        }
			}
      if(k=='\n')/*put into map and initialize*/
			{
        phonemes[n]='\0';
        mvm_insert(map,word,phonemes);
        for(i=0;i<N;i++)
        {
          word[i]=0;
          phonemes[i]=0;
        }
        b=0;
        n=0;
        count=0;
			}
	}
}

/*cut oringinal map into map1 and map2*/
void cut_map(int space,mvm* map,mvm* map_one,mvm* map_two)
{
  int i,j,numph,k,m=0,final=0;
  int countlen[N]={0};
  char target[N]={0};
  mvmcell* p=map->head;
  while(p!=NULL)
  {
    numph = strlen(p->data);
    for(k=numph;k>=0;k--)/*record space's location*/
    {
      if(p->data[k]==' ')
      {
        countlen[m]=k;
        m++;
      }
    }
    /*phonemes more than target's phonemes*/
    if((m+1)>space)
    {
      final=numph-countlen[space]+1;
      strncpy(target,p->data+(countlen[space-1]+1),final);
      target[final]='\0';
      mvm_insert(map_one,p->key,target);
      mvm_insert(map_two,target,p->key);
    }
    /*phonemes equal target's phonemes*/
    if((m+1)==space)
    {
      mvm_insert(map_one,p->key,p->data);
      mvm_insert(map_two,p->data,p->key);
    }
    /*initialize*/
    for(i=0;i<N;i++)
    {
      countlen[i]=0;
    }
    for(j=0;j<N;j++)
    {
      target[j]=0;
    }
    m=0;
    p=p->next;
  }
  reversemap(map_two);
}

/*reverse map_two to the same order with text file*/
void reversemap(mvm* map_two){
  mvmcell *previous = NULL,*current,*preceding;
  current = map_two->head;
  preceding = map_two->head->next;
  while (preceding != NULL)
  {
    current->next = previous;
    previous = current;
    current = preceding;
    preceding = preceding->next;
  }
  current->next = previous;
  map_two->head = current;
}

/*print the words out*/
void print_out(char** allwords,int i)
{
  int j=0;
  for(j=0;j<i;j++)
  {
    printf("%s ",allwords[j]);
  }
  printf("\n");
}
