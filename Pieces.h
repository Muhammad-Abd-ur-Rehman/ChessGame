#pragma once
#pragma once
#include <iostream>
using namespace std;
enum COLOR
{
	BLACK, WHITE
};
class Board;
class PIECE
{
protected:
	COLOR color;
	char symbol;
public:
	PIECE(char s, COLOR c);
	char get_Symbol();
	COLOR get_color();
	virtual bool isValidMove(int sr, int sc, int dr, int dc, Board* b) = 0;
	virtual ~PIECE() = 0;
};
class PAWN : public PIECE
{
public:
	PAWN(char s, COLOR c);
	bool isValidMove(int sr, int sc, int dr, int dc, Board* b);
};
class ROOK : public PIECE
{
public:
	ROOK(char s, COLOR c);
	bool isValidMove(int sr, int sc, int dr, int dc, Board* b);
};
class KNIGHT : public PIECE
{
public:
	KNIGHT(char s, COLOR c);
	bool isValidMove(int sr, int sc, int dr, int dc, Board* b);
};
class BISHOP : public PIECE
{
public:
	BISHOP(char s, COLOR c);
	bool isValidMove(int sr, int sc, int dr, int dc, Board* b);
};
class QUEEN : public PIECE
{
public:
	QUEEN(char s, COLOR c);
	bool isValidMove(int sr, int sc, int dr, int dc, Board* b);
};
class KING : public PIECE
{
public:
	KING(char s, COLOR c);
	bool isValidMove(int sr, int sc, int dr, int dc, Board* b);
};