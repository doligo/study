#pragma once
#include "GlobalDefine.h"
#include "JEngine.h"
#include "TimeManager.h"
#include "Label.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>

#define BULLET_MAX 15
#define STAR_MAX 12
#define RANK_MAX 10

enum dir {
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT
};

struct Bullet_Info
{
	JEngine::RECT b_rt;
	int b_dir1;
	int b_dir2;
	int b_speed;
};

struct Star_Info
{
	JEngine::RECT s_rt;
	int s_dir1;
	int s_dir2;
	int s_speed;
};

class Game2_Scene : public JEngine::Scene
{
private:
	JEngine::BitMap *m_pBack;
	JEngine::BitMap *m_pShow_Time;
	JEngine::BitMap *m_pShow_Fever[3];
	JEngine::BitMap *m_pFlight;
	JEngine::BitMap *m_pBullet;
	JEngine::BitMap *m_pExplosion[3];
	JEngine::BitMap *m_pStar[3];
	JEngine::Label *m_pShow_Score;
	JEngine::Label *m_pShow_Star_Score[STAR_MAX];
	JEngine::BitMap *m_pFever_Effect;
	JEngine::BitMap *m_pTimeOut;
	JEngine::BitMap *m_pLoadingBack;
	JEngine::BitMap *m_pLoading;
	JEngine::BitMap *m_pLoadingWord;
	JEngine::Label *m_pShow_Rank[10];
	JEngine::BitMap *m_pRankBack;

	bool player_alive;
	bool game_start;
	float time;
	float game_time;
	float timeover_time;
	float loading_time;
	float b_create_time;
	float s_create_time;
	float s_create_time2;
	RECT tmp_clip;

	JEngine::RECT m_pFlight_Rt;
	JEngine::POINT m_pFlight_Pt;

	Bullet_Info *bullet[BULLET_MAX];
	int explosion_count;

	Star_Info *star[STAR_MAX];

	int star_lv;
	int star_score;
	int game_score;
	int fever_full_check;
	float fever_gauge;
	int fever_light;
	float full_fever_time;
	int combo_count;

	bool show_result;
	bool rank_update;
	vector<int> score;
	int ranking[RANK_MAX];
public:
	Game2_Scene();

	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
	//bool ClickEvent();

	void Set_Flight();
	void Set_Bullet();
	void Set_Dir(int num, int start_point, int dir, int bullet_or_star);
	void Out_of_Map(int num);
	void Bullet_Collision();
	void Player_Alive_Check();
	void Set_Star();
	void Set_Fever();
	void Set_Score();
	void Eat_Star();
	void Time();
	void Set_Rank();
	void Set_Show_Rank();
	bool BackTo_Game();

	virtual ~Game2_Scene();
};

