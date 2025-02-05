#include "Play.h"

Play::Play()
{
	m_iLife = FULL_HP;
	m_iScore = 0;
	m_iName = { "\0" };
	m_iWord = { "\0" };
	m_iTmpWord = { "\0" };
	m_iload_word = 0; // 총 단어 갯수
	w = NULL; // 단어
	m_inum = 0;
	m_iTmp_Iw[INSERT_WORD_MAX] = { '\0' };
	m_iPause = FALSE;
	m_iStage = 1; // 스테이지
	m_iSpeed = 900; //기본속도
	m_iCreate_Speed = 3000; // 기본생성속도
	m_iItem_Use = FALSE;
	m_iItem_Clear_Use = FALSE;
	m_iItem_Blind_Use = FALSE;
	m_icur_time = 0;
	m_iold_time = 0;
}

void Play::Life_Name()
{
	char buf[256];

	Ui.EraseHeart(8, HEIGHT + 1);

	RED
	Ui.TextDraw("Life : ", 1, HEIGHT + 1);
	for (int i = 0, j = 8; i < m_iLife; i++) // 하트 보유수만큼 출력
	{
		Ui.HeartDraw(j, HEIGHT + 1);
		j += 2;
	}
	sprintf_s(buf, "Score : %d", m_iScore);
	Ui.DrawMidText(buf, WIDTH, HEIGHT + 1);
	if (m_iName == "\0")
	{
		Ui.DrawMidText("Name : 이름없음", WIDTH * 1.8, HEIGHT + 1);
	}
	else
	{
		Ui.DrawMidText("Name : " + m_iName, WIDTH * 1.8, HEIGHT + 1);
	}
}

void Play::Start()
{
	int Select = 0;

	while (1)
	{
		if (w != NULL)
		{
			delete[] w;
		}
		w = new _Ward[WARD_MAX]; // 미리 공간을 할당해야 Load_Ward 에서 동작이됌
		m_iLife = FULL_HP;
		m_iName = { "\0" };
		m_iStage = 1;
		m_iScore = 0;

		Load_Ward(w, m_iload_word);	// 단어 불러오기

		Ui.First_Screen();
		Life_Name();

		BLUE
		Ui.DrawMidText("1.Game Start", WIDTH, HEIGHT * 0.4);
		Ui.DrawMidText("2.Rank", WIDTH, HEIGHT * 0.4 + 3);
		Ui.DrawMidText("3.Exit", WIDTH, HEIGHT * 0.4 + 6);

		Select = Ui.MenuSelectCursor(3, 3, WIDTH * 0.4, HEIGHT * 0.4);

		if (Select == 1)
		{
			Game_Start();
		}
		else if (Select == 2)
		{
			Show_Rank();
		}
		else if (Select == 3)
		{
			return;
		}
	}
}

void Play::Show_Rank()
{
	int num = 0;
	string tmp_str = { "\0" };
	int tmp_score = 0;
	int tmp_stage = 0;
	char buf[256] = {'\0'};

	num = Count_Rank(); // 필요한 공간계산후 할당

	if (num == NULL)
	{
		RED
		cout << " 랭킹정보가 없습니다!!!" << endl;
		ORIGINAL
		_getch();
		return;
	}

	if (r != NULL)
	{
		delete[] r;
	}
	r = new Ranking[num];

	Load_Rank(r);
	Ui.Rank_Screen();

	if (num != 1)
	{
		for (int i = 0; i < num; i++) // 내림차순 정렬
		{
			if (r[i + 1].name.length() == NULL)
			{
				break;
			}
			if (r[i].total_score < r[i + 1].total_score)
			{
				tmp_score = r[i + 1].total_score;
				r[i + 1].total_score = r[i].total_score;
				r[i].total_score = tmp_score;

				tmp_stage = r[i + 1].stage;
				r[i + 1].stage = r[i].stage;
				r[i].stage = tmp_stage;

				tmp_str = r[i + 1].name;
				r[i + 1].name = r[i].name;
				r[i].name = tmp_str;
			}
		}
	}


	for (int i = 0; i < num; i++)
	{
		sprintf_s(buf,"%d",r[i].total_score);
		Ui.DrawMidText(buf, WIDTH - 1, HEIGHT * 0.1 + 8 + (i * 2));
		sprintf_s(buf, "%d", r[i].stage);
		Ui.DrawMidText(buf, WIDTH * 1.3 + 3, HEIGHT * 0.1 + 8 + (i * 2));
		Ui.DrawMidText(r[i].name, WIDTH * 0.6, HEIGHT * 0.1 + 8 + (i * 2));
	}


	_getch();
}

void Play::Game_Start()
{

	Ui.Game_Screen();
	Life_Name();

	BLUE
	Ui.DrawMidText("Skip : s", WIDTH, HEIGHT * 0.75);

	Story();
	Insert_Name();
	Playing();
}

void Play::Story()
{
	int i = 0;
	int line = 0;
	string story_talk[26] = { "0" };
	char ch = 0;
	ifstream Read;

	Read.open("베네치아_스토리.txt");




	if (!Read.eof())
	{
		Read >> line;
		while (line != 0)
		{
			getline(Read, story_talk[i]);
			i++;
			line--;
		}
	}


	for (int j = 0, k = 0, n = 0; j < i; j++)
	{
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 's')
			return;
		}

		Sleep(500);
		if (j > 9)
		{
			k = 0;
			n += 1;
			while (k != j - n)
			{
				if (_kbhit())
				{
					ch = _getch();
					if (ch == 's')
						return;
				}

				Ui.DrawMidText("                                          ", WIDTH, HEIGHT * 0.2 + k);
				Ui.DrawMidText(story_talk[k + n], WIDTH, HEIGHT * 0.2 + k);
				k++;
			}
			Ui.DrawMidText("                                          ", WIDTH, HEIGHT * 0.2 + j - n);
			Ui.DrawMidText(story_talk[j], WIDTH, HEIGHT * 0.2 + j - n);
		}
		else
		Ui.DrawMidText(story_talk[j], WIDTH, HEIGHT * 0.2 + j);
	}
}

void Play::Insert_Name()
{
	char ch = 0;
	char name_tmp[9] = { '\0' };
	int num = 0;
	string tmp = { "\0" };

	Ui.Game_Screen();
	Life_Name();


	BLUE
	Ui.DrawMidText("이름 입력", WIDTH, HEIGHT * 0.75 - 5);
	Ui.gotoxy(WIDTH, HEIGHT * 0.75);

	ORIGINAL
	while (1)
	{
		ch = _getch();

		if (ch == BACK_SPACE && num >= 1)
		{
			num--;
			name_tmp[num] = { '\0' };
			tmp = { "\0" };
			for (int i = 0; i < num; i++)
			{
				tmp += name_tmp[i];
			}
			Ui.DrawMidText("                   ", WIDTH, HEIGHT * 0.75 - 4);
		}

		else if (name_tmp[8] != NULL)
		{
			Ui.DrawMidText("10글자 초과!!", WIDTH, HEIGHT * 0.75 - 4);
			_getch();
		}

		else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			name_tmp[num] = ch;
			tmp += name_tmp[num];
			num++;
		}

		else if (ch == ENTER)
		{
			m_iName = tmp;
			if (m_iName != "\0")
			{
				break;
			}
		}

		Ui.DrawMidText("                    ", WIDTH, HEIGHT * 0.75);
		Ui.DrawMidText(tmp, WIDTH, HEIGHT * 0.75);
	}

	// 영어이외에는 입력안되고 한글은 바로지워짐
	// 9글자 까지, 입력하는 즉시 텍스트가 중앙에 맞춰짐
}

void Play::Insert_Word() // m_iPause == FALSE 구문 3개로 비패널티와 패널티시에 구분
{
	char ch = 0;

	ORIGINAL

	if (m_iPause == FALSE)
	{
		Ui.gotoxy(WIDTH, HEIGHT * 0.75);
	}

	ch = _getch();

	if (ch == BACK_SPACE && m_inum >= 1)
	{
		m_inum--;
		m_iTmp_Iw[m_inum] = { '\0' };
		m_iTmpWord = { "\0" };
		for (int i = 0; i < m_inum; i++)
		{
			m_iTmpWord += m_iTmp_Iw[i];
		}
		Ui.DrawMidText("                   ", WIDTH, HEIGHT * 0.75 - 4);
	}

	else if (m_iTmp_Iw[18] != NULL)
	{
		Ui.DrawMidText("20글자 초과!!", WIDTH, HEIGHT * 0.75 - 4);
		_getch();
	}

	
	else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		if (m_iPause == FALSE)
		{
			m_iTmp_Iw[m_inum] = ch;
			m_iTmpWord += m_iTmp_Iw[m_inum];
			m_inum++;
		}
	}

	else if (ch == ENTER)
	{
		m_iWord = m_iTmpWord;
		m_iTmpWord = { "\0" }; // 다음단어를 위해 초기화
		m_inum = 0;
		m_iTmp_Iw[INSERT_WORD_MAX] = { '\0' };
		Correct_Word();
		Life_Name(); // 점수 바로 출력
	}

	if (m_iPause == FALSE)
	{
		Ui.DrawMidText("                    ", WIDTH, HEIGHT * 0.75);
		Ui.DrawMidText(m_iTmpWord, WIDTH, HEIGHT * 0.75);
	}

}

int Play::Draw_Drop()
{
	int printed_word[WARD_MAX] = {'\0'};
	int word_num = 0;
	int num = 0;
	int heart = 0;

	int move_time = 0;
	int old_time = 0;
	int cur_time = 0;

	int warning_time = 0;

	m_iTmpWord = { "\0" }; // 재시작할시를 위해 초기화
	m_inum = 0;
	m_iTmp_Iw[INSERT_WORD_MAX] = { '\0' };

	move_time = clock();
	old_time = clock();
	warning_time = clock();
	m_iold_time = clock();

	while (word_num != WARD_MAX)
	{
		if (m_iScore >= 500 * m_iStage)
		{
			m_iStage++;
			return 0;
		}

		cur_time = clock();
		m_icur_time = clock();

		if (_kbhit()) // ★★★ kbhit는 경험상 딜레이가 있어도 미리 입력해놓으면 나중에 실행된다;;??
		{            // 그래서 && 로 진입이 안되게하지말고 다른방향으로 코드짜기
			Insert_Word();
		}

		if (clock() - move_time >= (m_iSpeed / m_iStage + 300) && word_num != NULL) // 단어속도시간
		{

			for (int i = 0; i < num; i++) // 출력된것은 전부 움직이게 하기
			{
				heart = Drop_Word(w, printed_word[i]);
				Item_Blind_Screen(printed_word[i]); // 아이템블라인드 적용상태일시 실행된다
				if (heart == 1)
				{
					m_iLife--;
					Life_Name();
					if (m_iLife == NULL)
					{
						Ui.DrawMidText("●○ 게임 오버 ●○", WIDTH, HEIGHT * 0.3);
						m_iTotal_Score += m_iScore; // 총 스코어
						Sleep(1500);
						return 1;
					}
				}
			}
			move_time = clock();

			BLUE
			Ui.LittleBox(WIDTH, HEIGHT * 0.7, WIDTH * 0.3, HEIGHT * 0.15);
			ORIGINAL
			Ui.DrawMidText(m_iTmpWord, WIDTH, HEIGHT * 0.75);
		}

		if (cur_time - old_time >= (m_iCreate_Speed / m_iStage) + 500) // 단어 출력시간
		{
			printed_word[num] = Draw_Word(w, printed_word[num]);
			word_num++;
			num++;
			old_time = cur_time;
		}

		if (clock() - warning_time >= 25 && m_iPause == TRUE) // 패널티문구시간
		{
			RED
				Ui.DrawMidText("Miss [입력금지]", WIDTH, HEIGHT * 0.75);
			ORIGINAL
				warning_time = clock();
		}

		if (m_iPause == TRUE && m_icur_time - m_iold_time >= 2000) // 패널티시간
		{
			m_iold_time = m_icur_time;
			m_iPause = FALSE;
		}

		if (m_iItem_Use == TRUE && m_icur_time - m_iold_time >= 2200) // 속도 변화아이템시간
		{
			m_iItem_Use = FALSE;
			m_iSpeed = 900;
			m_iCreate_Speed = 3000;
			m_iold_time = m_icur_time;
		}

		Item_Blind_Time(m_icur_time, m_iold_time); // 블라인드시간
	}
}

void Play::Item_Word(int i)
{

	if (w[i].item_word == WORD_SPEED_UP)
	{
		m_iSpeed -= 800;
		m_iItem_Use = TRUE;
	}
	else if (w[i].item_word == WORD_SPEED_DOWN)
	{
		m_iSpeed += 800;
		m_iItem_Use = TRUE;
	}
	else if (w[i].item_word == WORD_PAUSE)
	{
		m_iSpeed = 999999999;
		m_iCreate_Speed = 999999999;
		m_iItem_Use = TRUE;
	}
	else if (w[i].item_word == WORD_BLIND)
	{
		m_iItem_Blind_Use = TRUE;
	}
	else if (w[i].item_word == SCREEN_CLEAR)
	{
		m_iItem_Clear_Use = TRUE;
	}
}

int Play::Item_Blind_Screen(int num) // 화면블라인드 아이템
{
	int n = 0;

	if (m_iItem_Blind_Use == TRUE)
	{
		if (w[num].on_screen == TRUE && w[num].status == TRUE)
		{
			n = w[num].name.length();
			Ui.gotoxy(w[num].x, w[num].y);
			Ui.DrawBlind(n);
		}
	}

	return 0;
}

void Play::Item_Blind_Time(int cur, int old)
{
	if (m_iItem_Blind_Use == TRUE && cur - old >= 1500)
	{
		m_iItem_Blind_Use = FALSE;
	}
}

void Play::Item_Clear_Screen() // 화면클리어 아이템
{
	int n = 0;

	if (m_iItem_Clear_Use == TRUE)
	{
		for (int i = 0; i < WARD_MAX; i++)
		{
			if (w[i].on_screen == TRUE && w[i].status == TRUE)
			{
				n = w[i].name.length();
				Ui.gotoxy(w[i].x, w[i].y);
				Ui.EraseWord(n);
				w[i].status = FALSE;
				w[i].on_screen = FALSE;
				m_iScore += 53 + (m_iStage * 20);
			}
		}
		m_iItem_Clear_Use = FALSE;
	}
}

void Play::Correct_Word() // 단어 맞추면 점수가 오른다
{
	if (m_iWord.length() == 0)
		return;

	int num = 0;

	m_iold_time = m_icur_time; // 시간 일정하게 하기위해 단어 맞출때마다 old_time초기화

	for (int i = 0; i < WARD_MAX; i++)
	{
		if (m_iWord == w[i].name)
		{
			if (w[i].status == TRUE)
			{
				num = w[i].name.length(); // 단어 길이 저장

				Ui.gotoxy(w[i].x, w[i].y);
				Ui.EraseWord(num);
				w[i].status = FALSE;
				w[i].on_screen = FALSE;
				m_iScore += 53 + (m_iStage * 20);
				Item_Word(i); // 아무 아이템단어일경우 효과적용
				Item_Clear_Screen(); // 화면클리어 아이템일경우 실행된다
				return;
			}
		}
	}

	m_iPause = TRUE; // 정지 패널티
}

void Play::Playing()
{
	int game_over = FALSE;
	char buf[256];
	ofstream Save;

	while (game_over != TRUE)
	{
		m_iTotal_Score += m_iScore; // 총 스코어
		m_iScore = 0; // 스코어 초기화
		for (int i = 0; i < WARD_MAX; i++) // 단어 초기화
		{
			w[i].x = NULL;
			w[i].y = NULL;
			w[i].status = TRUE;
			w[i].item_word = NULL;
			w[i].on_screen = FALSE;
		}
		Set_xy(w); // 단어 위치 세팅
		Set_item(w); // 단어 아이템 세팅
		m_iItem_Use = FALSE;
		m_iItem_Blind_Use = FALSE;
		m_iItem_Clear_Use = FALSE;
		m_icur_time = 0;
		m_iold_time = 0;

		sprintf_s(buf, "★ %d Stage ★", m_iStage);

		PLUM
			Ui.BoxDraw(0, 0, WIDTH, HEIGHT);
		BLUE
			Ui.DrawMidText(buf, WIDTH, HEIGHT * 0.4);
		Life_Name();

		Sleep(2000);
		system("cls");
		Ui.Game_Screen();
		Life_Name();
		game_over = Draw_Drop(); //단어 그리고 떨어지는함수
	}

	Save.open("Rank.txt", ios::app);

	if (Save.is_open())
	{
		Save << m_iName;
		Save << " ";
		Save << m_iStage;
		Save << " ";
		Save << m_iTotal_Score;
		Save << endl;
	}

	Save.close();
	m_iTotal_Score = 0;
}

Play::~Play()
{
	if (w != NULL)
	{
		delete[] w;
	}
	w = NULL;
}