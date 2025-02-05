#include "Game_Scene.h"
#include "InputManager.h"
#include "ResoucesManager.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "BitMap.h"

Game_Scene::Game_Scene()
{
}

void Game_Scene::Init(HWND hWnd)
{
	ifstream Load;
	ofstream Save;
	int count = 0;

	JEngine::InputManager::GetInstance()->Clear();
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_ESCAPE);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	JEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);

	m_pBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("ColoredPaperBack.bmp");
	m_pPaper[RED] = JEngine::ResoucesManager::GetInstance()->GetBitmap("ColoredPaperRed.bmp");
	m_pPaper[GREEN] = JEngine::ResoucesManager::GetInstance()->GetBitmap("ColoredPaperGreen.bmp");
	m_pPaper[BLUE] = JEngine::ResoucesManager::GetInstance()->GetBitmap("ColoredPaperBlue.bmp");
	m_pPaper[YELLOW] = JEngine::ResoucesManager::GetInstance()->GetBitmap("ColoredPaperYellow.bmp");
	m_pShow_Score = new JEngine::Label();
	m_pShow_Paper_Score = new JEngine::Label();
	m_pShow_Time = JEngine::ResoucesManager::GetInstance()->GetBitmap("ColoredPaperTimeBar.bmp");
	m_pShow_Fever[0] = JEngine::ResoucesManager::GetInstance()->GetBitmap("Fever1.bmp");
	m_pShow_Fever[1] = JEngine::ResoucesManager::GetInstance()->GetBitmap("Fever2.bmp");
	m_pShow_Fever[2] = JEngine::ResoucesManager::GetInstance()->GetBitmap("Fever3.bmp");
	m_pLoadingBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("LoadingBack.bmp");
	m_pLoading = JEngine::ResoucesManager::GetInstance()->GetBitmap("Loading.bmp");
	m_pLoadingWord = JEngine::ResoucesManager::GetInstance()->GetBitmap("Loading_Word.bmp");
	m_pTimeOut = JEngine::ResoucesManager::GetInstance()->GetBitmap("TimeOver.bmp");
	m_pRankBack = JEngine::ResoucesManager::GetInstance()->GetBitmap("RankBack.bmp");
	for (int i = 0; i < 10; i++)
		m_pShow_Rank[i] = new JEngine::Label();

	time = 0;
	game_score = 0;
	paper_score = 100;
	paper_dir = NULL;
	moving_check = false;
	fever_lv = 0;
	fever_gauge = 0;
	combo_count = 0;
	full_fever_time = 0;
	fever_light = 0;

	paper_x = 155;
	paper_y = 300;

	for (int i = 0; i < 2; i++)
		visible_paper[i] = rand() % 4;

	game_time = 45000 + GetTickCount();
	loading_time = 2300 + GetTickCount();
	timeover_time = 0;
	show_result = false;
	rank_update = false;

	score.assign(10, 0);

	for (int i = 0; i < RANK_MAX; i++)
		ranking[i] = 0;

	Load.open("Rank1.txt");
	if (!Load) // 파일없으면 생성
	{
		Save.open("Rank1.txt", ios::app);
		Save.close();
	}
	else // 불러오기
	{
		while (!Load.eof())
		{
			if (count == 10)
				break;
			Load >> ranking[count];
			Load >> score[count];
			count++;
		}
	}
	Load.close();

	// 랭킹

	// 10개까지 그뒤는 지워지게하기
}

bool Game_Scene::Input(float fETime)
{
	if (JEngine::InputManager::GetInstance()->isKeyDown(VK_UP) && moving_check == false)
	{
		paper_dir = UP;
		moving_check = true;
	}
	else if (JEngine::InputManager::GetInstance()->isKeyDown(VK_DOWN) && moving_check == false)
	{
		paper_dir = DOWN;
		moving_check = true;
	}
	else if (JEngine::InputManager::GetInstance()->isKeyDown(VK_LEFT) && moving_check == false)
	{
		paper_dir = LEFT;
		moving_check = true;
	}
	else if (JEngine::InputManager::GetInstance()->isKeyDown(VK_RIGHT) && moving_check == false)
	{
		paper_dir = RIGHT;
		moving_check = true;
	}
	else if (JEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE))
		JEngine::SceneManager::GetInstance()->LoadScene(1);

	return false;
}

void Game_Scene::Update(float fETime)
{
	time = fETime; // fps에 쓰인다?

	Time();
	if (timeover_time == 0)
		Move();
	Point();
	Fever();
}

void Game_Scene::Draw(HDC hdc)
{
	if (loading_time >= GetTickCount())
	{
		m_pLoadingBack->Draw(0, 0); // 로딩
		m_pLoading->Draw(115, 190);
		m_pLoadingWord->Draw(150, 370);
		game_time = 45000 + GetTickCount();
	}
	else if (show_result == true)
	{
		m_pRankBack->Draw(0, 0);
		for (int i = 0; i < 10; i++)
			m_pShow_Rank[i]->Draw();
	}
	else
	{
		m_pBack->Draw(0, 0); // 배경

		m_pPaper[visible_paper[1]]->Draw(155, 300);
		m_pPaper[visible_paper[0]]->Draw(paper_x, paper_y);

		m_pShow_Score->Draw();
		m_pShow_Paper_Score->Draw();
		if (timeover_time == 0)
			m_pShow_Time->Draw2(24, 618, (game_time - GetTickCount()) / 45000, 1.0);

		if (fever_lv == 1 && full_fever_time == 0)
			m_pShow_Fever[0]->Draw2(22, 53, 1, 1);
		else if (fever_lv == 2 && full_fever_time == 0)
			m_pShow_Fever[1]->Draw2(22, 53, 1, 1);
		else if (fever_lv == 2 && full_fever_time != 0)
		{
			if (fever_light >= 0 && fever_light <= 9)
				m_pShow_Fever[0]->Draw2(22, 53, 1, 1);
			else if (fever_light >= 10 && fever_light <= 19)
				m_pShow_Fever[1]->Draw2(22, 53, 1, 1);
			else if (fever_light >= 20 && fever_light <= 29)
				m_pShow_Fever[2]->Draw2(22, 53, 1, 1);
			else
				fever_light = 0;

			fever_light++;
		}

		if (full_fever_time == 0)
			m_pShow_Fever[fever_lv]->Draw2(22, 53, fever_gauge / 200, 1);

		if (timeover_time != 0)
			m_pTimeOut->Draw(115, 280);
	}

}

void Game_Scene::Release()
{
	JEngine::UIManager::GetInstance()->RelaaseUI();
}

void Game_Scene::Move()
{
	if (paper_dir == UP && paper_y > UP_END)
	{
		paper_y -= 11;
	}
	else if (paper_dir == DOWN && paper_y < DOWN_END)
	{
		paper_y += 11;
	}
	else if (paper_dir == LEFT && paper_x > LEFT_END)
	{
		paper_x -= 11;
	}
	else if (paper_dir == RIGHT && paper_x < RIGHT_END)
	{
		paper_x += 11;
	}
	else if (moving_check == true)
	{
		if (paper_y >= UP_END || paper_y <= DOWN_END || paper_x >= LEFT_END || paper_x <= RIGHT_END) // 종이를 맞췄을때
		{
			if (fever_lv == 1)
				paper_score += 100;
			else if (fever_lv == 2 && fever_gauge < 200 && full_fever_time == 0)
				paper_score += 200;
			else if (full_fever_time != 0)
				paper_score += 300;

			if (paper_dir == UP && visible_paper[0] == GREEN)
			{
				game_score += paper_score;
				if (fever_lv < 3 && fever_gauge <= 200)
					fever_gauge += 20;
				combo_count++;
			}
			else if (paper_dir == DOWN && visible_paper[0] == YELLOW)
			{
				game_score += paper_score;
				if (fever_lv < 3 && fever_gauge <= 200)
					fever_gauge += 20;
				combo_count++;
			}
			else if (paper_dir == LEFT && visible_paper[0] == BLUE)
			{
				game_score += paper_score;
				if (fever_lv < 3 && fever_gauge <= 200)
					fever_gauge += 20;
				combo_count++;
			}
			else if (paper_dir == RIGHT && visible_paper[0] == RED)
			{
				game_score += paper_score;
				if (fever_lv < 3 && fever_gauge <= 200)
					fever_gauge += 20;
				combo_count++;
			}
			else // 틀렸을때
			{
				combo_count = 0;
				fever_gauge -= 40;

				if (fever_lv == 1)
					paper_score = 200;
				else if (fever_lv == 2 && fever_gauge < 200 && full_fever_time == 0)
					paper_score = 300;
				else if (full_fever_time != 0)
					paper_score = 400;
				else
					paper_score = 100;
			}


			moving_check = false;
			paper_dir = NULL;
			visible_paper[0] = visible_paper[1];
			visible_paper[1] = rand() % 4;
			paper_x = 155;
			paper_y = 300;

		}
	}
}

void Game_Scene::Point()
{
	char buf[256] = {};
	char buf2[256] = {};

	sprintf_s(buf, "%d", game_score);
	m_pShow_Score->Init(buf, 200, 25, DT_CENTER);

	sprintf_s(buf2, "%d", paper_score);
	m_pShow_Paper_Score->Init(buf2, paper_x + 40, paper_y + 45, DT_CENTER);

}

void Game_Scene::Time()
{
	if (game_time <= GetTickCount() && timeover_time == 0)
		timeover_time = 3000 + GetTickCount();
	else if (game_time <= GetTickCount() && timeover_time <= GetTickCount())
	{
		if (rank_update == false)
		{
			Set_Rank(); // 랭킹업뎃
			Set_Show_Rank();
		}
		show_result = true;
		JEngine::UIManager::GetInstance()->AddButton(180, 520, "OnSelect.bmp", std::bind(&Game_Scene::BackTo_Game, this));
	}
}

void Game_Scene::Fever()
{
	if (fever_gauge > 0 && fever_lv < 3)
		fever_gauge -= 0.02;

	if (fever_gauge <= 0 && fever_lv > 0)
	{
		fever_lv--;
		fever_gauge = 199;
	}
	else if (fever_gauge <= 0 && fever_lv == 0)
		fever_gauge = 0;

	if (fever_gauge >= 200)
	{
		if (fever_lv < 2)
		{
			fever_gauge = 10;
			fever_lv++;
		}
		else if (fever_lv == 2)
		{
			fever_gauge = 200;
		}
	}

	if (fever_lv == 2 && fever_gauge == 200)
	{
		if (full_fever_time == 0)
			full_fever_time = 5500 + GetTickCount();
		else if (full_fever_time <= GetTickCount())
		{
			full_fever_time = 0;
			fever_lv = 0;
			fever_gauge = 0;
			paper_score = 100;
		}
	}

	if (combo_count == 5)
	{
		fever_gauge += 40;
		combo_count = 0;
	}

	/*
	피버 1단계 - 기본페이퍼포인트 + 100 계속증가 - 시작포인트 200
	피버 2단계 - 기본페이퍼포인트 + 200 계속증가 - 시작포인트 300
	피버 3단계 - 기본페이퍼포인트 + 300 계속증가 - 시작포인트 400

	콤보 5번마다 피버게이지 2배 충전
	*/
}

void Game_Scene::Set_Rank()
{
	ofstream Save;
	int tmp_game_score = 0;
	int count = 0;

	for (int i = 0; i < 10; i++)
	{
		if (score[i] == 0)
		{
			score[i] = game_score;
			break;
		}
		else if (i == 9 && score[9] != 0)
		{
			tmp_game_score = game_score;

			for (int i = 9; i >= 0; i--)
			{
				if (score[i] <= tmp_game_score)
				{
					score[i] = tmp_game_score;
					break;
				}
			}
		}
	}

	tmp_game_score = 0;

	sort(score.begin(), score.end(), greater<int>());

	for (int i = 0; i < 10; i++)
		ranking[i]= i + 1;

	//// 동점수가 나올경우 점수는 하나만 남는게 아니라 바로 다음순위로 넘어가기 때문에 새로 정렬필요없음

	Save.open("Rank1.txt");
	if (Save.is_open())
	{
		while (count < 10)
		{
			Save << ranking[count] << " ";
			Save << score[count] << endl;
			count++;
		}
	}
	Save.close();

	rank_update = true;
}

void Game_Scene::Set_Show_Rank()
{
	int num = 0;
	int tmp_y = 25;
	char buf[256] = {};

	while (num != 10)
	{
		sprintf_s(buf, "%d등     %d", ranking[num], score[num]);
		m_pShow_Rank[num]->Init(buf, 50, tmp_y, DT_CENTER);
		num++;
		tmp_y += 25;
	}
}

bool Game_Scene::BackTo_Game()
{
	JEngine::SceneManager::GetInstance()->LoadScene(1);
	return true;
}

Game_Scene::~Game_Scene()
{
	score.clear();
}

