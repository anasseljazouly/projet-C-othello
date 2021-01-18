#include <stdio.h>
#include <stdlib.h>
typedef struct _user
{
    char name[20];

}user;
void sauvegarde(user user_x,int player ,board_list *cancel_boards)
{
    char *extension=".bin",*opening_mode="wb",*begin=user_x->name,*save_file=NULL;
    FILE* file=NULL;
    save_file=strcat(begin,extension);
    if (!fopen(save_file,"rb"))
    {
        fprintf( stderr, "file already exist\n" );//we can make something else
        remove(save_file);                        //delete the file
    }
    file=fopen(save_file,opening_mode);           //create or recreate the file

    fwrite(player,1,1,save_file);
    while(cancel_boards)
    {
    if (1!=fwrite(cancel_board,sizeof(board_list),1,save_file))
    {
        fprintf( stderr, "Cannot write this block in file\n" );
        cancel_boards=cancel_boards->previous ;
    }
    fclose(save_file);
}

