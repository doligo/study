#include <windows.h>
#include <iostream>
#pragma once

class Move_Character
{
private:
	HDC		MemDC;
	HBITMAP	m_BitMap;
	HBITMAP	m_Old_BitMap;
	BITMAP B_Info;
	SIZE	m_size;
public:
	Move_Character();
	int x;
	int y;
	int jump_y;
	int jump_y_2;
	int jump_x;

	int pre_direct;
	int direct;
	int pose;

	void Init(HWND hWnd, HINSTANCE hInst);
	void Draw(HDC hdc);
	void Jump(int degree);
	void Move_X(int x);
	void Move_Y(int y);

	~Move_Character();
};

