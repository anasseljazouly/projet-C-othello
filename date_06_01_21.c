#include <stdio.h>
#include <stdlib.h>
#define BLACK -1
#define WHITE  1
#define EMPTY 0
#define HIGHLIGHTER 10
                //0 1 2 3 4 5 6 7 
int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,1,-1,0,0,0},//3
                 {0,0,0,-1,1,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7

typedef struct 
{
    int x;
    int y;
}coord;
typedef struct azadzacontour
{
    int p_north;
    int p_north_west;
    int p_north_east;
    int p_south;
    int p_south_west;
    int p_south_east;
};

typedef enum{
        //<=                                 //=>
        north_west=1,  north=2,     north_east=3,
        west=0,         /*O*/          east=4,
        south_west=-1, south=-2 ,   south_east=-3
};



void highlight_if_possible(int type,int direction,int **board,int pos_l,int pos_c);
int still_in_board(int pos_l,int pos_c);
int contour (int **board,int pos_l,int pos_c,int player);
int search_player(int **board,int pos_l,int pos_c,int player,int player_origin,int dl,int dc);
void mark_position(int **board,int pos_l,int pos_c);

///////////////////////////////////////////////////////////////////
int search(int **board,coord point,int *directions);
void case_possible(int **board,int player);
void fetch_contour(int pos_x,int pos_y,int player,int *nombre);


int main()
{
    int i;

    
    return 0;
}



int contour (int **board,int pos_l,int pos_c,int player)
{   
    int direction;
    for(direction=-3;direction<5;direction++)
    {
        switch (direction)
        {
        case north://-1 0
           search_player(board,pos_l,pos_c,-player,player,-1,0);

            break;
        case north_east://-1 +1
            search_player(board,pos_l,pos_c,-player,player,-1,1);

            break;
        case north_west://-1 -1
            search_player(board,pos_l,pos_c,-player,player,-1,-1);

            break;
        case east:// 0 +1
            search_player(board,pos_l,pos_c,-player,player,0,1);

            break;
        case west:// 0 -1
            search_player(board,pos_l,pos_c,-player,player,0,-1);

            break;
        case south://+1 0
            search_player(board,pos_l,pos_c,-player,player,1,0);

            break;
        case south_east://+1 +1
            search_player(board,pos_l,pos_c,-player,player,1,1);

            break;
         case south_west://+1 -1 
            search_player(board,pos_l,pos_c,-player,player,1,-1);

            break;
        default:
            printf("invalid direction");
            break;
    }
    }
}

int still_in_board(int pos_l,int pos_c)//to check if we hit the boarder
{
    if (pos_l>=0 && pos_l<8 && pos_c>=0 && pos_l<8)
        return 1;
    return 0; //else
}

int search_player(int **board,int pos_l,int pos_c,int opponent,int player,int dl,int dc)
{
    if (opponent==1 || opponent==-1) 
    {
        if (still_in_board(pos_l+dl,pos_c+dc) && board[pos_l+dl][pos_c+dc]==opponent)
            {
                search_player(board,pos_l+dl,pos_c+dc,EMPTY,player,dl,dc);//cotour 0
            }
            }
    else if (opponent==0)//to search for a zero we stop when we find a zero when we aren't in the board anymore or when we found the player color
    {
        while(still_in_board(pos_l,pos_c) && board[pos_l][pos_c]==0 && board[pos_l][pos_c]!=player)
        {
            pos_l+=dl;pos_c+=dc;//next case 
        }
        if (board[pos_l][pos_c]==0)
            mark_position(board,pos_l,pos_c);
            return 1;
    }
    else 
        printf("non valid player");
        return 0;
}

void mark_position(int **board,int pos_l,int pos_c)
{
    board[pos_l][pos_c]=HIGHLIGHTER;//we can make a parallele board for the highliting and stack it on the original board 
                                    //and it may be also useful for turn playing 
}





