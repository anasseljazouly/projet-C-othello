int main()
{

	int list_mouvement[28][2] ;

	int indice_list_AI=0;
	int ai_pos_l,ai_pos_c;
	for (ai_pos_l=0;ai_pos_l<8;ai_pos_l++)
	{
		printf("X");
		for (ai_pos_c=0;ai_pos_c<8;ai_pos_c++)
		{ printf("Y");

		if (highlight_board[ai_pos_l][ai_pos_c]==2)
		{
			list_mouvement[indice_list_AI][0]=ai_pos_l;
			list_mouvement[indice_list_AI++][1]=ai_pos_c;

		}
		}

	}


	srand( time( NULL ) );

	int choosen=rand()%indice_list_AI;
	//list_mouvement[choosen][0],list_mouvement[choosen][1] is the movement choosen 
	printf("%d,%d",list_mouvement[choosen][0],list_mouvement[choosen][1]);

}
