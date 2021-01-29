int reload(int *last_turn,char *mode,char *player_name)
{
    char * extension=".bin";
    char path[100];
    char game_name[20];

    char opening_mode[]="rb";
    FILE* file=NULL;

    printf("entrer un nom pour la partie :");

    do{

        fscanf(stdin,"%s",game_name);
        sprintf(path,"games/%s/%s/game-%s%s",mode,player_name,game_name,extension);


        if (!fopen(path,"rb"))
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
            printf("%d",*last_turn);
            printf("\nreading unsuccessful : last turn ");
            fclose(file);
            return -1;

        }

    if (!fread(board,1,SIZE_OF_BOARD*sizeof(int), file))
        {
            printf("\nreading unsuccessful : board");
            fclose(file);
            return -1;

        }

    /* Fermeture du fichier : */
    fclose(file);
    return 0;
}
