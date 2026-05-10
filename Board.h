#pragma once
#include "Pieces.h"
class Board
{
private:
	PIECE* board[8][8];
	int size;
public:
	Board();
	void setupBoard();
	void displayBoard();
	void movepiece(int sr, int sc, int dr, int dc);
	bool is_Path_Clear(int sr, int sc, int dr, int dc);
	bool is_King_Alive();
	void find_King(COLOR c, int& row, int& col);
	bool is_Check(COLOR c);
	bool is_Check_Mate(COLOR c);
	bool check_safety(int sr, int sc, int dr, int dc, COLOR c);
	PIECE* get_piece(int r, int c);
	~Board();
};