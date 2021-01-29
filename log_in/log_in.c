void log_in(char *path,char *mode)
{
    char password[30];
    char name[20];
    printf("entrer your name and your password :");
    fscanf(stdin, "%s %s",name,password);

    path=malloc(sizeof(path)+sizeof(mode)+sizeof(name)+1);
    strcpy(path,"games");
    strcat(path,"/");
    strcat(path,mode);
    strcat(path,"/");

    char user_path[100];
    strcpy(user_path,path);

    strcat(path,name);

    //folder created
    strcat(user_path,"users.txt");
    FILE *file;
    file=fopen(user_path,"r");
    int found=0;

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
        printf("log in succesfully");
    }
    else
        printf("username or password incorecte");


}
