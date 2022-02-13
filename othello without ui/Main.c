#include "Header.h"

int main()
{
	int board[SQUARES];
	initial_board(board);
	display_board(board);
	othello(board);
	return 0;
}