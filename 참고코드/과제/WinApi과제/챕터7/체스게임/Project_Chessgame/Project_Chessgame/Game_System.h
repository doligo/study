#include "Board.h"
#pragma once

class Game_System
{
private:
	Board *m_bd;
public:
	Game_System();

	void Init_System(HDC hdc, HINSTANCE hinst);
	void Draw(HDC hdc);

	~Game_System();
};

