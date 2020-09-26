#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 3
#define HOLES N*N+1
#define FINAL_SOLUTION "12345678 "
#define TOP_ROW 0
#define BOTTOM_ROW 2
#define LEFT_COL 0
#define RIGHT_COL 2
#define LEFT -1
#define RIGHT 1
#define UP -3
#define DOWN 3

typedef enum { false, true }bool; 

typedef struct boardData
{
	char board_order[HOLES];
	char route;
	struct boardData *parent;
	struct boardData *next;
} boardData;


void test(void);
void board_list(boardData *boards,char *final_solution);
void print_output(boardData *boards);
void get_next(boardData *boards);
void change_order(char *temp,int index,int i);
bool check_same_order(boardData *boards,char *temp);
void insert_new(boardData *boards,char *temp);

int main(int argc, const char * agrv[])
{
	boardData *boards;
	boards = (boardData *)malloc(sizeof(boardData));
	char final_solution[]= FINAL_SOLUTION;
	strncpy(boards->board_order,agrv[1],HOLES);
	boards->parent = NULL;
	boards->next = NULL;
	board_list(boards,final_solution);
	free(boards);
	return 0;
}

void board_list(boardData *boards,char *final_solution)
{
	while(boards != NULL)
	{
		get_next(boards);
		if(strcmp(boards->board_order,final_solution) == 0)
		{
				printf("you can find solution!\n\n");
				print_output(boards);
				boards -> next = NULL;/*break the while loop*/
		}
		boards = boards -> next;
	}
}


void print_output(boardData *boards)
{
	int i;
	boardData *p=boards,
						*previous = NULL,
            *current = p,
            *preceding = p->parent;

/*Reverse its path*/
    while (preceding != NULL)
		{
        current->parent = previous;
        previous = current;
        current = preceding;
        preceding = preceding->parent;
    }
    current->parent = previous;
    p = current;

/*Print its path*/
	while(p!=NULL)
	{
		for(i=0; i<HOLES-1; ++i)
		{
			printf("%c",p->board_order[i]);
			if(i % N == 2)
			{
				printf("\n");
			}
		}
		printf("\n");
		p=p->parent;
	}
}

void get_next(boardData *boards)
{
	int j,index=0;
	char temp[HOLES];
	for(j=0; j<HOLES; j++) /*Calculate the blank's coordinate*/
	{
		if(boards->board_order[j] == ' ')
		{
			index = j;
		}
	}
	int col = index % N;
	int row = index / N;

	if (row != TOP_ROW)/*Change with up row and add to list*/
	{
		strncpy(temp,boards->board_order,HOLES);
		change_order(temp,index,index+UP);
		if(check_same_order(boards,temp) != true)
		{
			insert_new(boards,temp);
		}
	}

	if (col != RIGHT_COL) /*Change with right column and add to list*/
	{
		strncpy(temp,boards->board_order,HOLES);
		change_order(temp,index,index+RIGHT);
		if(check_same_order(boards,temp) != true)
		{
			insert_new(boards,temp);
		}
	}

	if (row != BOTTOM_ROW) /*Change with down row and add to list*/
	{
		strncpy(temp,boards->board_order,HOLES);
		change_order(temp,index,index+DOWN);
		if(check_same_order(boards,temp) != true)
		{
			insert_new(boards,temp);
		}
	}

	if (col != LEFT_COL) /*Change with left column and add to list*/
	{
		strncpy(temp,boards->board_order,HOLES);
		change_order(temp,index,index+LEFT);
		if(check_same_order(boards,temp) != true)
		{
			insert_new(boards,temp);
		}
	}
}

/*Change the orinigal order */
void change_order(char *temp,int index,int i)
{
	int k = temp[index];
	temp[index]=temp[i];
	temp[i]=k;
}

/*Check the order show up before or not*/
bool check_same_order(boardData *boards,char *temp)
{
	boardData *p = boards;
	p = p -> next;
	while(p != NULL)
	{
		if(strcmp(p->board_order,temp) == 0)
		{
			return true;
		}
		p = p -> next;
	}
	return false;
}

/*Insert new boards to list*/
void insert_new(boardData *boards,char *temp)
{
	boardData *NewNode, *p = boards;
	NewNode = (boardData *)malloc(sizeof(boardData));
	strncpy(NewNode->board_order,temp,HOLES);
	NewNode->parent=boards;
	NewNode->next=NULL;
	while(p -> next != NULL)
	{
		p = p-> next;
	}
	p->next=NewNode;
}
