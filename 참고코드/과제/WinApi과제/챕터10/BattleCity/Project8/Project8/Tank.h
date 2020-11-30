#include <Windows.h>
#include <iostream>
#include <string.h>
#pragma once

#define UP 11
#define DOWN 12
#define LEFT 13
#define RIGHT 14

#define DEAD 0
#define ALIVE 1

class Tank
{
private:
	bool status;
	int m_ix;
	int m_iy;
	int m_pre_x;
	int m_pre_y;
	int direct;
	int player;
	int shield;
	int motion;

public:
	Tank();

	int player_start_x;
	int player_start_y;
	int enemy_start_x;
	int enemy_start_y;
	int move_count;

	void Init_Tank(int player_or_enemy);
	void Moveing(int dir);
	int Get_Tank_X();
	int Get_Tank_Y();
	int Get_Tank_Direct();
	int Get_Tank_Motion();
	int Get_Player_Check();
	int Get_Status();
	void Set_Status(int value);
	void RollBack_pos();
	void Set_Tank_X(int num);
	void Set_Tank_Y(int num);
	bool Get_Tank_Shield();
	void Set_Tank_Shield(int value);

	~Tank();
};

