#define EMPTY 0
#define SIAH 1
#define SEFID 2
#define OUTER 3

#define ROWS 8
#define COLS 8
#define SQUARES 64

#define UP_LEFT -9
#define UP -8
#define UP_RIGHT -7
#define LEFT -1
#define RIGHT 1
#define DOWN_LEFT 7
#define DOWN 8
#define DOWN_RIGHT 9

#define GAME_FINISHED 1
#define GAME_NOT_FINISHED -1

void initial_board(int board[]);
char name_of(int piece);
int count(int player, int board[]);
void display_board(int board[]);
int get_user_move(int player, int board[]);
int convert_user_input_to_board_index(int user_input);
int get_opponent(int player);
int valid_position(int move);
int check_valid_change_row(int source, int destination, int dir);
int find_bracketing_piece(int square, int player, int board[], int dir);
int would_flip(int move, int player, int board[], int dir);
int legal_position(int move, int player, int board[]);
int any_legal_move(int player, int board[]);
void make_flips(int move, int player, int board[], int dir);
void make_move(int move, int player, int board[]);
int check_endgame(int player, int board[]);
void othello(int board[]);

void board1(int x1, int y1, int board[]);
