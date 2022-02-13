#include"graphics.h"
#include"header.h"

int main()
{
	int board[SQUARES];
	initial_board(board);
	//create a window for graphical show
	int wid = ALL_WINDOWS;
	initwindow(650, 600, "Othello");
	display_board(board);
	othello(board);
	delay(1000000000);
	return 0;
}