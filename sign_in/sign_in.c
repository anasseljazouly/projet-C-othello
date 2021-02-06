int key=17;


/*
sign_in: takes just the mode 1vs1 or 1vsia
it returns 1 if the operation is succesful and 0 of not

it creats a folder for the user
it puts the name and the password in the users.txt file

*/

int sign_in(char *mode)//games_mode_name
{
    //creat folder
    char password[20];
    char name[20];
    char path[100];
    char user_path[100];

    do{

    sprintf(path,"games/%s/",mode);
    strcpy(user_path,path);

    printf("\nentrer your name and your password :");
    fscanf(stdin,"%s%s",name,password);

    strcat(path,name);
    if(!creat_folder(path))
    {
        printf("\nname already exist try another name");
        continue;
    }
    break;

    }while(1);

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
    fprintf(file,"\n");

    fclose(file);
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
