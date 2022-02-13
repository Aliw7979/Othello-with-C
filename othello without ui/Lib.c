#include <stdio.h>
#include "Header.h"

void initial_board(int board[])
{
	int i;
	for (i = 0; i < SQUARES; i++)
		board[i] = 0;
	board[3 * ROWS + 3] = WHITE;
	board[3 * ROWS + 4] = BLACK;
	board[4 * ROWS + 3] = BLACK;
	board[4 * ROWS + 4] = WHITE;
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
	printf("  ");
	for (c = 0; c < COLS; c++)
		printf("  %d", c+1);
	printf("\n");
	for (r = 0; r < ROWS; r++)
	{
		printf("%d ", 10 * (r + 1));
		for (c = 0; c < COLS; c++)
		{
			printf(" %c ", name_of(board[r * COLS + c]));
		}
		printf("\n");
	}
	printf("[%c=%d %c=%d]\n", name_of(BLACK), count(BLACK, board), name_of(WHITE), count(WHITE, board));
	printf("\n");
}

int get_opponent(int player)
{
	switch (player) {
		case BLACK:
			return WHITE;
		case WHITE:
			return BLACK;
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
	int source_row = source/COLS;
	int destination_row = destination/COLS;
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
		(board[square] == get_opponent(player) && (check_valid_change_row(square, square+dir, dir) != -1)))
	{
		square = square + dir;
	}
	if (valid_position(square) == -1)
		return -1;
	else if (board[square] == player)
		return square;
	else if (check_valid_change_row(square, square+dir, dir) == -1)
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
	int ALLDIRECTIONS[8] = {UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT};
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
	int ALLDIRECTIONS[8] = {UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT};
	board[move] = player;
	for (i = 0; i <= 7; i++)
		make_flips(move, player, board, ALLDIRECTIONS[i]);
}

int check_endgame(int player, int board[])
{
	int black_score;
	int white_score;
	if (any_legal_move(player, board) == -1)
	{
		black_score = count(BLACK, board);
		white_score = count(WHITE, board);
		if (black_score > white_score)
			printf("The BLACK player has won!\n");
		else if (white_score > black_score)
			printf("The WHITE player has won!\n");
		else
			printf("The game has no winner\n");
		printf("Black: %d\n", black_score);
		printf("White: %d\n", white_score);
		return GAME_FINISHED;
	}
	return GAME_NOT_FINISHED;
}

int get_user_move(int player)
{
	int move;
	printf("%s player's turn :\n", player == BLACK ? "BLACK" : "WHITE");
	scanf("%d", &move);
	// scanf_s("%d", &move);
	return move;
}

int convert_user_input_to_board_index(int user_input)
{
	return (((user_input - 1) / 10) - 1) * 8 + ((user_input - 1) % 10);
}

void othello(int board[])
{
	int player = BLACK;
	while (check_endgame(player, board) == GAME_NOT_FINISHED)
	{
		int user_input = get_user_move(player);
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