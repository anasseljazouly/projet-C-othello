




//this function should be add in evry move 


void save_history (int pos_l,int pos_c,int player,char *player_name,char *game_name,char *mode)
{
    //Access the file
    char *path[100];
    FILE *file=NULL;

    //we get the path 
    sprintf(path,"games/%s/%s/history-%s.txt",player_name,mode,game_name);//why games ? because the main folder which contain the game is named games
    //we should have a path like :games/1vs1/imad/history-imad&hamza.txt
    
    //open the file
    file=fopen(path,"a");
    if(file==NULL)
    {
        printf("file didn t open ");
        return -1;//create or recreate the file
    }

    //save the mouvement 
    char *mouvement[5];
    if (player==BLACK)
        sprintf(mouvement,"B %c-%d\n",pos_c+65, pos_l);
    if (player==WHITE)
        sprintf(mouvement,"W %c-%d\n",pos_c+65, pos_l);

    //close the file
    fprintf(file,mouvement);
    fclose(file);
    return ;
}


