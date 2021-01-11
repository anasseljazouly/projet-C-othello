#include <stdio.h>
#include <stdlib.h>
#define BLACK 1
#define WHITE  -1
#define EMPTY_SPACE 0
#define HIGHLIGHTER 2
int TURN=BLACK;
                //0 1 2 3 4 5 6 7
int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,WHITE,BLACK,0,0,0},//3
                 {0,0,0,BLACK,WHITE,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7*/
int highlight_board[8][8]={0};
typedef enum{
        //<=                                 //=>
        north_west=1,  north=2,     north_east=3,
        west=0,         /*O*/          east=4,
        south_west=-1, south=-2 ,   south_east=-3
};


int still_in_board(int pos_l,int pos_c);//v
int contour (int pos_l,int pos_c,int player);//v
int search_player(int pos_l,int pos_c,int player,int player_origin,int dl,int dc);//v
void mark_position(int pos_l,int pos_c);//v
void display();//v
void display_h();//v
void set_color(int pos_l,int pos_c,int player);//v
int player_turn(int last_turn);//v
void who_wins();//v
void game();//v
void click_at(int player);//v
void reset_h();//v
void tell_turn(int player);//v

int main()
{
    game();



    return 0;
}
//--------------------------------------------------------------------------------------------------
int  contour (int pos_l,int pos_c,int player) //this is the function that highlight          //v
{
    if(board[pos_l][pos_c]!=player)
        return 0;
    int i=0;
    int direction;
    for(direction=-3;direction<5;direction++)
    {
        switch (direction)
        {
        case north://-1 0
           i+=search_player(pos_l,pos_c,-player,player,-1,0);
            break;
            
        case north_east://-1 +1
            i+=search_player(pos_l,pos_c,-player,player,-1,1);
            break;
            
        case north_west://-1 -1
            i+=search_player(pos_l,pos_c,-player,player,-1,-1);
            break;
            
        case east:// 0 +1
            i+=search_player(pos_l,pos_c,-player,player,0,1);
            break;
            
        case west:// 0 -1
            i+=search_player(pos_l,pos_c,-player,player,0,-1);
            break;
            
        case south://+1 0
            i+=search_player(pos_l,pos_c,-player,player,1,0);
            break;
            
        case south_east://+1 +1
            i+=search_player(pos_l,pos_c,-player,player,1,1);
            break;
            
         case south_west://+1 -1
            i+=search_player(pos_l,pos_c,-player,player,1,-1);
            break;
            
        default:
            printf("invalid direction");
            break;
    }
    }
    return i;
}

//--------------------------------------------------------------------------------------------------
void mark_position(int pos_l,int pos_c)         //v
{
    highlight_board[pos_l][pos_c]=HIGHLIGHTER;
}
//--------------------------------------------------------------------------------------------------
int still_in_board(int pos_l,int pos_c)//to check if we hit the boarder     //v
{
    if (pos_l>=0 && pos_l<8 && pos_c>=0 && pos_l<8)
        return 1;
    return 0; //else
}
//--------------------------------------------------------------------------------------------------
int search_player(int pos_l,int pos_c,int opponent,int player,int dl,int dc)      //for the highlight return the highlight    //v
{
    if (opponent==1 || opponent==-1)
    {
        if (still_in_board(pos_l+dl,pos_c+dc) && board[pos_l+dl][pos_c+dc]==opponent)
            {
                search_player(pos_l+dl,pos_c+dc,EMPTY_SPACE,player,dl,dc);//cotour 0
            }
            }
    else if (opponent==0)//to search for a zero we stop when we find a zero when we aren't in the board anymore or when we found the player color
    {
        while(still_in_board(pos_l,pos_c) && board[pos_l][pos_c]!=0 && board[pos_l][pos_c]!=player)
        {
            pos_l+=dl;pos_c+=dc;//next case
        }
        if (board[pos_l][pos_c]==0)
        {
            mark_position(pos_l,pos_c);
            return 1;
        }
        else
            return 0;
    }
    else{
        printf("non valid player");
        return 0;}
}
//--------------------------------------------------------------------------------------------------
void set_color(int pos_l,int pos_c,int player)//coloration  //v but we should optimize it more
{
    board[pos_l][pos_c]=player;
    int dl,dc,pos_l_b=pos_l,pos_c_b=pos_c;
    int check=0;
    for(dl=-1;dl<2;dl++)
    {
        for(dc=-1;dc<2;dc++)
        {
            if((dc==0 && dl==0) || !still_in_board(pos_l+dl,pos_c+dc))
            {
                continue;
            }

            while(board[pos_l+dl][pos_c+dc]==-player && still_in_board(pos_l+dl,pos_c+dc))
            {
                pos_l+=dl;pos_c+=dc;
                board[pos_l][pos_c]*=-1;
                check+=1;
            }


            if (!still_in_board(pos_l+dl,pos_c+dc) || ( check && board[pos_l+dl][pos_c+dc]==0 ))
            {

                for (;check>0;check--,pos_l-=dl,pos_c-=dc)
                {
                    board[pos_l][pos_c]*=-1;

                }

            }
            check=0;
            pos_l=pos_l_b;pos_c=pos_c_b;
        }

    }
}
//--------------------------------------------------------------------------------------------------
int player_turn(int last_turn)//give who's player turn based on the last_turn with highlighting //v
{
    int k=0,i,j,board_full=0;

    for (i=0;i<8;i++)
    {
        for(j=0;j<8;j++){
            k+=contour(i,j,last_turn);//this is the instruction that highlight
            board_full+=1;

        }
    }
    if (k!=0)
            {
                return -last_turn;//if there is a possible case
            }
    if (board_full!=64)
        return last_turn;//if there 's not a possible move if should bounce back the turn to the last player
    who_wins();//the game is over and we display the winner
    return 0;
}


//--------------------------------------------------------------------------------------------------
//click_at(player_turn(last_turn))
void click_at(int player)//it takes a player and demands to the user to enter a valid case where he want to move the board should be highlighted
{                        //and makes the move
    if (player==0)
        return ;
    unsigned int click_l,click_c,check=1;
    tell_turn(player);
    printf("where do you want to move tape : line , collone");
    display();
    display_h();
    do
    {

        scanf("%d%d",&click_l,&click_c);fflush(stdin);
        if (highlight_board[click_l][click_c]==HIGHLIGHTER)  // valide move
        {
            set_color(click_l,click_c,-player);
            check=0;
        }
        else
        {
            printf("unvalide move try again based on the highlight\n");
        }
    }while(check);
    reset_h();
}
//--------------------------------------------------------------------------------------------------

void game()
{
    int last_turn=WHITE;
    printf("\n////////////////////////////////////////////////////////////\n/////////////////////WELCOME TO OTHELLO/////////////////////\n");
    do
    {
        last_turn=player_turn(last_turn);
        click_at(last_turn);
    }while(last_turn);

}





//Choose a Color :\n   1:   for Black \n and \n-1:   for White \n





//--------------------------------------------------------------------------------------------------
void who_wins()//display who wins in the end
{
    int i,j,black_num=0,white_num=0;
    printf("GAME OVER");
    for (i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            if (board[i][j]==BLACK)
                black_num++;
            else
                white_num++;
        }
    }
    if (black_num<white_num)
        printf("WHITE WINS");
    else if (black_num>white_num)
        printf("BLACK WINS");
    else
        printf("DRAW");
}
//--------------------------------------------------------------------------------------------------
void display()                  //v
{
    int i,j;
    printf("\ngame board\n");
    for (i=0;i<8;i++)
    {
        for (j=0;j<8;j++)
        {
            printf("  %d  ",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
//--------------------------------------------------------------------------------------------------
void display_h()                  //v
{
    int i,j;
    printf("\n");
    printf("highlight board\n");
    for (i=0;i<8;i++)
    {
        for (j=0;j<8;j++)
        {
            printf("  %d  ",highlight_board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void reset_h()
{
    int i,j;

    for (i=0;i<8;i++)
    {
        for (j=0;j<8;j++)
        {
            highlight_board[i][j]=0;
        }
        printf("\n");
    }
}

void tell_turn(int player)
{
    if (player==BLACK)
        printf("IT'S BLACK'S TURN\n");
        return ;
    else
        printf("IT'S WHITE'S TURN\n");
        return ;
}