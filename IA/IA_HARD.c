#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BLACK 1
#define WHITE  -1
#define infinity 1000

int nmbr_mouvement=0;
int indice_list_AI=0;

typedef struct _list_mouvement
{
	int list_mouvement_possible[28][2]={-1,-1};//moves can't be higher than 28 https://puzzling.stackexchange.com/questions/31896/othello-most-number-of-legal-moves-in-a-given-turn
	int indice_list_possible=0;
}list_mouvement;

typedef struct _evaluation
{
	int _eval;
	int coord[2];

}eval;

int main()
{

	list_mouvement L;
	int ai_pos_l,ai_pos_c;
	for (ai_pos_l=0;ai_pos_l<8;ai_pos_l++)
		for (ai_pos_c=0;ai_pos_c<8;ai_pos_c++)
	{
		if (board[ai_pos_l][ai_pos_c]==2)
		{
			L.list_mouvement_AI[L.indice_list_AI][0]=ai_pos_l;
			L.list_mouvement_AI[L.indice_list_AI++][1]=ai_pos_c;

		}
	}

	indice_list_AI--;
	for (;indice_list_AI>=0;indice_list_AI--)
		printf("%d,%d\n",list_mouvement_AI[indice_list_AI][0],list_mouvement_AI[indice_list_AI][1]);
}


list_mouvement AI()//determine all the possible mouvement
{
	list_mouvement L;
	int ai_pos_l,ai_pos_c;
	for (ai_pos_l=0;ai_pos_l<8;ai_pos_l++)
		for (ai_pos_c=0;ai_pos_c<8;ai_pos_c++)
	{
		if (highlight_board[ai_pos_l][ai_pos_c]==2)
		{
			L.list_mouvement_possible[L.indice_list_possible][0]=ai_pos_l;
			L.list_mouvement_possible[L.indice_list_possible++][1]=ai_pos_c;
		}
	}

	L.indice_list_possible--;
	return L;
}


int evaluation()
{
	int i,j,sum=0;
	for (i=0;i<8;i++)
		for (j=0;j<8;j++)
			sum+=board[i][j];
	return sum;
}

	     //board           //4  -inf +inf	   //0 or 1
eval minimax(int board[8][8],int depth,int alpha,int beta,int maximizingPlayer)
{
	if (depth == 0 || last_turn==0)
	{
		eval maxeval;
		maxeval._eval=evaluation();
		return maxeval;
	}
	board_list *list_boards;
	list_board=list_board_global;
	if (maximizingPlayer)
	{//black
		eval maxEval ;
		maxEval._eval=-infinity;
		//make the possible moves
		list_mouvement L=AI();//we are using Highlight_board

		for (;L.indice_list_possible>=0;L.indice_list_possible--)
		{
			//board
			//play the move on the board
			/////////////////probleme can be here 
			free_ptr(&list_boards);
			set_color(L.list_mouvement_possible[L.indice_list_possible][0],L.list_mouvement_possible[L.indice_list_possible][1],BLACK);
			update_list_board(&list_boards,last_turn);
			reset_h();
			update_list_board(&list_boards,BLACK);//list board

			eval eval = minimax(board, depth - 1, alpha, beta, 0);
			
			
			///////////////////////or here 
			//cancel the move
			move_backward(&list_boards,last_turn);
			reset_h();//reset__highlight

			if (maxEval._eval<eval._eval)
			{
				maxEval._eval=eval;
				maxEval.coord[0]=L.list_mouvement_possible[L.indice_list_possible][0];
				maxEval.coord[1]=L.list_mouvement_possible[L.indice_list_possible][1];
			}
			alpha = max(alpha,eval._eval);
			if (beta <= alpha)
				break;
		}
		return maxEval;
	}
	else
	{
		eval minEval ;
		minEval._eval=+infinity;
		//make the possible moves
		list_mouvement L=AI();
		for (;L.indice_list_possible>=0;L.indice_list_possible--)
		{
			//board
			//play the move on the board
			free_ptr(&list_boards);
			set_color(L.list_mouvement_possible[L.indice_list_possible][0],L.list_mouvement_possible[L.indice_list_possible][1],WHITE);
			update_list_board(&list_boards,last_turn);
			reset_h();
			update_list_board(&list_boards,WHITE);//list board

			eval eval = minimax(board, depth - 1, alpha, beta, 1);

			//cancel the move
			move_backward(&list_boards,last_turn);
			reset_h();//reset__highlight

			if (minEval._eval>eval._eval)
			{
				minEval._eval=eval;
				minEval.coord[0]=L.list_mouvement_possible[L.indice_list_possible][0];
				minEval.coord[1]=L.list_mouvement_possible[L.indice_list_possible][1];
			}
			beta = min(beta,eval._eval);
			if (beta <= alpha)
				break;
		}return minEval
	}
}
int min (int a,int b)
{
	if (a>b)
		return b;
	return a;
}
int max (int a,int b)
{
	if (a<b)
		return b;
	return a;
}

