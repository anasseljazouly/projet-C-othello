int key=17;



int sign_in(char *path,char *mode)//games_mode_name
{
    //creat folder
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

    code(name,key);
    code(password,key);
    //append the name and the password
    fprintf(file,name);
    fprintf(file,"\n");
    fprintf(file,password);
    fprintf(file,"\n////////////////////////////////////////////////////////\n");

    fclose(file);printf("\ndone");
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
