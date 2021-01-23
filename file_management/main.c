#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Malloc(type) (type*)malloc(sizeof(type))
#define SIZE_OF_BOARD 64
#define TAILLE_BUF 8

typedef struct _user
{
    char name[20];
    char name_oponent[20];
}user;

typedef struct _board_list
{
    int _last_turn;
    struct _board_list *previous;
    int board_m[8][8];
    struct _board_list *next;
}board_list;

int board[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,0,1,1,0,0,0},//3
                 {0,0,0,1,1,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7*/

int board2[8][8]={{0,0,0,0,0,0,0,0}, //0
                 {0,0,0,0,0,0,0,0}, //1
                 {0,0,0,0,0,0,0,0}, //2
                 {0,0,5,1,1,0,0,0},//3
                 {0,0,0,1,1,0,0,0},//4
                 {0,0,0,0,0,0,0,0}, //5
                 {0,0,0,0,0,0,0,0}, //6
                 {0,0,0,0,0,0,0,0}};//7*/



//prototype 

void creat_folder(char *dirname);
int sauvegarde(user user_x,board_list *list_boards);
int reload(user user_x,board_list *list_boards);


int main()
{
    //just for testing
    board_list *list_boards=Malloc(board_list);
    int last_turn=1;

    list_boards->_last_turn=last_turn;
    list_boards->previous=NULL;
    list_boards->next=NULL;
    memcpy(list_boards->board_m,board,sizeof(int)*8*8);

    list_boards->previous=Malloc(board_list);

    list_boards->_last_turn=-last_turn;
    list_boards->previous->previous=NULL;
    list_boards->previous->next=list_boards->previous;
    memcpy(list_boards->previous->board_m,board2,sizeof(int)*8*8);

    user user_x;//=(user *)malloc(sizeof(user));
    strcpy(user_x.name,"test");

    printf("\n%d",sauvegarde(user_x,list_boards));
    printf("\n%d",reload(user_x,list_boards));
}

int sauvegarde(user user_x,board_list *list_boards)
{
    char * extension=".bin";
    char * opening_mode="wb";
    char * path;
    FILE* file=NULL;

    path=(char *)malloc(strlen(user_x.name)+strlen(extension)+1);
    strcpy(path,user_x.name);
    strcat(path,extension);
    printf("%s",path);

    if (!fopen(path,"rb"))
    {
        printf("file already exist it will be deleated");//we can make something else
        remove(path);                        //delete the file
    }

    file=fopen(path,opening_mode);
    if(file==NULL)
    {
        printf("file didn t open ");
        return -1;//create or recreate the file
    }


    while(list_boards)//writing patern player-board in loop
    {
        if (!fwrite(&list_boards->_last_turn,1,sizeof(int),file))
            {
                printf("\n writing unsuccessful : player");
                return -1;
            }

        if (!fwrite(&list_boards->board_m,1,sizeof(int)*SIZE_OF_BOARD,file))
            {
                printf("\nwriting unsuccessful : board ");
                return -1;
            }
        
        list_boards=list_boards->previous ;

    }
    fclose(file);
    return 0;
}
int reload(user user_x,board_list *list_boards)
{
    char * extension=".bin";
    char * opening_mode="rb";
    char * path;
    board_list *nex=NULL;
    int recover;
    FILE* file=NULL;


    int  nb_val_lues = SIZE_OF_BOARD ;

    path=(char *)malloc(strlen(user_x.name)+strlen(extension)+1);
    strcpy(path,user_x.name);
    strcat(path,extension);
    printf("\n%s",path);

    file=fopen(path,opening_mode);
    if(file==NULL)
    {
        printf("file don t exist ");
        return -1;
    }

/////////////////////////////////////////////////////////////////////////////////////////
    while (!feof(file)) /* vrai tant que fin du fichier non atteinte */
    {


        printf("\nim heere\n");
        if (!fread(&recover,1,sizeof(int), file))
        {
            return -2;
        }
        else
            {list_boards->_last_turn=recover;
            printf("\n reading successful : last turn %d",list_boards->_last_turn);
            }
        if(!(fread(&list_boards->board_m,1,sizeof(int)*SIZE_OF_BOARD, file)))
        {
            printf("\nwriting unsuccessful : board ");

        }
        list_boards->next=nex;
        nex=list_boards;
        list_boards->previous=NULL;
        list_boards=list_boards->previous ;
        list_boards=Malloc(board_list);
        list_boards->next=nex;
    }
    /* Fermeture du fichier : */


    return 0;
}



void creat_folder(char *dir_name)
{
   int check;

   check = mkdir(dir_name);
   //checking if directory is created
   if (!check)
      printf("Directory created\n");
   else {
      printf("Unable to create directory\n");
      exit(1);
   }
}



