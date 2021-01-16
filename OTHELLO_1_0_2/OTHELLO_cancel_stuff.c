#include<stdio.h>
#include<stdlib.h>
#define Malloc(type) (type *)malloc(sizeof(type))
int board[8][8]={0};

typedef struct _board_miroir
{
    int board_m[8][8];
    struct _board_miroir *next;
    struct _board_miroir *previous;
}board_miroir;

//structure explanation
/*        NULL<=|p-B-n|<=>|p-B-n|<=>....<=>|p-B-n|=>NULL          */
//we should just try out the algorithm and validate it 
board_miroir *cancel_boards=NULL;


void update_cancel_board(board_miroir **ptr_cancel_boards)      //we should use it in every round
{
    int i,j;
    board_miroir *new_board=Malloc(board_miroir);
    new_board->next=NULL;
    new_board->previous=(*ptr_cancel_boards);
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            new_board->board_m[i][j]=board[i][j];

        }
    }
    (*ptr_cancel_boards)->next=new_board;
    (*ptr_cancel_boards)=new_board;
    free(new_board);

}

void free_ptr(board_miroir *cancel_boards)          //if the player played while he got back 
{
    if (cancel_boards->next==NULL)
        return;
    else
        free_ptr(cancel_boards->next);
    free(cancel_boards);
}

void move_backward(int autorized)
{
    int i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            board[i][j]=cancel_boards->board_m[i][j];
        }
    }
    if(cancel_boards->next)
        cancel_boards=cancel_boards->next;
}

void move_forward(int autorized)
{
    int i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            board[i][j]=cancel_boards->board_m[i][j];
        }
    }
    
    if(cancel_boards->previous)
        cancel_boards=cancel_boards->previous;
}

void display()                //just to visualize things
{
    int i,j;
    printf("\ngame board\n");
    for (i=0;i<8;i++)
    {
        for (j=0;j<8;j++)
        {
            printf("   %d   ",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main()
{
    
}

