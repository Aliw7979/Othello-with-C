#include"header.h"
#include"graphics.h"
#include<stdio.h>
void initial_board(int board[])
{
	int i;
	for (i = 0; i < SQUARES; i++)
		board[i] = 0;
	board[3 * ROWS + 3] = SEFID;
	board[3 * ROWS + 4] = SIAH;
	board[4 * ROWS + 3] = SIAH;
	board[4 * ROWS + 4] = SEFID;
}
char name_of(int piece)
{
	char piecenames[4] = { '.', 'b', 'w', '?' };
	return piecenames[piece];
}

int count(int player, int board[])
{
	int i, cnt;
	cnt = 0;
	for (i = 0; i < SQUARES; i++)
		if (board[i] == player)
			cnt++;
	return cnt;
}

void display_board(int board[])
{

	int c, r;
	int i;
	//displaying the graphical board
	setcolor(YELLOW);
	rectangle(100, 100, 500, 500);
	
	setfillstyle(LTBKSLASH_FILL, MAGENTA);
	floodfill(0, 0, YELLOW);
	//drawing lines
	for (i = 150; i <= 450; i += 50)
		line(100, i, 500, i);
	for (i = 150; i <= 450; i += 50)
		line(i, 100, i, 500);

	printf("  ");
	for (c = 0; c < COLS; c++)


		for (r = 0; r < ROWS; r++)
		{

			for (c = 0; c < COLS; c++)
			{
				//drawing circles
				if (board[r*COLS + c] == 1) {
					setcolor(GREEN);
					circle(175 + 50 * (c - 1), 175 + 50 * (r - 1), 22);
					setfillstyle(SOLID_FILL, GREEN);
					floodfill(170 + 50 * (c - 1), 170 + 50 * (r - 1), GREEN);
				}
				else if (board[r*COLS + c] == 2) {
					setcolor(WHITE);
					circle(175 + 50 * (c - 1), 175 + 50 * (r - 1), 22);
					setfillstyle(SOLID_FILL, WHITE);
					floodfill(175 + 50 * (c - 1), 175 + 50 * (r - 1), WHITE);
				}
			}

		}
	printf("[%c=%d %c=%d]\n", name_of(SIAH), count(SIAH, board), name_of(SEFID), count(SEFID, board));

}

int get_opponent(int player)
{
	switch (player) {
	case SIAH:
		return SEFID;
	case SEFID:
		return SIAH;
	default:
		printf("Illegal player\n");
		return 0;
	}
}

int valid_position(int move)
{
	if (move >= 0 && move < 64)
		return 1;
	else
		return -1;
}

int check_valid_change_row(int source, int destination, int dir)
{
	int source_row = source / COLS;
	int destination_row = destination / COLS;
	if ((dir == UP_LEFT || dir == UP || dir == UP_RIGHT) && (destination_row - source_row != -1))
		return -1;
	else if ((dir == LEFT || dir == RIGHT) && (destination_row - source_row != 0))
		return -1;
	else if ((dir == DOWN_LEFT || dir == DOWN || dir == DOWN_RIGHT) && (destination_row - source_row != 1))
		return -1;
	else
		return 1;
}

int find_bracketing_piece(int square, int player, int board[], int dir)
{
	while (valid_position(square) != -1 &&
		(board[square] == get_opponent(player) && (check_valid_change_row(square, square + dir, dir) != -1)))
	{
		square = square + dir;
	}
	if (valid_position(square) == -1)
		return -1;
	else if (board[square] == player)
		return square;
	else if (check_valid_change_row(square, square + dir, dir) == -1)
		return -1;
	else
		return -1;
}

int would_flip(int move, int player, int board[], int dir)
{
	int c = move + dir;
	if ((valid_position(c) == -1) || (check_valid_change_row(move, c, dir) == -1))
		return -1;
	else if (board[c] == get_opponent(player))
		return find_bracketing_piece(c, player, board, dir);
	else
		return -1;
}

int legal_position(int move, int player, int board[])
{
	int ALLDIRECTIONS[8] = { UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT };
	if (valid_position(move) == -1)
		return -1;
	if (board[move] == EMPTY) {
		int i = 0;
		while (i < 8 && (would_flip(move, player, board, ALLDIRECTIONS[i]) == -1))
			i++;
		if (i == 8)
			return -1;
		else
			return 1;
	}
	else
		return -1;
}

int any_legal_move(int player, int board[])
{
	int move = 0;
	while (move < 64 && (legal_position(move, player, board) == -1))
		move++;
	if (move < 64)
		return 1;
	else
		return -1;
}

void make_flips(int move, int player, int board[], int dir)
{
	int bracketer, c;
	bracketer = would_flip(move, player, board, dir);
	if (bracketer != -1) {
		c = move + dir;
		while (c != bracketer)
		{
			board[c] = player;
			c = c + dir;
			if (valid_position(c) == -1)
				break;
		}
	}
}

void make_move(int move, int player, int board[])
{
	int i;
	int ALLDIRECTIONS[8] = { UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT };
	board[move] = player;
	for (i = 0; i <= 7; i++)
		make_flips(move, player, board, ALLDIRECTIONS[i]);
}

int check_endgame(int player, int board[])
{
	int SIAH_score;
	int SEFID_score;
	if (any_legal_move(player, board) == -1)
	{
		SIAH_score = count(SIAH, board);
		SEFID_score = count(SEFID, board);
		if (SIAH_score > SEFID_score)
			printf("The SIAH player has won!\n");
		else if (SEFID_score > SIAH_score)
			printf("The SEFID player has won!\n");
		else
			printf("The game has no winner\n");
		printf("SIAH: %d\n", SIAH_score);
		printf("SEFID: %d\n", SEFID_score);
		return GAME_FINISHED;
	}
	return GAME_NOT_FINISHED;
}

int get_user_move(int player, int board[])
{    //drawing square selector
	int x1 = 110, y1 = 110, x2 = 140, y2 = 140;
	setcolor(YELLOW);
	rectangle(x1, y1, x2, y2);
	//get user input 
	while (true) {
		char input;
		input = getchar();
		if (input == ' ') {
			break;
		}
		if (input == 'w') {

			board1(x1, y1, board);
			setcolor(YELLOW);
			if ((y1 - 50) < 110) {

				rectangle(x1, y1 += 350, x2, y2 += 350);

				continue;
			}


			rectangle(x1, y1 -= 50, x2, y2 -= 50);

		}
		else if (input == 's') {

			board1(x1, y1, board);
			setcolor(YELLOW);
			if ((y1 + 50) > 460) {

				rectangle(x1, y1 -= 350, x2, y2 -= 350);

				continue;
			}

			rectangle(x1, y1 += 50, x2, y2 += 50);

			continue;
		}
		else if (input == 'd') {

			board1(x1, y1, board);
			setcolor(YELLOW);
			if ((x1 + 50) > 460) {

				rectangle(x1 -= 350, y1, x2 -= 350, y2);

				continue;
			}

			rectangle(x1 += 50, y1, x2 += 50, y2);

			continue;
		}
		else if (input == 'a') {

			board1(x1, y1, board);
			setcolor(YELLOW);
			if ((x1 - 50) < 110) {

				rectangle(x1 += 350, y1, x2 += 350, y2);

				continue;
			}

			rectangle(x1 -= 50, y1, x2 -= 50, y2);

			continue;
		}
	}
	//convert user move to number of square
	int y = ((((x1 + x2) / 2) - 125) / 50) + 1;
	int x = ((((y1 + y2) / 2) - 125) / 50) + 1;
	x *= 10;
	int move = x + y;
	printf("%s player's turn :\n", player == SIAH ? "SIAH" : "SEFID");
	//scanf("%d", &move);
	// scanf_s("%d", &move);
	return move;
}

int convert_user_input_to_board_index(int user_input)
{
	return (((user_input - 1) / 10) - 1) * 8 + ((user_input - 1) % 10);
}

void othello(int board[])
{
	int player = SIAH;
	while (check_endgame(player, board) == GAME_NOT_FINISHED)
	{
		int user_input = get_user_move(player, board);
		int move = convert_user_input_to_board_index(user_input);
		if (legal_position(move, player, board) != -1)
		{
			make_move(move, player, board);
			player = get_opponent(player);
			display_board(board);
		}
		else
			printf("Invalid move. Please choose an appropriate square.\n");
	}
}

//this is graphical board for updating display with every move
void board1(int i, int j, int board[]) {
	int r, c;
	clearviewport();



	setcolor(YELLOW);
	rectangle(100, 100, 500, 500);

	setfillstyle(LTSLASH_FILL, MAGENTA);

	floodfill(0, 0, YELLOW);

	for (i = 150; i <= 450; i += 50)
		line(100, i, 500, i);
	for (i = 150; i <= 450; i += 50)
		line(i, 100, i, 500);
	for (c = 0; c < COLS; c++)

		for (r = 0; r < ROWS; r++)
		{
			for (c = 0; c < COLS; c++)
			{
				if (board[r*COLS + c] == 1) {
					setcolor(GREEN);
					circle(175 + 50 * (c - 1), 175 + 50 * (r - 1), 22);
					setfillstyle(SOLID_FILL, GREEN);
					floodfill(170 + 50 * (c - 1), 170 + 50 * (r - 1), GREEN);
				}
				else if (board[r*COLS + c] == 2) {
					setcolor(WHITE);
					circle(175 + 50 * (c - 1), 175 + 50 * (r - 1), 22);
					setfillstyle(SOLID_FILL, WHITE);
					floodfill(175 + 50 * (c - 1), 175 + 50 * (r - 1), WHITE);
				}
			}

		}

}
