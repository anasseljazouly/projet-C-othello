#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unistd.h>
#include <sys/stat.h>

#define largeur 600
#define hauteur 600
#define BLACK 1
#define WHITE  -1
#define EMPTY_SPACE 0
#define HIGHLIGHTER 2
#define Malloc(type) (type*)malloc(sizeof(type))


int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,BLACK,WHITE,0,0,0},//3
                 {0,0,0,WHITE,BLACK,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7*/

int highlight_board[8][8]={0};

typedef struct _board_list
{
    int _last_turn;
    struct _board_list *previous;
    int board_m[8][8];
    struct _board_list *next;

}board_list;

enum{
        //<=                                 //=>
        north_west=1,  north=2,     north_east=3,
        west=0,         /*O*/          east=4,
        south_west=-1, south=-2 ,   south_east=-3
};

int key=10;

void first_page(SDL_Window *window);
void menu(SDL_Window *window,SDL_Event event,int *num_menu);
void putpion(SDL_Window *window);
void putpion_with_hint(SDL_Window *window);
void fondgame(SDL_Renderer *renderer);
int if_click_highlight(SDL_Event event);
void initialisation_des_coordonnes(SDL_Rect coordonnes_case[8][8]);
void coordonne_click(int *ligne,int *colonne,SDL_Rect coordonnes_case[8][8],SDL_Event event);
int if_click_replay(SDL_Event event);
void restart_game();
int if_click_back(SDL_Event event);
int if_click_next(SDL_Event event);
int if_click_home(SDL_Event event);
void save_the_game(SDL_Window *window,int hint_etat,int *programme,int *stay_in_menu,int type);
void show(char * text,SDL_Renderer *renderer);
SDL_Renderer* fond_first_page(SDL_Window *window);
void fond_username(SDL_Renderer *renderer);
void fond_password(SDL_Renderer *renderer);
void fond_errorusername(SDL_Renderer *renderer);
void fond_usernamenotfound(SDL_Renderer *renderer);
void fond_errorpassword(SDL_Renderer *renderer);

/*****************************************************c****************************/
int player_turn(int last_turn,int *pass_turn);//v
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
void update_list_board(board_list **ptr_list_boards,int last_turn);
void free_ptr(board_list **ptr_list_boards);
void move_backward(board_list **ptr_list_boards,int last_turn);
void move_forward(board_list **ptr_list_boards,int last_turn);
int sign_in(SDL_Renderer *renderer);
int log_in(SDL_Renderer *renderer);
int decode (char *str,int key);
int code (char *str,int key);
int creat_folder(char *dir_name);
int isDirectoryExists(const char *path);

int main( int argc, char * argv[] )
{

    SDL_Window* window=NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,largeur,hauteur,SDL_WINDOW_OPENGL);
    SDL_Event event;
    int stay_in_menu=1;
    int save=0,pass_turn=0;
    int HOME=1;int QUIT=0;
    TTF_Init();

    first_page(window);

    while (stay_in_menu)
    {
    //menu
    int num_menu=0;
    menu(window,event,&num_menu);
    printf("menu %d\n",num_menu);
    //coordonnes des cases:
    SDL_Rect coordonnes_case[8][8]={0};
    initialisation_des_coordonnes(coordonnes_case);

    //click restart
    int onrestart=-1;//out of restart

    //indice etat :
    int hint_etat=-1;//off

    //list of boards
    board_list *list_boards;
    list_boards=Malloc(board_list);
    board_list *to_free=list_boards;

    //first player
    int last_turn=BLACK;
    printf("%d",last_turn);
    list_boards->_last_turn=last_turn;
    list_boards->previous=NULL;
    list_boards->next=NULL;
    memcpy(list_boards->board_m,board,sizeof(int)*8*8);

    last_turn=player_turn(last_turn,&pass_turn);
    int click_l=-1;
    int click_c=-1;

    /*//board+indices
    display();
    display_h();
    putpion(window);*/

    int programme= 1;
    switch(num_menu)
    {

    case 2:
    //board+indices
    display();
    display_h();
    putpion(window);
    while(programme)
    {

        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:

                        coordonne_click(&click_l,&click_c,coordonnes_case,event);

                        if(if_click_home(event))
                        {
                            save_the_game(window,hint_etat,&programme,&stay_in_menu,HOME);
                        }

                        if(if_click_back(event))
                        {
                            move_backward(&list_boards,last_turn);
                            reset_h();
                            last_turn=list_boards->_last_turn;
                            last_turn=player_turn(last_turn,&pass_turn);
                            display();
                            display_h();
                            if(hint_etat==1)
                            {putpion_with_hint(window);}
                            else{putpion(window);}
                            break;
                        }

                        if(if_click_next(event))
                        {
                           move_forward(&list_boards,last_turn);
                            reset_h();
                            last_turn=list_boards->_last_turn;
                            last_turn=player_turn(last_turn,&pass_turn);
                            display();
                            display_h();
                            if(hint_etat==1)
                            {putpion_with_hint(window);}
                            else{putpion(window);}
                            break;
                        }

                        if(if_click_highlight(event))
                        {
                            hint_etat=-hint_etat;
                            if(hint_etat==1)
                            {putpion_with_hint(window);}
                            else{putpion(window);}
                        }

                       if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==-1)  // valide move+hint off
                        {
                            free_ptr(&list_boards);
                            set_color(click_l,click_c,-last_turn);
                            update_list_board(&list_boards,last_turn);
                            reset_h();
                            printf("%d",last_turn);
                            last_turn=player_turn(last_turn,&pass_turn);
                            putpion(window);
                            display();
                            display_h();
                        }

                        if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==1)  // valide move+hint on
                        {
                            free_ptr(&list_boards);
                            set_color(click_l,click_c,-last_turn);
                            update_list_board(&list_boards,last_turn);
                            int i,j;
                            reset_h();
                            printf("%d",last_turn);
                            last_turn=player_turn(last_turn,&pass_turn);
                            putpion_with_hint(window);
                            display();
                            display_h();
                        }

                        if(last_turn==0)
                        {
                            who_wins();

                            programme = 0;
                            restart_game();
                        }

                        if(if_click_restart(event))
                        {
                            restart_game();
                            last_turn=BLACK;
                            printf("%d",last_turn);
                            //free(to_free);
                            list_boards=Malloc(board_list);
                            list_boards->_last_turn=last_turn;
                            list_boards->previous=NULL;
                            list_boards->next=NULL;
                            memcpy(list_boards->board_m,board,sizeof(int)*8*8);
                            last_turn=player_turn(last_turn,&pass_turn);
                            click_l=-1;
                            click_c=-1;
                            hint_etat=-1;
                            putpion(window);
                        }

                    break;

                case SDL_QUIT :
                    save_the_game(window,hint_etat,&programme,&stay_in_menu,QUIT);
                 break;
                default: break;
            }
        }
    }
    free(to_free);break;
    default:break;
    }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

SDL_Renderer* fond_first_page(SDL_Window *window)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);

    SDL_Rect first_page_rect;
    first_page_rect.x=0;
    first_page_rect.y=0;
    first_page_rect.w=largeur;
    first_page_rect.h=hauteur;

    SDL_Surface *first_page=SDL_LoadBMP("pictures/first_page.bmp");
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,first_page);
    SDL_FreeSurface(first_page);
    SDL_RenderCopy(renderer,texture,NULL,&first_page_rect);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
    return renderer;
}

void first_page(SDL_Window *window)
{

    int running=1;SDL_Renderer *renderer;int return_to_first_page=1;
    SDL_Event event;
    while(return_to_first_page)
    {
        renderer=fond_first_page(window);printf("walo");
        fond_first_page(window);
        running=1;
        while(running)
        {
            while(SDL_PollEvent(&event))
            {

                switch(event.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        if(event.button.x>200 && event.button.x<400 && event.button.y>220 && event.button.y<295)//sign_in
                        {
                            if (sign_in(renderer)==1)
                            {
                                SDL_DestroyRenderer(renderer);return ;
                            }
                            else{running=0;SDL_DestroyRenderer(renderer);}
                        }
                        if(event.button.x>200 && event.button.x<400 && event.button.y>370 && event.button.y<445)//log_in
                        {
                            if(log_in(renderer)==1)
                            {
                                SDL_DestroyRenderer(renderer);return;
                            }
                            else{running=0;SDL_DestroyRenderer(renderer);}
                        }
                        break;
                    case SDL_QUIT:
                        exit(1);
                    default:break;
                }
            }
        }
    }
}


void menu(SDL_Window *window,SDL_Event event,int *num_menu)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);

    SDL_Rect menu_rect;
    menu_rect.x=0;
    menu_rect.y=0;
    menu_rect.w=largeur;
    menu_rect.h=hauteur;

    SDL_Surface *menu=SDL_LoadBMP("pictures/menu.bmp");
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,menu);
    SDL_FreeSurface(menu);
    SDL_RenderCopy(renderer,texture,NULL,&menu_rect);

    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);

    int choice=0;

    while (!choice)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                //1 player
                if(event.button.x>50 && event.button.x<250 && event.button.y>120 && event.button.y<195)
                {
                    *num_menu=1;choice=1;
                }
                //2 players
                if(event.button.x>350 && event.button.x<550 && event.button.y>120 && event.button.y<195)
                {
                    *num_menu=2;choice=1;
                }
                //reload
                if(event.button.x>200 && event.button.x<400 && event.button.y>245 && event.button.y<320)
                {
                    *num_menu=3;choice=1;
                }
                //best score
                if(event.button.x>200 && event.button.x<400 && event.button.y>370 && event.button.y<445)
                {
                    *num_menu=4;choice=1;
                }
                //rules
                if(event.button.x>200 && event.button.x<400 && event.button.y>495 && event.button.y<570)
                {
                    *num_menu=5;choice=1;
                }
            }
            if(event.type==SDL_QUIT)
            {
                *num_menu=0;choice=1;SDL_Quit();
            }
        }
    }
    SDL_DestroyRenderer(renderer);
}


void fondgame(SDL_Renderer *renderer)
{
    //fond
    SDL_Rect fond_rect;
    fond_rect.x=0;
    fond_rect.y=0;
    fond_rect.w=largeur;
    fond_rect.h=hauteur;


    SDL_Surface *fond=SDL_LoadBMP("pictures/fond.bmp");
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,fond);
    SDL_FreeSurface(fond);
    SDL_RenderCopy(renderer,texture,NULL,&fond_rect);

    //fond du board
    SDL_Rect board_rect;
    board_rect.x = (largeur-400)/2;
    board_rect.y = (hauteur-400)/2;
    board_rect.w = 400;
    board_rect.h = 400;

    SDL_Surface *fond_board =SDL_LoadBMP("pictures/fondboard.bmp");
    texture=SDL_CreateTextureFromSurface(renderer,fond_board);
    SDL_FreeSurface(fond_board);
    SDL_RenderCopy(renderer,texture,NULL,&board_rect);

    //click replay
    SDL_Rect rect_click_restart;
    rect_click_restart.x=200;
    rect_click_restart.y=525;
    rect_click_restart.w=200;
    rect_click_restart.h=50;

    SDL_Surface *click_restart;
        click_restart=SDL_LoadBMP("pictures/restart.bmp");
        texture=SDL_CreateTextureFromSurface(renderer,click_restart);
        SDL_FreeSurface(click_restart);
        SDL_RenderCopy(renderer,texture,NULL,&rect_click_restart);

    //click back
    SDL_Rect rect_click_back;
    rect_click_back.x=20;
    rect_click_back.y=533;
    rect_click_back.w=50;
    rect_click_back.h=50;

    SDL_Surface *click_back;
        click_back=SDL_LoadBMP("pictures/back.bmp");
        texture=SDL_CreateTextureFromSurface(renderer,click_back);
        SDL_FreeSurface(click_back);
        SDL_RenderCopy(renderer,texture,NULL,&rect_click_back);

    //click next
    SDL_Rect rect_click_next;
    rect_click_next.x=75;
    rect_click_next.y=533;
    rect_click_next.w=50;
    rect_click_next.h=50;

    SDL_Surface *click_next;
        click_next=SDL_LoadBMP("pictures/next.bmp");
        texture=SDL_CreateTextureFromSurface(renderer,click_next);
        SDL_FreeSurface(click_next);
        SDL_RenderCopy(renderer,texture,NULL,&rect_click_next);

    //click home
    SDL_Rect rect_click_home;
    rect_click_home.x=550;
    rect_click_home.y=25;
    rect_click_home.w=25;
    rect_click_home.h=25;

    SDL_Surface *click_home;
        click_home=SDL_LoadBMP("pictures/home.bmp");
        texture=SDL_CreateTextureFromSurface(renderer,click_home);
        SDL_FreeSurface(click_home);
        SDL_RenderCopy(renderer,texture,NULL,&rect_click_home);

    //lignes
    int i;
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    for(i=1;i<9;i++)
    {
        SDL_RenderDrawLine(renderer,100+i*50,100,100+i*50,500);
        SDL_RenderDrawLine(renderer,100,100+i*50,500,100+i*50);
    }
    SDL_DestroyTexture(texture);
}

//put pion with highlight
void putpion_with_hint(SDL_Window *window)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture=NULL;
    fondgame(renderer);

    //pions
    SDL_Rect rect_black,rect_white;
    SDL_Surface *pion_black=SDL_LoadBMP("pictures/black.bmp");
    SDL_Surface *pion_white=SDL_LoadBMP("pictures/white.bmp");

    //hint
    SDL_Rect rect_hint;
    SDL_Surface *hint=SDL_LoadBMP("pictures/hight.bmp");

    //hint on
    SDL_Rect rect_click_hint;
    rect_click_hint.x=525;
    rect_click_hint.y=525;
    rect_click_hint.w=100;
    rect_click_hint.h=200;

    SDL_Surface *click_hint=SDL_LoadBMP("pictures/indiceon.bmp");
    texture=SDL_CreateTextureFromSurface(renderer,click_hint);
    SDL_FreeSurface(click_hint);
    SDL_RenderCopy(renderer,texture,NULL,&rect_click_hint);

    int j,k;
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


//put pion without highlight
void putpion(SDL_Window *window)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture=NULL;
    fondgame(renderer);

    //pions
    SDL_Rect rect_black,rect_white;
    SDL_Surface *pion_black=SDL_LoadBMP("pictures/black.bmp");
    SDL_Surface *pion_white=SDL_LoadBMP("pictures/white.bmp");

    //click hint
    SDL_Rect rect_click_hint;
    rect_click_hint.x=525;
    rect_click_hint.y=525;
    rect_click_hint.w=100;
    rect_click_hint.h=200;

    SDL_Surface *click_hint=SDL_LoadBMP("pictures/indiceoff.bmp");
    texture=SDL_CreateTextureFromSurface(renderer,click_hint);
    SDL_FreeSurface(click_hint);
    SDL_RenderCopy(renderer,texture,NULL,&rect_click_hint);

    int j,k;
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

int if_click_highlight(SDL_Event event)
{
    if(event.button.x>525 && event.button.y>525)
    {
        return 1;
    }
    return 0;
}

int if_click_restart(SDL_Event event)
{
    if(event.button.x>200 && event.button.x<400 && event.button.y>525 && event.button.y<575)
    {
        return 1;
    }
    return 0;
}

int if_click_back(SDL_Event event)
{
    if(event.button.x>20 && event.button.x<70 && event.button.y>533 && event.button.y<583)
    {
        return 1;
    }
    return 0;
}

int if_click_next(SDL_Event event)
{
    if(event.button.x>75 && event.button.x<125 && event.button.y>533 && event.button.y<583)
    {
        return 1;
    }
    return 0;
}

int if_click_home(SDL_Event event)
{
    if(event.button.x>550 && event.button.x<575 && event.button.y>25 && event.button.y<50)
    {
        return 1;
    }
    return 0;
}


void restart_game()
{
   int restart_board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,WHITE,BLACK,0,0,0},//3
                 {0,0,0,BLACK,WHITE,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7*/
    memcpy(board,restart_board,sizeof(int)*8*8);
    int restart_highlight_board[8][8]={0};
    memcpy(highlight_board,restart_highlight_board,sizeof(int)*8*8);
    display();
    display_h();
}

void save_the_game(SDL_Window *window,int hint_etat,int *programme,int *stay_in_menu,int type)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    if(hint_etat==1)
    {
        putpion_with_hint(window);
    }
    else{putpion(window);}
    SDL_Event event;

    //save the game (interface)
    SDL_Surface *save=SDL_LoadBMP("pictures/save.bmp");

    SDL_Rect rect_save_the_game;
    rect_save_the_game.x=50;
    rect_save_the_game.y=150;
    rect_save_the_game.w=500;
    rect_save_the_game.h=300;

    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,save);
    SDL_RenderCopy(renderer,texture,NULL,&rect_save_the_game);
    SDL_RenderPresent(renderer);

    int choice=0;
    while (!choice)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                //YES
                if(event.button.x>125 && event.button.x<225 && event.button.y>335 && event.button.y<385)
                {
                    //save
                    SDL_DestroyRenderer(renderer);
                    choice=1;
                }
                //NO
                if(event.button.x>375 && event.button.x<475 && event.button.y>335 && event.button.y<385)
                {
                    //home
                    if(type==1)
                    {
                       *programme=0;
                        restart_game();
                        SDL_DestroyRenderer(renderer);
                        choice=1;
                    }
                    if(type==0)
                    {
                        *programme=0;
                        *stay_in_menu=0;
                        SDL_DestroyRenderer(renderer);
                        choice=1;
                    }

                }
                //cancel
                if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)
                {
                    if(hint_etat==1)
                    {
                        SDL_DestroyRenderer(renderer);putpion_with_hint(window);
                    }
                    else{SDL_DestroyRenderer(renderer);putpion(window);}
                    choice=1;
                }

            }
            if(event.type==SDL_QUIT)
            {
                choice=1;SDL_Quit();
            }
        }
    }
    SDL_FreeSurface(save);
    SDL_DestroyTexture(texture);
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


void show(char * text,SDL_Renderer *renderer)
{
    TTF_Font *font =TTF_OpenFont("Tahoma.ttf",36);
    SDL_Color color ={250,250,250,250};
    SDL_Surface *textsurface;
    SDL_Texture *textu;
    SDL_Rect textRect;
    textRect.x=180;textRect.y=334;
    textsurface = TTF_RenderText_Solid(font,text,color);
    textu=SDL_CreateTextureFromSurface(renderer,textsurface);
    SDL_QueryTexture(textu, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer,textu,NULL,&textRect);
    SDL_RenderPresent(renderer);

}

void fond_username(SDL_Renderer *renderer)
{
    SDL_Rect username_rect;
    username_rect.x=50;
    username_rect.y=150;
    username_rect.w=500;
    username_rect.h=300;

    SDL_Surface *username_surface=SDL_LoadBMP("pictures/username.bmp");
    SDL_Texture *username_texture=SDL_CreateTextureFromSurface(renderer,username_surface);
    SDL_FreeSurface(username_surface);
    SDL_RenderCopy(renderer,username_texture,NULL,&username_rect);

    SDL_DestroyTexture(username_texture);
    SDL_RenderPresent(renderer);
}

void fond_errorusername(SDL_Renderer *renderer)
{
    SDL_Rect username_rect;
    username_rect.x=50;
    username_rect.y=150;
    username_rect.w=500;
    username_rect.h=300;

    SDL_Surface *username_surface=SDL_LoadBMP("pictures/errorusername.bmp");
    SDL_Texture *username_texture=SDL_CreateTextureFromSurface(renderer,username_surface);
    SDL_FreeSurface(username_surface);
    SDL_RenderCopy(renderer,username_texture,NULL,&username_rect);

    SDL_DestroyTexture(username_texture);
    SDL_RenderPresent(renderer);
}

void fond_password(SDL_Renderer *renderer)
{
    SDL_Rect username_rect;
    username_rect.x=50;
    username_rect.y=150;
    username_rect.w=500;
    username_rect.h=300;

    SDL_Surface *username_surface=SDL_LoadBMP("pictures/password.bmp");
    SDL_Texture *username_texture=SDL_CreateTextureFromSurface(renderer,username_surface);
    SDL_FreeSurface(username_surface);
    SDL_RenderCopy(renderer,username_texture,NULL,&username_rect);

    SDL_DestroyTexture(username_texture);
    SDL_RenderPresent(renderer);
}

void fond_usernamenotfound(SDL_Renderer *renderer)
{
    SDL_Rect username_rect;
    username_rect.x=50;
    username_rect.y=150;
    username_rect.w=500;
    username_rect.h=300;

    SDL_Surface *username_surface=SDL_LoadBMP("pictures/unsernamenotfound.bmp");
    SDL_Texture *username_texture=SDL_CreateTextureFromSurface(renderer,username_surface);
    SDL_FreeSurface(username_surface);
    SDL_RenderCopy(renderer,username_texture,NULL,&username_rect);

    SDL_DestroyTexture(username_texture);
    SDL_RenderPresent(renderer);
}

void fond_errorpassword(SDL_Renderer *renderer)
{
    SDL_Rect username_rect;
    username_rect.x=50;
    username_rect.y=150;
    username_rect.w=500;
    username_rect.h=300;

    SDL_Surface *username_surface=SDL_LoadBMP("pictures/errorpassword.bmp");
    SDL_Texture *username_texture=SDL_CreateTextureFromSurface(renderer,username_surface);
    SDL_FreeSurface(username_surface);
    SDL_RenderCopy(renderer,username_texture,NULL,&username_rect);

    SDL_DestroyTexture(username_texture);
    SDL_RenderPresent(renderer);
}


/***********************************************C****************************************/
int player_turn(int last_turn,int* pass_turn)//give who's player turn based on the last_turn with highlighting //v
{
    int k=0,i,j,highlight_full=0,black_num=0,white_num=0,board_total=0;
    for (i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            k+=contour(i,j,last_turn);//this is the instruction that highlight
            if(board[i][j]==1)
            {
                black_num++;
            }
            if(board[i][j]==-1)
            {
                white_num++;
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
    board_total=black_num+white_num;
    if(board_total==64)
    {
        return 0;
    }
    if (highlight_full!=0)
    {
        *pass_turn=0;printf("gggg");
        return -last_turn;//if there is a possible case
    }
    if(highlight_full==0 && black_num!=0 && white_num!=0)//no possible case
    {   if(*pass_turn<2)
        {
            (*pass_turn)++;
            player_turn(-last_turn,&(*pass_turn));
        }
        else{return 0;}
    }
    if(black_num==0 || white_num==0)
    {return 0;}
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
            if(board[i][j]==WHITE)
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

void update_list_board(board_list **ptr_list_boards,int last_turn)      //we should use it in every round
{
    int i,j;
    board_list *new_board=Malloc(board_list);
    new_board->next=NULL;
    new_board->previous=(*ptr_list_boards);
    new_board->_last_turn=last_turn;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            new_board->board_m[i][j]=board[i][j];

        }
    }
    (*ptr_list_boards)->next=new_board;
    (*ptr_list_boards)=new_board;
}

void free_ptr(board_list **ptr_list_boards)          //if the player played while he got back
{
    if ((*ptr_list_boards)->next==NULL)
        {return;}
    else
       {
           free((*ptr_list_boards)->next);return;
       }

}

void move_backward(board_list **ptr_list_boards,int last_turn)
{
    if((*ptr_list_boards)->previous !=NULL)
    {
        (*ptr_list_boards)=(*ptr_list_boards)->previous;
        int i,j;
        memcpy(board,(*ptr_list_boards)->board_m,sizeof(int)*8*8);
        display();
        last_turn=(*ptr_list_boards)->_last_turn;
    }
}

void move_forward(board_list **ptr_list_boards,int last_turn)
{
    if((*ptr_list_boards)->next !=NULL)
    {
        (*ptr_list_boards)=(*ptr_list_boards)->next;
        memcpy(board,(*ptr_list_boards)->board_m,sizeof(int)*8*8);
        last_turn=(*ptr_list_boards)->_last_turn;
    }
}

int sign_in(SDL_Renderer *renderer)//games_mode_name
{
    //creat folder
    char password[20];strcpy(password,"");
    char username[20];strcpy(username,"");
    char path[100];
    char user_path[100];
    int k=0;
    int running=1,error=0;

    fond_username(renderer);
    do{

    sprintf(path,"games/");
    strcpy(user_path,path);
    if(error)
    {
        fond_errorusername(renderer);
    }
    printf("\nentre your name :");
    while(running)
    {
        show(username,renderer);
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_SPACE :
                    strcat(username,"");break;
                case SDLK_RETURN :
                    running=0;break;
                case SDLK_BACKSPACE:
                    username[strlen(username)-1]='\0';
                    break;
                default:
                    if(strlen(username)<7 && (((int)event.key.keysym.sym>=48 &&(int)event.key.keysym.sym<=57)
                       || ((int)event.key.keysym.sym>=97 &&(int)event.key.keysym.sym<=122)))
                    {
                    username[strlen(username)]=event.key.keysym.sym;
                    if(k==0){username[1]='\0';k++;}//some errors
                    if(k==1){username[2]='\0';k++;}
                    if(k==2 && strlen(username)==4){username[3]='\0';k++;}
                    break;}
                    else{break;}
                }
                if(error)
                {
                    fond_errorusername(renderer);
                }
                else{fond_username(renderer);}
                show(username,renderer);
                break;
            case SDL_MOUSEBUTTONDOWN :
                if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)
                {
                    return 0;
                }
                break;
            case SDL_QUIT :
                exit(1);
            default: break;
            }
        }
    }

    strcat(path,username);
    if(isDirectoryExists(path))
    {
        running=1;
        printf("\nname already exist try another name");
        error=1;
        continue;
    }
    break;
    }while(1);


    printf("\nentrer your password :");
    fond_password(renderer);
    running=1;k=0;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_SPACE :
                    strcat(password,"");break;
                case SDLK_RETURN :
                    running=0;break;
                case SDLK_BACKSPACE:
                    password[strlen(password)-1]='\0';
                    break;
                default:
                    if(strlen(password)<7 && (((int)event.key.keysym.sym>=48 &&(int)event.key.keysym.sym<=57)
                       || ((int)event.key.keysym.sym>=97 &&(int)event.key.keysym.sym<=122)))
                    {
                    password[strlen(password)]=event.key.keysym.sym;
                    if(k==0){password[1]='\0';k++;}//some errors
                    if(k==1){password[2]='\0';k++;}
                    if(k==2 && strlen(password)==4){password[3]='\0';k++;}
                    break;}
                    else{break;}
                    }
                fond_password(renderer);
                show(password,renderer);
                break;
            case SDL_MOUSEBUTTONDOWN :
                if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)
                {
                    return 0;
                }
                break;
            case SDL_QUIT :
                exit(1);
            default: break;
            }
        }
    }

    creat_folder(path);

    //folder created
    strcat(user_path,"users.txt");
    FILE *file;
    file=fopen(user_path,"a");

    //open the user file
    if (file==NULL)
    {
        printf("couldn't open the file ");
        return 0;
    }

    code(username,key);
    code(password,key);
    //append the name and the password
    fprintf(file,"%s",username);
    fprintf(file,"\n");
    fprintf(file,password);
    fprintf(file,"\n");

    fclose(file);
    return 1;

}

int decode (char *str,int key)
{
    int i;
	for(i = 0; str[i] != '\0'; ++i)
	{
	    str[i]-=key;
	}
	return 0;
}

int log_in(SDL_Renderer *renderer)
{
    char password[30]="";
    char username[20]="";
    char path[100]="";
    char user_path[100]="";
    int k=0;
    int running=1,error=0;

    fond_username(renderer);
    do{
        sprintf(path,"games/");
        strcpy(user_path,path);
        if(error)
        {
            fond_usernamenotfound(renderer);
        }
        printf("\nentre your name :");
        while(running)
        {
            show(username,renderer);
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {

                switch(event.type)
                {

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_SPACE :
                        strcat(username,"");break;
                    case SDLK_RETURN :
                        running=0;break;
                    case SDLK_BACKSPACE:
                        username[strlen(username)-1]='\0';
                        break;
                    default:
                        if(strlen(username)<7 && (((int)event.key.keysym.sym>=48 &&(int)event.key.keysym.sym<=57)
                           || ((int)event.key.keysym.sym>=97 &&(int)event.key.keysym.sym<=122)))
                        {
                        username[strlen(username)]=event.key.keysym.sym;
                        if(k==0){username[1]='\0';k++;}//some errors
                        if(k==1){username[2]='\0';k++;}
                        //if(k==2 && strlen(username)==4){username[3]='\0';k++;}
                        break;}
                        else{break;}
                    }
                    if(error)
                    {
                        fond_usernamenotfound(renderer);
                    }
                    else{fond_username(renderer);}
                    show(username,renderer);
                    break;
                case SDL_MOUSEBUTTONDOWN :
                    if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)
                    {
                        return 0;
                    }
                    break;
                case SDL_QUIT :
                    exit(1);
                default: break;
                }
            }
        }
    strcat(path,username);

    //folder created
    strcat(user_path,"users.txt");
    FILE *file;
    file=fopen(user_path,"r");
    int found=0;
    int max=0;
    while(!(found || feof(file)))
    {
        char password_u[20];
        char username_u[20];
        fscanf(file,"%s\n%s",username_u,password_u);
        decode(username_u,key);
        decode(password_u,key);

        if (!strcmp(username_u,username))
            {
                found=1;
                break;
            }
    }
    if (found)
    {
        printf("username exist");
        break;
    }
    else
       {   running=1;
           printf("username incorecte");
           error=1;
       }
}while(1);

    fond_password(renderer);
    running=1;k=0;error=0;
    do
    {
        sprintf(path,"games/");
        strcpy(user_path,path);
        printf("\nentrer your password :");
        if(error)
        {
            fond_errorpassword(renderer);
        }
        while(running)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {

                switch(event.type)
                {

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_SPACE :
                        strcat(password,"");break;
                    case SDLK_RETURN :
                        running=0;break;
                    case SDLK_BACKSPACE:
                        password[strlen(password)-1]='\0';
                        break;
                    default:
                        if(strlen(password)<7 && (((int)event.key.keysym.sym>=48 &&(int)event.key.keysym.sym<=57)
                           || ((int)event.key.keysym.sym>=97 &&(int)event.key.keysym.sym<=122)))
                        {
                        password[strlen(password)]=event.key.keysym.sym;
                        if(k==0){password[1]='\0';k++;}//some errors
                        if(k==1){password[2]='\0';k++;}
                        //if(k==2 && strlen(password)==4){password[3]='\0';k++;}
                        break;}
                        else{break;}
                        }
                    if(error)
                    {
                        fond_errorpassword(renderer);
                    }
                    else{fond_password(renderer);}
                    show(password,renderer);
                    break;
                case SDL_MOUSEBUTTONDOWN :
                    if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)
                    {
                        return 0;
                    }
                    break;
                case SDL_QUIT :
                    exit(1);
                default: break;
                }
            }
        }
        strcat(path,username);

        //folder created
        strcat(user_path,"users.txt");
        FILE *file;
        file=fopen(user_path,"r");
        int found=0;
        int max=0;
        while(!(found || feof(file)))
        {
            char password_u[20];
            char name_u[20];
            fscanf(file,"%s\n%s",name_u,password_u);
            decode(name_u,key);
            decode(password_u,key);

            if (!strcmp(password_u,password))
                {
                    found=1;
                    break;
                }
        }
        if (found)
        {
            printf("password correct");
            break;
        }
        else
           {   running=1;
               printf("username incorecte");
               error=1;
           }
    }while(1);
    return 1;
}



int code (char *str,int key)
{
    int i ;
	for(i = 0; str[i] != '\0'; ++i)
	{
	    str[i]+=key;
	}
	return 0;
}


int creat_folder(char *dir_name)
{
   int check;

   check = mkdir(dir_name);
   //checking if directory is created
   if (!check)
   {
      printf("Directory created\n");
      return 1;
   }
   else {
      printf("Unable to create directory\n");
      return 0;
   }
}

int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}
