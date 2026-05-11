#include "Game.h"

void GAME::Input_convert(string s, int& row, int& col)
{
	row = 8 - (s[1] - '0');
	col = s[0] - 'a';
}

void GAME::switch_Turn()
{
	if (current_turn == WHITE)
	{
		current_turn = BLACK;
	}
	else
	{
		current_turn = WHITE;
	}
}

bool GAME::is_Valid_Input(string s)
{
	if (s.length() != 2)
	{
		return false;
	}
	if ((s[0] >= 'a' && s[0] <= 'h') && (s[1] >= '1' && s[1] <= '8'))
	{
		return true;
	}
	return false;
}

bool GAME::is_Current_Player_Piece(int r, int c)
{
	if (b.get_piece(r, c) == nullptr)
	{
		return false;
	}
	return b.get_piece(r, c)->get_color() == current_turn;
}