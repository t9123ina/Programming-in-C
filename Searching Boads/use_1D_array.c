#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 3
#define HOLES N*N+1
#define NUMBOARDS 600000
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
	int parent_num;
	int child_num;
	char record_route;
} boardData;


void test(void);
void board_list(boardData *boards,char *final_solution);
void print_output(boardData *boards,int k);
bool find_solution(boardData *boards,int present,int back,char *final_solution);
void get_next(boardData *boards,int i,int count);
void change_order(char temp[HOLES],int index,int i);
bool check_same_order(boardData *boards,int i,char temp[HOLES]);
void insert_new(boardData *boards,char temp[HOLES],int i,int count);

int main(int argc, const char * agrv[])
{
	test();
	static boardData boards[NUMBOARDS];
	char final_solution[HOLES]= FINAL_SOLUTION;
	strncpy(boards[0].board_order,agrv[1],HOLES);
	boards[0].parent_num = -1;
	boards[0].child_num = 0;
	board_list(boards,final_solution);
	return 0;
}

void test(void)
{
	static boardData testboard[4];
	strncpy(testboard[0].board_order,"1234567 8",HOLES);
	assert(check_same_order(testboard,4,"12345678 ") == 0);
	assert(find_solution(testboard,0,4,"12345678 ") == 0);
}

void board_list(boardData *boards,char *final_solution)
{
	int count_eachboard=0,real_step=0;
	while(real_step <= NUMBOARDS)
	{
		get_next(boards,count_eachboard,real_step);
		real_step += boards[count_eachboard].child_num;
		if(find_solution(boards,count_eachboard,real_step,final_solution) == true)
		{
			printf("you can find solution!\n\n");
			print_output(boards,real_step);
			real_step = NUMBOARDS;/*break the while loop*/
		}
		count_eachboard++;
	}
	if(real_step > NUMBOARDS)
	{
		printf("Cannot find solution!");
	}
}

/*find the final solution through child boards*/
bool find_solution(boardData *boards,int present,int back,char *final_solution)
{
	int j,front=0;
	front = back - boards[present].child_num+1;
	for(j=front;j<=back; j++)
	{
		if(strcmp(boards[j].board_order,final_solution) == 0)
		{
			boards[j].child_num = -1; /*A mark for printing route*/
			return true;
		}
	}
	return false;
}

void print_output(boardData *boards,int k)
{
	int i,j,l,temp=0;
	/*Record the path*/
	for(j=k; j>=0; j--)
	{
		if(boards[j].child_num == -1)
		{
			temp = boards[j].parent_num;
			boards[j].record_route = 'Y';
		}
		if(j == temp)
		{
			temp = boards[j].parent_num;
			boards[j].record_route = 'Y';
		}
	}

	for(l=0; l<NUMBOARDS; l++)
	{
		if(boards[l].record_route == 'Y')
		{
			for(i=0; i<HOLES-1; ++i)
			{
				printf("%c",boards[l].board_order[i]);
				if(i % N == 2)
				{
					printf("\n");
				}
			}
			printf("\n");
		}
	}
}

/*Get the next board from the parent board*/
void get_next(boardData *boards,int i,int count)
{
	int j,index=0;
	char temp[HOLES];
	for(j=0; j<HOLES; j++) /*Calculate the blank's coordinate*/
	{
		if(boards[i].board_order[j] == ' ')
		{
			index = j;
		}
	}
	int col = index % N;
	int row = index / N;

	if (row != TOP_ROW)/*Change with up row and add to list*/
	{
		strncpy(temp,boards[i].board_order,HOLES);
		change_order(temp,index,index+UP);
		if(check_same_order(boards,i,temp) != true)
		{
			count++;
			insert_new(boards,temp,i,count);
		}
	}

	if (col != RIGHT_COL) /*Change with right column and add to list*/
	{
		strncpy(temp,boards[i].board_order,HOLES);
		change_order(temp,index,index+RIGHT);
		if(check_same_order(boards,i,temp) != true)
		{
			count++;
			insert_new(boards,temp,i,count);
		}
	}

	if (row != BOTTOM_ROW) /*Change with down row and add to list*/
	{
		strncpy(temp,boards[i].board_order,HOLES);
		change_order(temp,index,index+DOWN);
		if(check_same_order(boards,i,temp) != true)
		{
			count++;
			insert_new(boards,temp,i,count);
		}
	}

	if (col != LEFT_COL) /*Change with left column and add to list*/
	{
		strncpy(temp,boards[i].board_order,HOLES);
		change_order(temp,index,index+LEFT);
		if(check_same_order(boards,i,temp) != true)
		{
			count++;
			insert_new(boards,temp,i,count);
		}
	}
}

/*Change the orinigal order */
void change_order(char temp[HOLES],int index,int i)
{
	int k = temp[index];
	temp[index]=temp[i];
	temp[i]=k;
}

/*Check the order show up before or not*/
bool check_same_order(boardData *boards,int i,char temp[HOLES])
{
	int k;
	for(k=0; k<i; k++)
	{
		if(strcmp(boards[k].board_order,temp) == 0)
		{
			return true;
		}
	}
	return false;
}

/*Insert new boards to list, save the parent of the new board, and count this new board in the child num from the parent board*/
void insert_new(boardData *boards,char temp[HOLES],int i,int count)
{
	strncpy(boards[count].board_order,temp,HOLES);
	boards[count].parent_num = i;
	boards[i].child_num++;
}
