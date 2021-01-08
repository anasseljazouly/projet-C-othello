#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define largeur 600
#define hauteur 600



int tab[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,-1,0,1,-1,0,0,0},//3
                 {0,0,0,-1,1,1,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,1,0,0,0,0}};//7



void boardinterface(SDL_Window *window,SDL_Renderer *renderer,int tab[8][8]);




int main( int argc, char * argv[] )
{
    SDL_Window* window=NULL;SDL_Renderer *renderer=NULL;SDL_Texture *texture=NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,largeur,hauteur,0);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);

    //bord initial
    boardinterface(window,renderer,tab);


    SDL_bool programme= SDL_TRUE ;
    while(programme)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:

                    break;


                case SDL_QUIT :
                programme = SDL_FALSE; break;
                default: break;
            }

        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;

}


void boardinterface(SDL_Window *window,SDL_Renderer *renderer,int tab[8][8])
{
    //bordure
    SDL_SetRenderDrawColor(renderer,85,0,0,SDL_ALPHA_OPAQUE);

    SDL_Rect bordure;
    bordure.x=(largeur-430)/2;
    bordure.y = (largeur-430)/2;
    bordure.w = 430;
    bordure.h = 430;

    SDL_RenderFillRect(renderer,&bordure);

    //board
    SDL_SetRenderDrawColor(renderer,0,250,250,SDL_ALPHA_OPAQUE);
    SDL_Rect board;
    board.x = (largeur-400)/2;
    board.y = (hauteur-400)/2;
    board.w = 400;
    board.h = 400;
    SDL_RenderFillRect(renderer,&board);

    //lignes des boards
    int i;
    SDL_SetRenderDrawColor(renderer,127,127,127,SDL_ALPHA_OPAQUE);
    for(i=1;i<9;i++)
    {
        SDL_RenderDrawLine(renderer,100+i*50,100,100+i*50,500);
        SDL_RenderDrawLine(renderer,100,100+i*50,500,100+i*50);
    }

    //pions
    SDL_Rect pion;
    SDL_Surface *pionnoir=SDL_LoadBMP("noir.bmp");
    SDL_Surface *pionblanc=SDL_LoadBMP("blanc.bmp");
    int j,k;SDL_Texture *texture=NULL;
    for (j=0;j<8;j++)
    {
        for(k=0;k<8;k++)
        {
            int maxx,minx,maxy,miny;
            maxx=100+(j+1)*50;minx=100+(j)*50;maxy=100+(k+1)*50;miny=100+(k)*50;
            //pion
            pion.x=minx+2;
            pion.y=miny+2;
            pion.h=47;
            pion.w=47;
            //noir
            if(tab[k][j]==1)
            {
                texture=SDL_CreateTextureFromSurface(renderer,pionnoir);
                SDL_RenderCopy(renderer,texture,NULL,&pion);
                SDL_RenderPresent(renderer);
            }
            //blanc
            if(tab[k][j]==-1)
            {
                texture=SDL_CreateTextureFromSurface(renderer,pionblanc);
                SDL_RenderCopy(renderer,texture,NULL,&pion);
                SDL_RenderPresent(renderer);
            }

        }
    }
}
