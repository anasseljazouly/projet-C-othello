int save(int *last_turn, char *mode,char *player_name)
{
    char * extension=".bin";
    char path[100];
    char game_name[20];

    char opening_mode[]="wb";
    FILE* file=NULL;

    printf("entrer un nom pour la partie :");

    do{

        fscanf(stdin,"%s",game_name);
        sprintf(path,"games/%s/%s/game-%s%s",player_name,mode,game_name,extension);

        if (!fopen(path,"rb"))
        {
            break;//if the file doesn't exist we will break from the do while
        }
        printf("name already exist : try another party name :");//else we will say to the user that the game already exist

       }while(1);


    file=fopen(path,opening_mode);

    if(file==NULL)
    {
        printf("file didn t open ");
        return -1;//create or recreate the file
    }

   if (!fwrite(&last_turn,1,sizeof(int),file))
            {
                printf("\n writing unsuccessful : player");
                return -1;
                fclose(file);
            }

    if (!fwrite(board,1,sizeof(int)*SIZE_OF_BOARD,file))
            {
                printf("\nwriting unsuccessful : board ");
                return -1;
                fclose(file);
            }
    
    //if we want to add something CRTL-C CTRL-V the previous if 


    /* Fermeture du fichier : */
    fclose(file);
    return 0;
}
