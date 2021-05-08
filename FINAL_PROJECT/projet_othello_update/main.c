#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unistd.h>
#include <sys/stat.h>
#include <SDL_mixer.h>


#define largeur 600
#define hauteur 600
#define BLACK 1
#define WHITE -1
#define EMPTY_SPACE 0
#define HIGHLIGHTER 2
#define Malloc(type) (type*)malloc(sizeof(type))



int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,WHITE,BLACK,0,0,0},//3
                 {0,0,0,BLACK,WHITE,0,0,0},//4
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

typedef struct _party
{
    struct _party *next;
    char text[8];
    struct _party *previous;
}party;

typedef struct _scores_list
{
    char name[20];
    int score;
    char mode[10];
}scores_list;

int key=10;
char player_name[20];
char mode[5];
int reloaddone=0,reloaddone2=0,total=0,reload_from_1vs1=0,reload_from_ia=0;
char game_name[20];
int save_history_counter=0;


/***************************  l'explication de chaque fonction est dans le rapport  ******************************/
void first_page(SDL_Window *window);
void menu(SDL_Window *window,SDL_Event event,int *num_menu);
void putpion(SDL_Window *window,int last_turn);
void putpion_with_hint(SDL_Window *window,int last_turn);
void fondgame(SDL_Renderer *renderer,int last_turn);
int if_click_highlight(SDL_Event event);
void initialisation_des_coordonnes(SDL_Rect coordonnes_case[8][8]);
void coordonne_click(int *ligne,int *colonne,SDL_Rect coordonnes_case[8][8],SDL_Event event);
int if_click_replay(SDL_Event event);
void restart_game();
int if_click_back(SDL_Event event);
int if_click_next(SDL_Event event);
int if_click_home(SDL_Event event);
int if_click_historic(SDL_Window *Window);
void save_the_game(SDL_Window *window,int hint_etat,int *programme,int *stay_in_menu,int type,int last_turn);
void show(char * text,SDL_Renderer *renderer);
void bestscores(SDL_Window *window);
SDL_Renderer* fond_first_page(SDL_Window *window);
void fond(SDL_Renderer *renderer,char* surface_name);
void fond_errornameparty(SDL_Renderer *renderer);
void fond_nameparty(SDL_Renderer *renderer);
void fond_username(SDL_Renderer *renderer);
void fond_password(SDL_Renderer *renderer);
void fond_errorusername(SDL_Renderer *renderer);
void fond_usernamenotfound(SDL_Renderer *renderer);
void fond_errorpassword(SDL_Renderer *renderer);
void fond_chooseparty(SDL_Renderer *renderer);
void fond_choosemode(SDL_Renderer *renderer);
void fond_menu(SDL_Renderer *renderer);
void fond_historic(SDL_Renderer *renderer);
void rules(SDL_Window *window);
int choosemode_inreload(SDL_Window *window,int *last_turn);
int chooseparty(SDL_Renderer *renderer,int *last_turn);


/*****************************************************c****************************/


int player_turn(int last_turn,int *pass_turn);
int contour (int pos_l,int pos_c,int player);
void click_at(int player,int click_l,int click_c);
void reset_h();
void set_color(int pos_l,int pos_c,int player);
int still_in_board(int pos_l,int pos_c);
int search_player(int pos_l,int pos_c,int player,int player_origin,int dl,int dc);
void mark_position(int pos_l,int pos_c);
void display();
void display_h();
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
int sauvegarde(int *last_turn);
int reload(int *last_turn);
int save(int *last_turn,SDL_Renderer *renderer);
void save_history (int pos_l,int pos_c,int player);
void increment_decrement(int i);
void best_scores(int score_black);
void simple_IA(int *pos_l,int *pos_c);
void remove_files();


int main( int argc, char * argv[])
{

    SDL_Window* window=NULL;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

    Mix_Music *background_music = Mix_LoadMUS("soundtrack/background_music.mp3");
    Mix_Chunk *click = Mix_LoadWAV("soundtrack/button_click.wav");

    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(click,MIX_MAX_VOLUME);

    window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,largeur,hauteur,SDL_WINDOW_OPENGL);
    SDL_Event event;
    int stay_in_menu=1;int last_turn;
    int save=0,pass_turn=0;
    int HOME=1;int QUIT=0;

    Mix_PlayMusic(background_music,-1);
    first_page(window);
    printf("\nHELLO %s",player_name);
    while (stay_in_menu)
    {
    //menu
    int num_menu=0;
    if(reloaddone==1)
    {
        reloaddone=0;
        if(reload_from_1vs1==1)
        {
            reload_from_1vs1=0;
            num_menu=2;
        }
        if(reload_from_ia==1)
        {
            reload_from_ia=0;
            num_menu=1;
        }
        reset_h();
    }
    else {menu(window,event,&num_menu);last_turn=BLACK;}
    printf("\nmenu %d\n",num_menu);
    //coordonnes des cases:
    SDL_Rect coordonnes_case[8][8]={0};
    initialisation_des_coordonnes(coordonnes_case);

    total=0;save_history_counter=0;
    //click restart
    int onrestart=-1;//out of restart

    //indice etat :
    int hint_etat=-1;//off

    //list of boards
    board_list *list_boards;
    list_boards=Malloc(board_list);
    board_list *to_free=list_boards;

    //first player
    list_boards->_last_turn=last_turn;
    list_boards->previous=NULL;
    list_boards->next=NULL;
    memcpy(list_boards->board_m,board,sizeof(int)*8*8);

    last_turn=player_turn(last_turn,&pass_turn);

    int click_l=-1;
    int click_c=-1;

    int programme= 1;

    switch(num_menu)
    {
    case 1://mode 1vsIA
        strcpy(mode,"1vsai");
        //board+indices
        display();
        display_h();
        putpion(window,last_turn);

        while(programme)
        {
            if(last_turn==-1)
            {
            while(SDL_PollEvent(&event))
            {

                switch(event.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                            click_c=-1;
                            click_l=-1;
                            coordonne_click(&click_l,&click_c,coordonnes_case,event);
                            if(if_click_home(event))
                            {
                                printf("\n home");
                                save_the_game(window,hint_etat,&programme,&stay_in_menu,HOME,list_boards->_last_turn);
                            }

                            if(if_click_next(event))
                            {
                                printf("\n next");
                               move_forward(&list_boards,last_turn);
                               move_forward(&list_boards,last_turn);
                                reset_h();
                                last_turn=list_boards->_last_turn;
                                last_turn=player_turn(last_turn,&pass_turn);
                                display();
                                display_h();
                                if(hint_etat==1)
                                {putpion_with_hint(window,last_turn);}
                                else{putpion(window,last_turn);}
                                printf("%d",save_history_counter);

                            }

                            if(if_click_back(event))
                            {
                                printf("\n back");
                                move_backward(&list_boards,last_turn);
                                move_backward(&list_boards,last_turn);
                                reset_h();
                                last_turn=list_boards->_last_turn;
                                last_turn=player_turn(last_turn,&pass_turn);
                                display();
                                display_h();
                                if(hint_etat==1)
                                {putpion_with_hint(window,last_turn);}
                                else{putpion(window,last_turn);}
                                printf("%d",save_history_counter);

                            }

                            if(if_click_highlight(event))
                            {
                                printf("\nhighlight");
                                hint_etat=-hint_etat;
                                if(hint_etat==1)
                                {putpion_with_hint(window,last_turn);}
                                else{putpion(window,last_turn);}
                            }

                           if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==-1)  // valide move+hint off
                            {
                                Mix_PlayChannel(-1,click,0);
                                printf("\n move1");
                                free_ptr(&list_boards);
                                set_color(click_l,click_c,-last_turn);
                                update_list_board(&list_boards,last_turn);
                                reset_h();
                                printf("%d",last_turn);
                                save_history(click_l+1,click_c,last_turn);
                                last_turn=player_turn(last_turn,&pass_turn);
                                putpion(window,last_turn);
                                display();
                                display_h();
                            }

                            if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==1)  // valide move+hint on
                            {
                                Mix_PlayChannel(-1,click,0);
                                printf("\n move2");
                                free_ptr(&list_boards);
                                set_color(click_l,click_c,-last_turn);
                                update_list_board(&list_boards,last_turn);
                                int i,j;
                                reset_h();
                                printf("%d",last_turn);
                                save_history(click_l+1,click_c,last_turn);
                                last_turn=player_turn(last_turn,&pass_turn);
                                putpion_with_hint(window,last_turn);
                                display();
                                display_h();
                            }

                            if(last_turn==0)
                            {
                                save_history(click_l+1,click_c,last_turn);
                                if_click_historic(window);
                                programme = 0;
                                restart_game();
                                remove_files();
                            }

                            if(if_click_restart(event))
                            {
                                printf("\n restart");
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
                                putpion(window,last_turn);
                                save_history_counter=0;
                            }
                            if(event.button.x>265 && event.button.x<465 && event.button.y>15 && event.button.y<45)
                            {
                                printf("\n historic\n");
                                if_click_historic(window);
                               if(hint_etat==1)
                                {putpion_with_hint(window,last_turn);}
                                else{putpion(window,last_turn);}
                            }
                        break;

                    case SDL_QUIT :
                        save_the_game(window,hint_etat,&programme,&stay_in_menu,QUIT,list_boards->_last_turn);
                     break;
                    default: break;
                }
            }
        }

        else
        {
                simple_IA(&click_l,&click_c);

                if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==-1)  // valide move+hint off
                {
                    Mix_PlayChannel(-1,click,0);
                    printf("\n move1");
                    free_ptr(&list_boards);
                    set_color(click_l,click_c,-last_turn);
                    update_list_board(&list_boards,last_turn);
                    reset_h();
                    printf("%d",last_turn);
                    save_history(click_l+1,click_c,last_turn);
                    last_turn=player_turn(last_turn,&pass_turn);
                    putpion(window,last_turn);
                    display();
                    display_h();
                }

                if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==1)  // valide move+hint on
                {
                    Mix_PlayChannel(-1,click,0);
                    printf("\n move2");
                    free_ptr(&list_boards);
                    set_color(click_l,click_c,-last_turn);
                    update_list_board(&list_boards,last_turn);
                    int i,j;
                    reset_h();
                    printf("%d",last_turn);
                    save_history(click_l+1,click_c,last_turn);
                    last_turn=player_turn(last_turn,&pass_turn);
                    putpion_with_hint(window,last_turn);
                    display();
                    display_h();
                }

                if(last_turn==0)
                {
                    save_history(click_l+1,click_c,last_turn);
                    if_click_historic(window);
                    programme = 0;
                    restart_game();
                    remove_files();
                }
            }
        }
        free(to_free);break;

    case 2://mode 1vs1

    strcpy(mode,"1vs1");
    //board+indices
    display();
    display_h();
    putpion(window,last_turn);
    while(programme)
    {

        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                        click_c=-1;
                        click_l=-1;
                        coordonne_click(&click_l,&click_c,coordonnes_case,event);
                        if(if_click_home(event))
                        {
                            printf("\n home");
                            save_the_game(window,hint_etat,&programme,&stay_in_menu,HOME,list_boards->_last_turn);
                        }

                        if(if_click_next(event))
                        {
                            printf("\n next");
                           move_forward(&list_boards,last_turn);
                            reset_h();
                            last_turn=list_boards->_last_turn;
                            last_turn=player_turn(last_turn,&pass_turn);
                            display();
                            display_h();
                            if(hint_etat==1)
                            {putpion_with_hint(window,last_turn);}
                            else{putpion(window,last_turn);}
                            printf("%d",save_history_counter);

                        }

                        if(if_click_back(event))
                        {
                            printf("\n back");
                            move_backward(&list_boards,last_turn);
                            reset_h();
                            last_turn=list_boards->_last_turn;
                            last_turn=player_turn(last_turn,&pass_turn);
                            display();
                            display_h();
                            if(hint_etat==1)
                            {putpion_with_hint(window,last_turn);}
                            else{putpion(window,last_turn);}
                            printf("%d",save_history_counter);

                        }

                        if(if_click_highlight(event))
                        {
                            printf("\nhighlight");
                            hint_etat=-hint_etat;
                            if(hint_etat==1)
                            {putpion_with_hint(window,last_turn);}
                            else{putpion(window,last_turn);}
                        }

                       if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==-1)  // valide move+hint off
                        {
                            Mix_PlayChannel(-1,click,0);
                            printf("\n move1");
                            free_ptr(&list_boards);
                            set_color(click_l,click_c,-last_turn);
                            update_list_board(&list_boards,last_turn);
                            reset_h();
                            printf("%d",last_turn);
                            save_history(click_l+1,click_c,last_turn);
                            last_turn=player_turn(last_turn,&pass_turn);
                            putpion(window,last_turn);
                            display();
                            display_h();
                        }

                        if (highlight_board[click_l][click_c]==HIGHLIGHTER && hint_etat==1)  // valide move+hint on
                        {
                            Mix_PlayChannel(-1,click,0);
                            printf("\n move2");
                            free_ptr(&list_boards);
                            set_color(click_l,click_c,-last_turn);
                            update_list_board(&list_boards,last_turn);
                            int i,j;
                            reset_h();
                            printf("%d",last_turn);
                            save_history(click_l+1,click_c,last_turn);
                            last_turn=player_turn(last_turn,&pass_turn);
                            putpion_with_hint(window,last_turn);
                            display();
                            display_h();
                        }

                        if(last_turn==0)
                        {
                            save_history(click_l+1,click_c,last_turn);
                            if_click_historic(window);
                            programme = 0;
                            restart_game();
                            remove_files();
                        }

                        if(if_click_restart(event))
                        {
                            printf("\n restart");
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
                            putpion(window,last_turn);
                            save_history_counter=0;
                        }
                        if(event.button.x>265 && event.button.x<465 && event.button.y>15 && event.button.y<45)
                        {
                            printf("\n historic\n");
                            if_click_historic(window);
                           if(hint_etat==1)
                            {putpion_with_hint(window,last_turn);}
                            else{putpion(window,last_turn);}
                        }
                    break;

                case SDL_QUIT :
                    save_the_game(window,hint_etat,&programme,&stay_in_menu,QUIT,list_boards->_last_turn);
                 break;
                default: break;
            }
        }
    }
    free(to_free);break;
    case 3:
        if(choosemode_inreload(window,&last_turn)==1)
        {
            reloaddone=1;
            reloaddone2=1;
        }
        break;
    case 4:
        bestscores(window);
        break;
    case 5:
        rules(window);
        break;
    default:break;
    }
    }
    SDL_DestroyWindow(window);
    Mix_FreeMusic(background_music);
    Mix_FreeChunk(click);
    Mix_CloseAudio();
    TTF_Quit();
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
        renderer=fond_first_page(window);
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

void fond_menu(SDL_Renderer *renderer)
{
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
}


void menu(SDL_Window *window,SDL_Event event,int *num_menu)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);

    fond_menu(renderer);

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


void fondgame(SDL_Renderer *renderer,int last_turn)
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

    //marks
    SDL_Rect rect_marks;
    rect_marks.x=75;
    rect_marks.y=75;
    rect_marks.w=408;
    rect_marks.h=440;

    SDL_Surface *marks;
        marks=SDL_LoadBMP("pictures/marks.bmp");
        texture=SDL_CreateTextureFromSurface(renderer,marks);
        SDL_FreeSurface(marks);
        SDL_RenderCopy(renderer,texture,NULL,&rect_marks);

    //lignes
    int i;
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    for(i=1;i<9;i++)
    {
        SDL_RenderDrawLine(renderer,100+i*50,100,100+i*50,500);
        SDL_RenderDrawLine(renderer,100,100+i*50,500,100+i*50);
    }

    //players
    SDL_Rect rect_players;
    rect_players.x=25;
    rect_players.y=5;
    rect_players.w=145;
    rect_players.h=55;

    SDL_Surface *players;
        players=SDL_LoadBMP("pictures/players.bmp");
        texture=SDL_CreateTextureFromSurface(renderer,players);
        SDL_FreeSurface(players);
        SDL_RenderCopy(renderer,texture,NULL,&rect_players);

    //scores
    int j;int black_num=0,white_num=0;
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

    char text[2];
    TTF_Font *font =TTF_OpenFont("Tahoma.ttf",24);
    SDL_Color black ={0,0,0,0};
    SDL_Surface *textsurface;
    SDL_Texture *textu;
    SDL_Rect textRect;
    //black
    textRect.x=180;textRect.y=2;
    sprintf(text,"%d",black_num);
    textsurface = TTF_RenderText_Solid(font,text,black);
    textu=SDL_CreateTextureFromSurface(renderer,textsurface);
    SDL_QueryTexture(textu, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer,textu,NULL,&textRect);
    //white
    SDL_Color white ={250,250,250,250};
    textRect.x=180;textRect.y=28;
    sprintf(text,"%d",white_num);
    textsurface = TTF_RenderText_Solid(font,text,white);
    textu=SDL_CreateTextureFromSurface(renderer,textsurface);
    SDL_QueryTexture(textu, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer,textu,NULL,&textRect);

    SDL_DestroyTexture(textu);

    //historic
    SDL_Rect rect_historic;
    rect_historic.x=265;
    rect_historic.y=15;
    rect_historic.w=200;
    rect_historic.h=30;

    SDL_Surface *historic;
    historic=SDL_LoadBMP("pictures/historic.bmp");
    texture=SDL_CreateTextureFromSurface(renderer,historic);
    SDL_FreeSurface(historic);
    SDL_RenderCopy(renderer,texture,NULL,&rect_historic);

    //turn_hint
    SDL_Rect rect_turn;
    rect_turn.x=10;
    rect_turn.w=10;
    rect_turn.h=10;
    SDL_Surface *turn;
    if(last_turn==-1)
    {
        rect_turn.y=10;
        turn=SDL_LoadBMP("pictures/black.bmp");
    }
    else
    {
        rect_turn.y=40;
        turn=SDL_LoadBMP("pictures/white.bmp");
    }

    texture=SDL_CreateTextureFromSurface(renderer,turn);
    SDL_FreeSurface(turn);
    SDL_RenderCopy(renderer,texture,NULL,&rect_turn);

    SDL_DestroyTexture(texture);
}

void rules(SDL_Window *window)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);

    SDL_Rect fond_rect;
    fond_rect.x=0;
    fond_rect.y=0;
    fond_rect.w=largeur;
    fond_rect.h=hauteur;


    SDL_Surface *fond=SDL_LoadBMP("pictures/rules.bmp");
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,fond);
    SDL_FreeSurface(fond);
    SDL_RenderCopy(renderer,texture,NULL,&fond_rect);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    int programme=1;
    while(programme)
        {
            while(SDL_PollEvent(&event))
            {

                switch(event.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        if(if_click_home(event))
                        {
                            programme=0;
                        }break;
                    default: break;
                }
            }
        }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

}

//put pion with highlight
void putpion_with_hint(SDL_Window *window,int last_turn)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture=NULL;
    fondgame(renderer,last_turn);

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
void putpion(SDL_Window *window,int last_turn)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture=NULL;
    fondgame(renderer,last_turn);

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

int if_click_historic(SDL_Window *window)
{
        SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
        SDL_Event event1;

        fond_historic(renderer);

        char text[20],path[100];FILE *file=NULL;
        TTF_Font *font =TTF_OpenFont("Tahoma.ttf",18);
        SDL_Color black ={0,0,0,0};
        SDL_Surface *textsurface;
        SDL_Texture *textu;

        int i=0;int total=0;
        if(strcmp(game_name,"")!=0)
        {
            sprintf(path,"games/%s/%s/history-%s.txt",player_name,mode,game_name);
            file=fopen(path,"r");
            if (file==NULL)
            {
                printf("error file history");
            }

            while(!feof(file))
            {
            fscanf(file,"%s\n",text);
            if(strcmp(text,"******")==0)
            {
                i=i+2;
            }
            SDL_Rect textRect;
            if(total<29)
            {
                textRect.x=100;
                textRect.y=10+20*i;
            }
            else
            {
                if(total<58)
                {
                    textRect.x=200;
                    textRect.y=10+20*(i-29);
                }
                else
                {
                    textRect.x=300;
                    textRect.y=10+20*(i-58);
                }
            }
            i++;
            textsurface = TTF_RenderText_Solid(font,text,black);
            textu=SDL_CreateTextureFromSurface(renderer,textsurface);
            SDL_QueryTexture(textu, NULL, NULL, &textRect.w, &textRect.h);
            SDL_RenderCopy(renderer,textu,NULL,&textRect);
            SDL_DestroyTexture(textu);
            SDL_RenderPresent(renderer);
            total++;
            }
            fclose(file);
        }
        sprintf(path,"games/%s/%s/history.txt",player_name,mode);
        file=NULL;
        printf("%s",path);
        file=fopen(path,"r");
        if (file!=NULL)
        {
            while(!feof(file))
            {
            fscanf(file,"%s\n",text);
            if(strcmp(text,"******")==0)
            {
                i=i+2;
            }
            SDL_Rect textRect;
            if(total<29)
            {
                textRect.x=100;
                textRect.y=10+20*i;
            }
            else
            {
                if(total<58)
                {
                    textRect.x=200;
                    textRect.y=10+20*(i-29);
                }
                else
                {
                    textRect.x=300;
                    textRect.y=10+20*(i-58);
                }
            }
            i++;
            textsurface = TTF_RenderText_Solid(font,text,black);
            textu=SDL_CreateTextureFromSurface(renderer,textsurface);
            SDL_QueryTexture(textu, NULL, NULL, &textRect.w, &textRect.h);
            SDL_RenderCopy(renderer,textu,NULL,&textRect);
            SDL_DestroyTexture(textu);
            SDL_RenderPresent(renderer);
            total++;
            }
            fclose(file);printf("file closed");
        }
        i=1;
        while(i)
        {
            while(SDL_PollEvent(&event1))
            {
                switch(event1.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        if(event1.button.x>550 && event1.button.x<570 && event1.button.y>20 && event1.button.y<40)
                        {
                            i=0;
                        }
                        break;
                    case SDL_QUIT :
                        i=0;
                    default : break;
                }
            }
        }
        SDL_DestroyRenderer(renderer);
        printf("end of history");
        return 1;
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

void save_the_game(SDL_Window *window,int hint_etat,int *programme,int *stay_in_menu,int type,int last_turn)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    if(hint_etat==1)
    {
        putpion_with_hint(window,last_turn);
    }
    else{putpion(window,last_turn);}
    SDL_Event event;

    //save the game (interface)
    SDL_Surface *surface_save=SDL_LoadBMP("pictures/save.bmp");

    SDL_Rect rect_save_the_game;
    rect_save_the_game.x=50;
    rect_save_the_game.y=150;
    rect_save_the_game.w=500;
    rect_save_the_game.h=300;

    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface_save);
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
                    if(save(last_turn,renderer)==0)//cancel
                    {
                        if(hint_etat==1)
                        {
                            SDL_DestroyRenderer(renderer);putpion_with_hint(window,last_turn);
                        }
                        else{SDL_DestroyRenderer(renderer);putpion(window,last_turn);}
                        choice=1;
                    }
                    else
                    {
                        *programme=0;
                        restart_game();
                        strcpy(mode,"");
                        strcpy(game_name,"");
                        SDL_DestroyRenderer(renderer);
                        choice=1;
                    }
                }
                //NO
                if(event.button.x>375 && event.button.x<475 && event.button.y>335 && event.button.y<385)
                {
                    char path[100];
                    sprintf(path,"games/%s/%s/history.txt",player_name,mode);
                    remove(path);
                    //home
                    if(type==1)
                    {
                       *programme=0;
                        restart_game();
                        strcpy(mode,"");
                        strcpy(game_name,"");
                        SDL_DestroyRenderer(renderer);
                        choice=1;
                    }
                    //quit
                    if(type==0)
                    {
                        *programme=0;
                        *stay_in_menu=0;
                        SDL_DestroyRenderer(renderer);
                        choice=1;
                    }
                    reloaddone2=0;
                }
                //cancel
                if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)
                {
                    if(hint_etat==1)
                    {
                        SDL_DestroyRenderer(renderer);putpion_with_hint(window,last_turn);
                    }
                    else{SDL_DestroyRenderer(renderer);putpion(window,last_turn);}
                    choice=1;
                }

            }
            if(event.type==SDL_QUIT)
            {
                char path[100];
                sprintf(path,"games/%s/%s/history.txt",player_name,mode);
                remove(path);
                strcpy(mode,"");
                strcpy(game_name,"");
                choice=1;SDL_Quit();
            }
        }
    }
    SDL_FreeSurface(surface_save);
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
    TTF_CloseFont(font);
}

void fond(SDL_Renderer *renderer,char *surface_name)
{
    SDL_Rect rect;
    rect.x=50;
    rect.y=150;
    rect.w=500;
    rect.h=300;

    char path[50];
    sprintf(path,"pictures/%s.bmp",surface_name);
    SDL_Surface *surface=SDL_LoadBMP(path);
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer,texture,NULL,&rect);

    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}


void fond_username(SDL_Renderer *renderer)
{
    fond(renderer,"username");
}

void fond_errorusername(SDL_Renderer *renderer)
{
    fond(renderer,"errorusername");
}

void fond_password(SDL_Renderer *renderer)
{
    fond(renderer,"password");
}

void fond_usernamenotfound(SDL_Renderer *renderer)
{
   fond(renderer,"unsernamenotfound");
}

void fond_errorpassword(SDL_Renderer *renderer)
{
    fond(renderer,"errorpassword");
}

void fond_nameparty(SDL_Renderer *renderer)
{
    fond(renderer,"nameparty");
}

void fond_errornameparty(SDL_Renderer *renderer)
{
    fond(renderer,"errornameparty");
}

void fond_chooseparty(SDL_Renderer *renderer)
{
    fond(renderer,"chooseparty");
}

void fond_choosemode(SDL_Renderer *renderer)
{
    fond(renderer,"choosemode");
}

void fond_historic(SDL_Renderer *renderer)
{
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=600;
    rect.h=600;

    SDL_Surface *surface=SDL_LoadBMP("pictures/show_historic.bmp");
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer,texture,NULL,&rect);

    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}

int choosemode_inreload(SDL_Window *window,int *last_turn)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    fond_menu(renderer);
    fond_choosemode(renderer);
    int choice=0;
    SDL_Event event;
    while (!choice)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                //1vsai
                if(event.button.x>100 && event.button.x<250 && event.button.y>350 && event.button.y<390)
                {
                    strcpy(mode,"1vsai");
                    if(chooseparty(renderer,last_turn)==1)
                    {
                        SDL_DestroyRenderer(renderer);
                        reload_from_ia=1;
                        return 1;
                    }
                    choice=1;
                }
                //1vs1
                if(event.button.x>350 && event.button.x<500 && event.button.y>350 && event.button.y<390)
                {
                    strcpy(mode,"1vs1");printf("\n1vs1");
                    if(chooseparty(renderer,last_turn)==1)
                    {
                        SDL_DestroyRenderer(renderer);
                        reload_from_1vs1=1;
                        return 1;
                    }
                    choice=1;
                }
                if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)//cancel
                {
                    choice=1;
                }
            }
            if(event.type==SDL_QUIT)
            {
                exit(1);
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    return 0;
}

int chooseparty(SDL_Renderer *renderer,int *last_turn)
{
    fond_chooseparty(renderer);
    int choosing=0;
    SDL_Event event;

    party *choice = Malloc(party);
    choice->previous=NULL;
    choice->next=NULL;

    char path[100];
    sprintf(path,"games/%s/%s/game_names.txt",player_name,mode);
    FILE *file=NULL;
    file=fopen(path,"r");
    if(file==NULL)
    {
        printf("\nerreur de lecture");
        return 0;
    }
    if(feof(file))
    {
        return 0;
    }

    fscanf(file,"%s\n",choice->text);
    party *first=choice;
    while(!feof(file))
    {
        party *new_choice=Malloc(party);
        new_choice->next=NULL;
        new_choice->previous=choice;
        fscanf(file,"%s\n",new_choice->text);
        printf("%s\n",choice->text);
        choice->next=new_choice;
        choice=new_choice;
    }
    fclose(file);
    first->previous=choice;
    choice->next=first;
    show(choice->text,renderer);
    while (!choosing)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                //next
                if(event.button.x>445 && event.button.x<505 && event.button.y>335 && event.button.y<385)
                {
                    choice=choice->next;
                    fond_chooseparty(renderer);
                    show(choice->text,renderer);printf("%s",choice->text);
                }
                //previous
                if(event.button.x>95 && event.button.x<145 && event.button.y>335 && event.button.y<385)
                {
                    choice=choice->previous;
                    fond_chooseparty(renderer);
                    show(choice->text,renderer);
                }
                //choice
                if(event.button.x>175 && event.button.x<425 && event.button.y>335 && event.button.y<385)
                {
                    sprintf(path,"games/%s/%s/game-%s.bin",player_name,mode,choice->text);
                    printf("%s",path);
                    file=fopen(path,"rb");
                    if(file==NULL)
                    {
                        printf("file don t exist ");
                        SDL_DestroyRenderer(renderer);
                        free(choice);
                        return -1;
                    }
                    if (!fread(last_turn,1,sizeof(int), file))
                    {
                        printf("%d",*last_turn);
                        printf("\nreading unsuccessful : last turn ");
                        fclose(file);
                        SDL_DestroyRenderer(renderer);
                        free(choice);
                        return -1;

                    }

                    if (!fread(board,1,64*sizeof(int),file))
                    {
                        printf("\nreading unsuccessful : board");
                        fclose(file);
                        SDL_DestroyRenderer(renderer);
                        free(choice);
                        return -1;
                    }
                    strcpy(game_name,choice->text);
                    printf("%s",game_name);
                    free(choice);
                    SDL_DestroyRenderer(renderer);
                    fclose(file);
                    return 1;
                }
                if(event.button.x>525 && event.button.x<545 && event.button.y>155 && event.button.y<175)//cancel
                {
                SDL_DestroyRenderer(renderer);
                free(choice);
                fclose(file);return 0;
                }
            }
            if(event.type==SDL_QUIT)
            {
                free(choice);
                SDL_DestroyRenderer(renderer);
                fclose(file);;
                exit(1);
            }
        }
    }
}


void bestscores(SDL_Window *window)
{
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    fond_historic(renderer);

    SDL_Event event1;

    char name[20],mode[10],path[100],text[100];
    int number,score;

    FILE *file=NULL;

    TTF_Font *font =TTF_OpenFont("Tahoma.ttf",24);
    SDL_Color black ={0,0,0,0};
    SDL_Surface *textsurface;
    SDL_Texture *textu;

    int i=0;
    sprintf(path,"games/%s/bestscores.txt",player_name);
        file=NULL;
        file=fopen(path,"r");
        if (file!=NULL)
        {
            while(!feof(file))
            {
            fscanf(file,"%d. %s %d %s\n",&number,name,&score,mode);
            sprintf(text,"%d. %s %d %s",number,name,score,mode);
            printf("%d. %s %d %s\n",number,name,score,mode);
            SDL_Rect textRect;
                textRect.x=200;
                textRect.y=20+30*i;

            textsurface = TTF_RenderText_Solid(font,text,black);
            textu=SDL_CreateTextureFromSurface(renderer,textsurface);
            SDL_QueryTexture(textu, NULL, NULL, &textRect.w, &textRect.h);
            SDL_RenderCopy(renderer,textu,NULL,&textRect);
            SDL_DestroyTexture(textu);
            SDL_RenderPresent(renderer);
            i++;
            }
            fclose(file);
        }
        i=1;
        while(i)
        {
            while(SDL_PollEvent(&event1))
            {
                switch(event1.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        if(event1.button.x>550 && event1.button.x<570 && event1.button.y>20 && event1.button.y<40)
                        {
                            i=0;
                        }
                        break;
                    case SDL_QUIT :
                        i=0;
                    default : break;
                }
            }
        }
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
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
        *pass_turn=0;
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

void set_color(int pos_l,int pos_c,int player)//coloration
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
        save_history_counter--;
    }
}

void move_forward(board_list **ptr_list_boards,int last_turn)
{
    if((*ptr_list_boards)->next !=NULL)
    {
        (*ptr_list_boards)=(*ptr_list_boards)->next;
        memcpy(board,(*ptr_list_boards)->board_m,sizeof(int)*8*8);
        last_turn=(*ptr_list_boards)->_last_turn;
        save_history_counter++;
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
                    //if(k==2 && strlen(username)==4){username[3]='\0';k++;}
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
    //for each mode creat a folder
    char folder_name[100];
    sprintf(folder_name,"%s/1vs1/",path);
    creat_folder(folder_name);
    sprintf(folder_name,"%s/1vsai/",path);
    creat_folder(folder_name);

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
    strcpy(player_name,username);
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
    //create file
    file=fopen(user_path,"a");
    fclose(file);

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
            show(password,renderer);
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
    strcpy(player_name,username);
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

int save(int *last_turn,SDL_Renderer *renderer)
{
    char * extension=".bin";
    char path[100];
    char path_game_names[100];
    int k=0;
    int running=1,error=0;
    char opening_mode[]="wb";
    FILE* file=NULL;
    FILE* file_text=NULL;

    printf("entrer un nom pour la partie :");
if(reloaddone2==0)
{
    fond_nameparty(renderer);
    do{
        if(error)
        {
            fond_errornameparty(renderer);
            show(game_name,renderer);
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
                        strcat(game_name,"");break;
                    case SDLK_RETURN :
                        running=0;break;
                    case SDLK_BACKSPACE:
                        game_name[strlen(game_name)-1]='\0';
                        break;
                    default:
                        if(strlen(game_name)<7 && (((int)event.key.keysym.sym>=48 &&(int)event.key.keysym.sym<=57)
                           || ((int)event.key.keysym.sym>=97 &&(int)event.key.keysym.sym<=122)))
                        {
                        game_name[strlen(game_name)]=event.key.keysym.sym;
                        if(k==0){game_name[1]='\0';k++;}//some errors
                        if(k==1){game_name[2]='\0';k++;}
                        //if(k==2 && strlen(password)==4){password[3]='\0';k++;}
                        break;}
                        else{break;}
                        }
                    if(error)
                    {
                        fond_errornameparty(renderer);
                    }
                    else{fond_nameparty(renderer);}
                    show(game_name,renderer);
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
        sprintf(path,"games/%s/%s/game-%s%s",player_name,mode,game_name,extension);
        sprintf(path_game_names,"games/%s/%s/game_names.txt",player_name,mode);
        if (!fopen(path,"rb"))
        {
            break;//if the file doesn't exist we will break from the do while
        }
        printf("name already exist : try another party name :");//else we will say to the user that the game already exist
        fond_errornameparty(renderer);
        error=1;
        running=1;
       }while(1);
}

    sprintf(path,"games/%s/%s/game-%s%s",player_name,mode,game_name,extension);
    printf("%s",path);
    file=fopen(path,opening_mode);

    if(file==NULL)
    {
        printf("file didn t open ");
        return -1;//create or recreate the file
    }
if(reloaddone2==0)
    {file_text=fopen(path_game_names,"a");
    if(!fprintf(file_text,"%s\n",game_name))
    {
        printf("\ncouldn't write the party's name");
        return -1;
        fclose(file_text);
    }}

   if (!fwrite(&last_turn,1,sizeof(int),file))
            {
                printf("\n writing unsuccessful : player");
                return -1;
                fclose(file);
            }

    if (!fwrite(board,1,sizeof(int)*64,file))
            {
                printf("\nwriting unsuccessful : board ");
                return -1;
                fclose(file);
            }

    //rename history file
    char rename_history[100];
    sprintf(rename_history,"games/%s/%s/history.txt",player_name,mode);
    FILE *src_file=fopen(rename_history,"r");
    char rename_to[100];
    sprintf(rename_to,"games/%s/%s/history-%s.txt",player_name,mode,game_name);
    FILE *dest_file=fopen(rename_to,"a");
    char carac;
    carac=fgetc(src_file);
    while(carac!=EOF)
    {
        fprintf(dest_file,"%c",carac);
        carac=fgetc(src_file);
    }
    fclose(src_file);
    remove(rename_history);

    /* Fermeture du fichier : */
    reloaddone2=0;
    fclose(dest_file);
    fclose(file_text);
    fclose(file);
    return 1;
}

void save_history (int pos_l,int pos_c,int player)
{
    //Access the file
    char path[100];
    char path2[100];
    FILE *file=NULL;
    FILE *file2=NULL;
    //we get the path
    sprintf(path,"games/%s/%s/history.txt",player_name,mode);//why games ? because the main folder which contain the game is named games
    //we should have a path like :games/1vs1/imad/history-imad&hamza.txt

    Mix_Chunk *win = Mix_LoadWAV("soundtrack/win.wav");

    //open the file
    file=fopen(path,"a");
    fclose(file);
    file=fopen(path,"r+");
    fseek(file,0,SEEK_SET);
    if(file==NULL)
    {
        printf("file didn t open ");
        return -1;//create or recreate the file
    }
    //file tempo that will rename
    sprintf(path2,"games/%s/%s/tempo.txt",player_name,mode);
    file2=fopen(path2,"a");
    int cursor;char move[5];
    for(cursor=0;cursor<save_history_counter;cursor++)
    {
        fscanf(file,"%s",move);
        fprintf(file2,"%s\n",move);
    }

    //save the mouvement
    char mouvement[5];
    if (player==-BLACK)
        {sprintf(mouvement,"B:%c-%d\n",pos_c+65, pos_l);}
    if (player==-WHITE)
        {sprintf(mouvement,"W:%c-%d\n",pos_c+65, pos_l);}
    if(player==0)
    {
        int i,j,black_num=0,white_num=0;
        fprintf(file2,"******\nGAME OVER\n");
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
        {
            fprintf(file2,"******\nWHITE WINS\n");
        }

        else
         {
            if (black_num>white_num)
            {
                Mix_PlayChannel(-1,win,0);
                fprintf(file2,"******\nBLACK WINS\n");
            }
            else
            {
                Mix_PlayChannel(-1,win,0);
                fprintf(file2,"******\nDRAW\n");
            }
        }

        fprintf(file2,"******\nWHITE :%d\nBLACK :%d\n",white_num,black_num);
        best_scores(black_num);
    }

    save_history_counter++;
    //close the file
    fprintf(file2,mouvement);
    fclose(file);
    fclose(file2);
    sprintf(path,"games/%s/%s/history.txt",player_name,mode);
    remove(path);
    sprintf(path2,"games/%s/%s/tempo.txt",player_name,mode);
    rename(path2,path);
    return ;
}

void best_scores(int score_black)
{
    char path[50];
    sprintf(path,"games/%s/bestscores.txt",player_name);
    FILE *file=NULL;
    file=fopen(path,"a");
    fclose(file);
    file=fopen(path,"r");
    if(file==NULL)
    {
        printf("file: bestscores doesn't exist");
        return 0;
    }
    scores_list list[10];
    int number_file;
    int number_total=0;
    char name_file[10];
    char mode_file[10];
    int score_file;
    int score_writed=0;
    char carac;
    carac=fgetc(file);
    fseek(file,0,SEEK_SET);
    if(carac!=EOF)
    {
    while(!feof(file) && number_total<10)
    {
        fscanf(file,"%d. %s %d %s\n",&number_file,name_file,&score_file,mode_file);
        if(score_black>score_file && score_writed==0)
        {
            strcpy(list[number_total].name,player_name);
            list[number_total].score=score_black;
            strcpy(list[number_total].mode,mode);
            score_writed=1;
            number_total++;
            strcpy(list[number_total].name,name_file);
            list[number_total].score=score_file;
            strcpy(list[number_total].mode,mode_file);
        }
        else
        {
            if(score_black==score_file && score_writed==0)
            {
                strcpy(list[number_total].name,player_name);
                list[number_total].score=score_black;
                strcpy(list[number_total].mode,mode);
                score_writed=1;
                number_total++;
                strcpy(list[number_total].name,name_file);
                list[number_total].score=score_file;
                strcpy(list[number_total].mode,mode_file);
            }
            else
            {
                strcpy(list[number_total].name,name_file);
                list[number_total].score=score_file;
                strcpy(list[number_total].mode,mode_file);
            }
        }
        number_total++;
    }
    if(score_writed==0 && number_total<10)
    {
        strcpy(list[number_total].name,player_name);
        list[number_total].score=score_black;
        strcpy(list[number_total].mode,mode);
        score_writed=1;
        number_total++;
    }
    }
    fclose(file);
    printf("%d\n",number_total);
    file=fopen(path,"w+");
    int i;
    for(i=0;i<number_total;i++)
    {
        fprintf(file,"%d. %s %d %s\n",i+1,list[i].name,list[i].score,list[i].mode);
        printf("%d %s %d %s\n",i+1,list[i].name,list[i].score,list[i].mode);
    }
    if(number_total==0)
    {
        fprintf(file,"1. %s %d %s\n",player_name,score_black,mode);
    }
    printf("%d\n",number_total);
    fclose(file);
}


void simple_IA(int *pos_l,int *pos_c)
{
    int list_mouvement[28][2] ;

	int indice_list_AI=0;
	int ai_pos_l,ai_pos_c;
	for (ai_pos_l=0;ai_pos_l<8;ai_pos_l++)
	{
		for (ai_pos_c=0;ai_pos_c<8;ai_pos_c++)
		{
            if (highlight_board[ai_pos_l][ai_pos_c]==2)
            {
                list_mouvement[indice_list_AI][0]=ai_pos_l;
                list_mouvement[indice_list_AI++][1]=ai_pos_c;

            }
		}
	}


	srand( time( NULL ) );

	int choosen=rand()%indice_list_AI;
	//list_mouvement[choosen][0],list_mouvement[choosen][1] is the movement choosen
	*pos_l=list_mouvement[choosen][0];
	*pos_c=list_mouvement[choosen][1];
}

void remove_files()
{
    char path[100],path2[100];
    sprintf(path,"games/%s/%s/history.txt",player_name,mode);
    remove(path);
    sprintf(path,"games/%s/%s/game-%s.bin",player_name,mode,game_name);
    remove(path);
    sprintf(path,"games/%s/%s/history-%s.txt",player_name,mode,game_name);
    remove(path);

    FILE *file,*file2;
    sprintf(path,"games/%s/%s/game_names.txt",player_name,mode);
    file=fopen(path,"w");
    fclose(file);
    file=fopen(path,"r");
    sprintf(path2,"games/%s/%s/game_names_copy.txt",player_name,mode);
    file2=fopen(path2,"a");

    char name[10];
    while(!feof(file))
    {
        fscanf(file,"%s",name);
        if(strcmp(name,game_name)==0)
        {
            printf("name deleted");
        }
        else
        {
            fprintf(file2,"%s\n",name);
        }
    }
    fclose(file);
    fclose(file2);
    sprintf(path,"games/%s/%s/game_names.txt",player_name,mode);
    remove(path);
    sprintf(path2,"games/%s/%s/game_names_copy.txt",player_name,mode);
    rename(path2,path);

    sprintf(path,"games/%s/%s/game_names.txt",player_name,mode);
    file2=fopen(path,"r");
    char carac;
    carac=fgetc(file2);
    if(carac==EOF)
    {
        fclose(file2);
        remove(path);
    }
    else{fclose(file2);}
    strcpy(game_name,"");
    strcpy(mode,"");
    printf("remove files");
}
