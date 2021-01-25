#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Malloc(type) (type*)malloc(sizeof(type))
#define SIZE_OF_BOARD 64
#define TAILLE_BUF 8

typedef struct _user
{
    char name[20];
    char password[20];
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
int sauvegarde(int *last_turn);
int reload(int *last_turn);

void register(char *name,char *password)
{
    
}
void sign_in(char *name,char *password)
{
    
}
user menu_register()
{
    user user_x;
    printf("do you want to sign in or to register");
    //click at register or sign in : it's name is click
    switch(click)
    {
        case ://register
            printf("entrer your name ");
            do{
            fscanf(stdin,"%s",user_x.name);
            if (!creat_folder(user_x.name))
            {
                printf("name already exist,entrer a new name :");
                continue ;//show him a picture of the erreur
            }
             break;
            }while(1);
            printf("entrer a password ");
            fscanf(stdin,"%s",user_x.password);
            break;
        case ://sign in
            FILE *file;
            char *path=(char *)malloc(100*sizeof(char));
            printf("entrer your name ");
            do{
            fscanf(stdin,"%s",user_x.name);
            strcpy(path,user_x.name);
            strcat(path,"/");
            strcpy(path,user_x.name);
            strcat(path,extension);
            file=fopen(path,"rb");
            if (file==NULL)
            {
                printf("name don't exist,entrer an existing name :");//show him a picture of the erreur
                continue ;
            }
             fread();
             break;
            }while(1);
            
            printf("entrer a password ");
            fscanf(stdin,"%s",user_x.password);
            break;
            


            
    }
    
}
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////7
int sauvegarde(int *last_turn,char * opening_mode="wb")
{
    char * extension=".bin";
    
    char * path;
    char name_game[20];
    FILE* file=NULL;
    
    printf("entrer un nom pour la partie");        
    path=(char *)malloc(strlen(name_game)+strlen(extension)+1);
    
    do{
        scanf("%s",name_game);
        strcpy(path,name_game);
        strcat(path,extension);

        if (!fopen(path,"rb"))
        {
            printf("nom existe deja : entrer un nouveau nom pour la partie :");
        }
       }while(1);

    file=fopen(path,opening_mode);
    if(file==NULL)
    {
        printf("file didn t open ");
        return -1;//create or recreate the file
    }


   if (!fwrite(last_turn,1,sizeof(int),file))
            {
                printf("\n writing unsuccessful : player");
                return -1;
                fclose(file);
            }
    if (!fwrite(board_m,1,sizeof(int)*SIZE_OF_BOARD,file))
            {
                printf("\nwriting unsuccessful : board ");
                return -1;
                fclose(file);
            }
        
        
    /* Fermeture du fichier : */
    fclose(file);
    return 0;
}
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////777
int reload(int * last_turn,char * opening_mode)
{
    char * extension=".bin";
    
    char * path;
    char name_game[20];
    FILE* file=NULL;

    printf("entrer un nom pour la partie");        
    path=(char *)malloc(strlen(name_game)+strlen(extension)+1);
    
    do{
        scanf("%s",name_game);
        strcpy(path,name_game);
        strcat(path,extension);

        if (fopen(path,"rb"))
        {
            printf("nom n'existe pas : entrer un nom pour la partie :");
            continue;
        }
        break;
       }while(1);
    
    file=fopen(path,opening_mode);
    if(file==NULL)
    {
        printf("file don t exist ");
        return -1;
    }

    
    if (!fread(last_turn,1,sizeof(int), file))
        {
            printf("\n reading unsuccessful : last turn ");
            return -1;
            fclose(file);
        }

    if (!fread(board,1,SIZE_OF_BOARD*sizeof(int), file))
        {
            printf("\n reading unsuccessful : board");
            return -1;
            fclose(file);
        }
       
    /* Fermeture du fichier : */
    fclose(file);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
int registerx(char *path,char *name_p,char *password,char *mode)//games_mode_name
{
    //creat folder
    printf("\nhi");
    path=malloc(sizeof(path)+sizeof(mode)+sizeof(name_p)+1);printf("\nhi");
    strcpy(path,"games");
    strcat(path,"/");printf("\%s",path);
    strcat(path,mode);
    strcat(path,"/");

    char user_path[100];
    strcpy(user_path,path);

    strcat(path,name_p);
    printf("hi");
    if(!creat_folder(path))
    {
        printf("name already exist try another name");
        return 0;
    }

    //folder created
    strcat(user_path,"users.txt");
    FILE *file;
    file=fopen(user_path,"a");

    //open the user file
    if (file==NULL)
    {
        printf("could,t open the file ");
        return 0;
    }

    //append the name and the password
    fprintf(file,name_p);
    fprintf(file,"\n");
    fprintf(file,password);
    fclose(file);
    return 1;

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

 

