#pragma once

#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string> 




// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "9999"

using namespace std;

/*
Status Codes:
-1: initialised but not connected
0:ready to connect
1: errror occured



*/
class Socket
{
public:
	Socket();
	Socket(char*, int);
	int Prepare(char*, int, char*, int);
	int Init();
	int Send(string input);
	string Recieve();
	int Disconnect();
	string SendRecieve(string input);
	int getStatus();
	char getStatusMsg();
	~Socket();
	bool WinsockInitialized();
	int initWinsock();

private:
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	//addrinfo address;
	sockaddr address;
	int addressLen;
	char *sendbuf;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	int status;
	CHAR statusMsg;
	SOCKET s;
	CHAR addressChar;
	CHAR *addressPtr;
	WSADATA wsaData;
	int protocol;
	bool connected;
	char * sockPort;


};
