﻿#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <process.h>
#include <iostream>
using namespace std;

#define BUFSIZE 512
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void *arg); //전방선언
unsigned WINAPI RecvMsg(void *arg);
void ErrorHandling(const char *msg);

char name[NAME_SIZE] = "[사용자]";
char msg[BUFSIZE];

enum PACKET_INDEX
{
	PACKET_INDEX_CHAT,
};

#pragma pack(push, 1)
struct Packet_Chat
{
	int type;
	int size;
	char data[BUFSIZE];
};
#pragma pack(pop)

int main()
{
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	HANDLE hSndThread, hRcvThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(9001);

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);
	closesocket(hSock);
	WSACleanup();

	return 0;
}

unsigned WINAPI SendMsg(void *arg)
{
	SOCKET hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUFSIZE];
	Packet_Chat packet;
	int len;

	while (true)
	{
		packet.type = PACKET_INDEX_CHAT;
 
		cin >> msg;
		sprintf(nameMsg, "%s %s", name, msg);

		strcpy(packet.data, nameMsg);
		packet.size = strlen(packet.data);

		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSock);
			exit(0);
		}

		send(hSock, (char*)&packet, sizeof(packet), 0);

	}

	return 0;
}

unsigned WINAPI RecvMsg(void *arg)
{
	int hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUFSIZE];
	int strLen;
	Packet_Chat *packet = NULL;

	while (1)
	{
		strLen = recv(hSock, nameMsg, sizeof(nameMsg), 0);
		packet = (Packet_Chat*)nameMsg;

		if (strLen == -1)
			return -1;

		cout << packet->data << "\n";
	}

	return 0;
}

void ErrorHandling(const char *msg)
{
	cout << msg << "\n";
	exit(1);
}