#pragma once
#include "Board.h"
class GAME
{
	Board b;
	COLOR current_turn = WHITE;
public:
	void switch_Turn();
	void Input_convert(string input, int& row, int& col);
	bool is_Valid_Input(string s);
	bool is_Current_Player_Piece(int r, int c);
};