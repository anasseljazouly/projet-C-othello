#include<stdio.h>

int code(char *str,int key)
{
	char ch;
	int i;


	for(i = 0; str[i] != '\0'; ++i){
		ch = str[i];

		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;

			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}

			str[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;

			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}

			str[i] = ch;
		}
	}

	return 0;
}

int decode(char *str,int key)
{
	char ch;
	int i;


	for(i = 0; str[i] != '\0'; ++i){
		ch = str[i];

		if(ch >= 'a' && ch <= 'z'){
			ch = ch - key;

			if(ch < 'a'){
				ch = ch + 'z' - 'a' + 1;
			}

			str[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch - key;

			if(ch < 'A'){
				ch = ch + 'Z' - 'A' + 1;
			}

			str[i] = ch;
		}
	}


	return 0;
}
