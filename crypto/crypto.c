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
