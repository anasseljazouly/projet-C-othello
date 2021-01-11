#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define largeur 600
#define hauteur 600






void putpion(SDL_Window *window,int board[8][8]);
void fondgame(SDL_Window *window,SDL_Renderer *renderer);
void initialisation_des_coordonnes(SDL_Rect coordonnes_case[8][8]);
void coordonne_click(int *ligne,int *colonne,SDL_Rect coordonnes_case[8][8],SDL_Event event);




int main( int argc, char * argv[] )
{
    int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,1,-1,0,0,0},//3
                 {0,0,0,-1,1,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,1,0,0,0,0}};//7
    SDL_Window* window=NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,largeur,hauteur,0);

    //coordonnes des cases:
    SDL_Rect coordonnes_case[8][8]={0};
    initialisation_des_coordonnes(coordonnes_case);

    //bord initial
    putpion(window,board);

    SDL_bool programme= SDL_TRUE ;
    while(programme)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    ;int colonne=-1;
                    int ligne=-1;
                    coordonne_click(&ligne,&colonne,coordonnes_case,event);
                    if (ligne!=-1 && colonne!=-1 && board[ligne][colonne]==0)
                    {
                    board[ligne][colonne]=-1;
                    putpion(window,board);
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

void putpion(SDL_Window *window,int board[8][8])
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    fondgame(window,renderer);

    //pions
    SDL_Rect rect_black,rect_white;
    SDL_Surface *pion_black=SDL_LoadBMP("black.bmp");
    SDL_Surface *pion_white=SDL_LoadBMP("white2.bmp");
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
