#include <iostream>
#include <windows.h>
#include <string.h>
#pragma once
using namespace std;

struct Piece
{
	int x;
	int y;
};

class chess_pieces
{
private:
	Piece m_King;
	Piece m_Queen;
	Piece m_Rook[2];
	Piece m_Bishop[2];
	Piece m_Knight[2];
	Piece m_Pawn[8];

	HDC MemDC;
	HBITMAP hbtmap, old_hbtmap;
	BITMAP btmap;
public:
	chess_pieces();

	int m_x;
	int m_y;
	void Init_pieces(HDC hdc, int player_num);
	void Pieces_Draw(HDC hdc, int x, int y);

	~chess_pieces();
};

