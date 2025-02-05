#include "NetWork.h"

Player_Info NetWork::Player_info;
Player_Info NetWork::Recv_Player_info[MAX_PLAYER];
bool NetWork::m_player_connect = false;
int NetWork::m_player_wait_room = false;
bool NetWork::m_player_first_send = false;

NetWork::NetWork()
{
	Player_info.Player_Level = Lung;
	Player_info.Player_Pos = 0;
}

void NetWork::Init_Network(HWND hWnd)
{
	//m_hWnd = hWnd;

	if (WSAStartup(MAKEWORD(2, 2), &m_WsaData) != 0)
	{
		cout << "WSAStartup() Error!" << endl;
		system("pause");
	}

	m_ServerSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_ServerAddr.sin_port = htons(9001);

	if (connect(m_ServerSock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr)) == SOCKET_ERROR)
	{
		cout << "Connet() Error!" << endl;
		system("pause");
		return;
	}

	m_player_connect = false;
	Player_info.Player_Update = false;

	m_SendThread = (HANDLE)_beginthreadex(NULL, 0, Send, (void*)&m_ServerSock, 0, NULL);
	m_RecvThread = (HANDLE)_beginthreadex(NULL, 0, Recv, (void*)&m_ServerSock, 0, NULL);
}

unsigned WINAPI NetWork::Send(void *arg)
{
	SOCKET sock = *((SOCKET*)arg);
	int value = 0;
	int connect_check = 0;

	while (1)
	{
		if (connect_check == false && m_player_connect == true) // 처음접속 시
		{
			send(sock, (char*)&m_player_connect, sizeof(m_player_connect), 0);
			connect_check = true;
		}
		if (connect_check == true && m_player_connect == true && m_player_first_send == false) // 처음접속하고 한번만 보낸다
		{
			Player_info.Player_Connect = true;
			Player_info.Player_Update = true;
			Player_info.Player_Checking = 20;

			send(sock, (char*)&Player_info, sizeof(Player_info), 0);

			m_player_first_send = true;
			Player_info.Player_Update = false;
		}
		else if (m_player_wait_room == true && connect_check == true && Player_info.Player_Update == true) // 대기실에서 채팅칠때
		{
			send(sock, (char*)&Player_info, sizeof(Player_info), 0);
			Player_info.Player_Update = false;
		}
	}

	return 0;
}

unsigned WINAPI NetWork::Recv(void *arg)
{
	SOCKET sock = *((SOCKET*)arg);
	int value = 0;
	char buf[BUFSIZ];
	int *tmp = 0;
	int trigger = 0;

	while (1)
	{
		value = recv(sock, buf, sizeof(buf), 0);

		if (value == SOCKET_ERROR)
			break;
		if (value == 0)
			break;

		buf[value] = '\n';  // 끝을 구별하기 위해

		if (m_player_wait_room == true && value == sizeof(Player_Info))
		{
			Player_Info *tmp_info;
			tmp_info = (Player_Info*)buf;
			
			if (tmp_info->Player_Update == true)
			{
				for (int i = 1; i < 9; i++)
				{
					if (tmp_info->Player_Num == i)
					{
						Recv_Player_info[i - 1].Player_Character = tmp_info->Player_Character;
						strcpy_s(Recv_Player_info[i - 1].Player_Chat, tmp_info->Player_Chat);
						Recv_Player_info[i - 1].Player_Ingame_Num = tmp_info->Player_Ingame_Num;
						Recv_Player_info[i - 1].Player_Level = tmp_info->Player_Level;
						strcpy_s(Recv_Player_info[i - 1].Player_Name, tmp_info->Player_Name);
						Recv_Player_info[i - 1].Player_Pos = tmp_info->Player_Pos;
						Recv_Player_info[i - 1].Player_Update = tmp_info->Player_Update;
						Recv_Player_info[i - 1].Player_Connect = tmp_info->Player_Connect;
						Recv_Player_info[i - 1].Player_Num = tmp_info->Player_Num;
						Recv_Player_info[i - 1].Player_Checking = tmp_info->Player_Checking;
						break;
					}
				}
			}
			
		}
		else if (value == sizeof(Player_Info)) // 딱 한번만
			//// 위로 올려보기  Player_Checking 이 안먹음
		{
			Player_Info *tmp_info;
			tmp_info = (Player_Info*)buf;

			if (tmp_info->Player_Checking == 30)
				break;

			for (int i = 1; i < 9; i++)
			{
				if (tmp_info->Player_Num == i)
				{
					Recv_Player_info[i - 1].Player_Character = tmp_info->Player_Character;
					strcpy_s(Recv_Player_info[i - 1].Player_Chat, tmp_info->Player_Chat);
					Recv_Player_info[i - 1].Player_Ingame_Num = tmp_info->Player_Ingame_Num;
					Recv_Player_info[i - 1].Player_Level = tmp_info->Player_Level;
					strcpy_s(Recv_Player_info[i - 1].Player_Name, tmp_info->Player_Name);
					Recv_Player_info[i - 1].Player_Pos = tmp_info->Player_Pos;
					Recv_Player_info[i - 1].Player_Update = tmp_info->Player_Update;
					Recv_Player_info[i - 1].Player_Connect = tmp_info->Player_Connect;
					Recv_Player_info[i - 1].Player_Num = tmp_info->Player_Num;
					Recv_Player_info[i - 1].Player_Checking = 30;
				}
			}		  
	  
		}
		else if (trigger == false && value == sizeof(int))
		{
			tmp = (int*)buf;
			Recv_Player_info[*tmp - 1].Player_Num = *tmp;
			trigger = true;
		}
	}

	return 0;
}

void NetWork::Release_Network()
{
	//// 따로 분류해줘야 오브젝트를 기다리지않고 계속돌아간다
	//// 대신 종료시에 꼭 이 함수를 사용해서 해제 해줘야 한다
	//WaitForSingleObject(m_SendThread, INFINITE);
	//WaitForSingleObject(m_RecvThread, INFINITE);

	closesocket(m_ServerSock);

	WSACleanup();
}


NetWork::~NetWork()
{

}