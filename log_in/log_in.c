
/*

log_in: takes just the mode 1vs1 or 1vsia
it's an infinite loop which we can make it stop using sdl (we should add it later)
demands to the user the name and the password and search for this combinaison in the users.txt file 

*/

void log_in(char *mode)
{
    char password[30];
    char name[20];
    char path[100];
    char user_path[100];

    do{

    sprintf(path,"games/%s/",mode);
    strcpy(user_path,path);

    printf("\nentrer your name and your password :");
    fscanf(stdin,"%s%s",name,password);

    strcat(path,name);

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

        if (!strcmp(name_u,name))
            if (!strcmp(password_u,password))
            {
                found=1;
                break;
            }
    }
    if (found)
    {
        printf("log-in succesfully");
        break;
    }
    else
        printf("username or password incorecte");

    }while(1);


}
