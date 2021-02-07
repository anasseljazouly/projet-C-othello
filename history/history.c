





//this function should be add in evry move 
//**GLOBAL
char game_name[20];
char mode[6];
char player_name[20];
int save_history_counter=0;

void save_history (int pos_l,int pos_c,int player,)
{
    //Access the file
    char *path[100];
    FILE *file=NULL;

    //we get the path 
    sprintf(path,"games/%s/%s/history-%s.txt",player_name,mode,game_name);//why games ? because the main folder which contain the game is named games
    //we should have a path like :games/1vs1/imad/history-imad&hamza.txt
    
    //open the file
    file=fopen(path,"a");
    fseek(file,-save_history_counter*5,SEEK_END);
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
        
	increment_decrement(1);
    //close the file
    fprintf(file,mouvement);
    fclose(file);
    return ;
}


//in move backward area we should add 
//in move backward increment_decrement(0)
//in move forward increment_decrement(1)

void increment_decrement(int i)
{
	if (save_history_counter==0 )
		if(i)
			save_history_counter++;
		return;
	if(i)
		save_history_counter++;
	else
		save_history_counter--;
	return; 
		
}




void reload_history ()
{
    //Access the file
    char *path[100];
    FILE *file=NULL;
	
	//ask them to give you the game name
	fscanf(stdin,"%s",game_name);
	
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

    char buffer[6];
    //save the mouvement 
    while(!feof(file))
    {
        fgets(buffer,5,file);
        strcat(buffer,'\0');
        
        //***************put your code here***************
        //show the buffer in SDl
    }

    //close the file
    fprintf(file,mouvement);
    fclose(file);
    return ;
}

