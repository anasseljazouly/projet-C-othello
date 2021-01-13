#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define largeur 600
#define hauteur 600
#define BLACK 1
#define WHITE  -1
#define EMPTY_SPACE 0
#define HIGHLIGHTER 2

int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,WHITE,BLACK,0,0,0},//3
                 {0,0,0,BLACK,WHITE,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7*/

int highlight_board[8][8]={0};

enum{
        //<=                                 //=>
        north_west=1,  north=2,     north_east=3,
        west=0,         /*O*/          east=4,
        south_west=-1, south=-2 ,   south_east=-3
};

void putpion(SDL_Window *window);
void fondgame(SDL_Window *window,SDL_Renderer *renderer);
void initialisation_des_coordonnes(SDL_Rect coordonnes_case[8][8]);
void coordonne_click(int *ligne,int *colonne,SDL_Rect coordonnes_case[8][8],SDL_Event event);

/*****************************************************c****************************/
int player_turn(int last_turn);//v
int contour (int pos_l,int pos_c,int player);//v
void click_at(int player,int click_l,int click_c);//v
void tell_turn(int player);//v
void reset_h();//v
void set_color(int pos_l,int pos_c,int player);//v
int still_in_board(int pos_l,int pos_c);//v
int search_player(int pos_l,int pos_c,int player,int player_origin,int dl,int dc);//v
void mark_position(int pos_l,int pos_c);//v
void display();//v
void display_h();//v
void who_wins();

int main( int argc, char * argv[] )
{

    SDL_Window* window=NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,largeur,hauteur,0);

    //coordonnes des cases:
    SDL_Rect coordonnes_case[8][8]={0};
    initialisation_des_coordonnes(coordonnes_case);

    //bord initial
    putpion(window);

    //first player
    int last_turn=BLACK;
    last_turn=player_turn(last_turn);
    int click_l=-1;
    int click_c=-1;
    //board+indices
    display();
    display_h();
    putpion(window);
    SDL_bool programme= SDL_TRUE ;
    while(programme)
    {


        SDL_Event event;
        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                        coordonne_click(&click_l,&click_c,coordonnes_case,event);
                       if (highlight_board[click_l][click_c]==HIGHLIGHTER)  // valide move
                        {
                        set_color(click_l,click_c,-last_turn);
                        reset_h();
                        last_turn=player_turn(last_turn);
                        putpion(window);
                        display();
                        display_h();
                        }
                        else
                        {
                            printf("unvalide move try again based on the highlight\n");
                        }
                            printf("%d turn",last_turn);
                        if(last_turn==0)
                        {
                            who_wins();
                            SDL_Delay(3000);
                            programme = SDL_FALSE;
                        }
                    break;

                case SDL_QUIT :
                programme = SDL_FALSE; break;
                default: break;
            }


        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}


void fondgame(SDL_Window *window,SDL_Renderer *renderer)
{
    //fond
    SDL_Rect fond_rect;
    fond_rect.x=0;
    fond_rect.y=0;
    fond_rect.w=largeur;
    fond_rect.h=hauteur;


    SDL_Surface *fond=SDL_LoadBMP("fond.bmp");
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,fond);
    SDL_FreeSurface(fond);
    SDL_RenderCopy(renderer,texture,NULL,&fond_rect);

    //fond du board
    SDL_Rect board_rect;
    board_rect.x = (largeur-400)/2;
    board_rect.y = (hauteur-400)/2;
    board_rect.w = 400;
    board_rect.h = 400;

    SDL_Surface *fond_board =SDL_LoadBMP("fondboard.bmp");
    texture=SDL_CreateTextureFromSurface(renderer,fond_board);
    SDL_FreeSurface(fond_board);
    SDL_RenderCopy(renderer,texture,NULL,&board_rect);

    //lignes
     int i;
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    for(i=1;i<9;i++)
    {
        SDL_RenderDrawLine(renderer,100+i*50,100,100+i*50,500);
        SDL_RenderDrawLine(renderer,100,100+i*50,500,100+i*50);
    }
}

void putpion(SDL_Window *window)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    fondgame(window,renderer);

    //pions
    SDL_Rect rect_black,rect_white,rect_hint;
    SDL_Surface *pion_black=SDL_LoadBMP("black.bmp");
    SDL_Surface *pion_white=SDL_LoadBMP("white2.bmp");
    SDL_Surface *hint=SDL_LoadBMP("hight2.bmp");
    int j,k;SDL_Texture *texture=NULL;
    for (j=0;j<8;j++)
    {
        for(k=0;k<8;k++)
        {
            int maxx,minx,maxy,miny;
            maxx=100+(j+1)*50;minx=100+(j)*50;maxy=100+(k+1)*50;miny=100+(k)*50;
            //pion
            rect_white.x=minx-5;
            rect_white.y=miny-5;
            rect_white.h=58;
            rect_white.w=58;

            rect_black.x=minx-1;
            rect_black.y=miny-1;
            rect_black.h=52;
            rect_black.w=52;

            //hint
            rect_hint.x=minx+19;
            rect_hint.y=miny+21;
            rect_hint.h=11;
            rect_hint.w=11;

            //noir
            if(board[k][j]==1)
            {
                texture=SDL_CreateTextureFromSurface(renderer,pion_black);
                SDL_RenderCopy(renderer,texture,NULL,&rect_black);
                SDL_RenderPresent(renderer);
            }
            //blanc
            if(board[k][j]==-1)
            {
                texture=SDL_CreateTextureFromSurface(renderer,pion_white);
                SDL_RenderCopy(renderer,texture,NULL,&rect_white);
                SDL_RenderPresent(renderer);
            }
            //highlight
            if(highlight_board[k][j]==2)
            {
                texture=SDL_CreateTextureFromSurface(renderer,hint);
                SDL_RenderCopy(renderer,texture,NULL,&rect_hint);
                SDL_RenderPresent(renderer);
            }

        }
    }
    SDL_FreeSurface(pion_black);
    SDL_FreeSurface(pion_white);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
}


void initialisation_des_coordonnes(SDL_Rect coordonnes_case[8][8])
{
    int j,k,i=0;
    for (j=0;j<8;j++)
    {
        for(k=0;k<8;k++)
        {
            coordonnes_case[j][k].y=100+j*50;
            coordonnes_case[j][k].x=100+k*50;
            coordonnes_case[j][k].h=100+(j+1)*50;
            coordonnes_case[j][k].w=100+(k+1)*50;
        }
    }
}

void coordonne_click(int *ligne,int *colonne,SDL_Rect coordonnes_case[8][8],SDL_Event event)
{
    int j,k;
    for (j=0;j<8;j++)
    {
        for(k=0;k<8;k++)
        {
            if(event.button.x>coordonnes_case[j][k].x && event.button.x<coordonnes_case[j][k].w &&
               event.button.y>coordonnes_case[j][k].y && event.button.y<coordonnes_case[j][k].h )
            {

                    *ligne=j;*colonne=k;
            }
        }
    }
}

/***********************************************C****************************************/
int player_turn(int last_turn)//give who's player turn based on the last_turn with highlighting //v
{
    int k=0,i,j,highlight_full=0,board_vide=0;

    for (i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            k+=contour(i,j,last_turn);//this is the instruction that highlight
            if(board[i][j]==0)
            {
                board_vide++;
            }
        }
    }
    for (i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            if(highlight_board[i][j]==2)
            {
                highlight_full++;
            }
        }
    }
    printf("%d aa",highlight_full);
    if(board_vide==0)
    {
        return 0;
    }
    if (highlight_full!=0)
    {
        return -last_turn;//if there is a possible case
    }
    if(highlight_full==0)
    {
        player_turn(-last_turn);
    }
}


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
        if (board[pos_l][pos_c]==0 && still_in_board(pos_l,pos_c))
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

void click_at(int player,int click_l,int click_c)//it takes a player and demands to the user to enter a valid case where he want to move the board should be highlighted
{                        //and makes the move
    if (player==0)
      {
          return ;
      }
    unsigned int check=1;
    tell_turn(player);
    display();
    display_h();
    do
    {
        if (highlight_board[click_l][click_c]==HIGHLIGHTER)  // valide move
        {
            set_color(click_l,click_c,-player);
            check=0;
        }
        else
        {
            printf("unvalide move try again based on the highlight\n");
        }
    reset_h();
    }while(check);

}

void tell_turn(int player)
{
    if (player==BLACK)
    {
        printf("IT'S BLACK'S TURN\n");
        return ;
    }
    else
    {
        printf("IT'S WHITE'S TURN\n");
        return ;
    }
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
                check+=1;
            }


            if (!still_in_board(pos_l+dl,pos_c+dc) || !( check && board[pos_l+dl][pos_c+dc]==0 ))
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

int still_in_board(int pos_l,int pos_c)//to check if we hit the boarder     //v
{
    if (pos_l>=0 && pos_l<8 && pos_c>=0 && pos_c<8)
        return 1;
    return 0; //else
}

void mark_position(int pos_l,int pos_c)         //v
{
    highlight_board[pos_l][pos_c]=HIGHLIGHTER;
}

void display()                  //v
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
            printf("   %d   ",highlight_board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void who_wins()//display who wins in the end
{
    int i,j,black_num=0,white_num=0;
    printf("GAME OVER\n");
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
        printf("WHITE WINS\n");
    else if (black_num>white_num)
        printf("BLACK WINS\n");
    else
        printf("DRAW\n");
    printf("\nWHITE :%d\nBLACK :%d\n",white_num,black_num);
}
