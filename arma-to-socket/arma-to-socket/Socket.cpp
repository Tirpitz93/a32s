

#include "stdafx.h"
#include <string> 
#include "Socket.h"



Socket::Socket(){
	status = -1;
	connected = false;
	statusMsg = *"Socket Created";
}

int Socket::Prepare(CHAR *address, int sockType = 1, char* port = "9999", int timeout= 1000)
{
	if (status != -1){
		status = -1;
	}

	sockPort = port;
	protocol = sockType;
	addressPtr = address;
	s = INVALID_SOCKET;
	result = NULL,
		ptr = NULL,
		hints;
	sendbuf = "this is a test";

	// Validate the parameters

	// Initialize Winsock
	initWinsock();

	//set timout
	int nTimeout = timeout; // ms
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(int));


	ZeroMemory(&hints, sizeof(hints));
	//set sock type
	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = sockType;
	//set protocol
	switch (sockType){

	case 1:
		hints.ai_protocol = IPPROTO_TCP;
		break;
	case 2:
		hints.ai_protocol = IPPROTO_UDP;
		break;
	default:
		hints.ai_protocol = IPPROTO_TCP;

	};

	status = 0;
	statusMsg = *"Socket Prepped";
	return 0;

}

int Socket::Init() {
	
	// Resolve the server address and port
	iResult = getaddrinfo(addressPtr, sockPort, &hints, &result);
	if (iResult != 0) {
		statusMsg = ("getaddrinfo failed with error: %d\n", iResult);

		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		s = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (s == INVALID_SOCKET) {
			statusMsg = ("socket failed with error: %ld\n", WSAGetLastError());

			return 1;
		}

		//sendbuf = "this is a test";
		//iResult = sendto(s, sendbuf, strlen(sendbuf), 0, ptr->ai_addr, ptr->ai_addrlen);
		// Connect to server.
		if (protocol == 1){


			if (connected != true){

				iResult = connect(s, ptr->ai_addr, (int)ptr->ai_addrlen);

				if (iResult == SOCKET_ERROR) {
					closesocket(s);
					s = INVALID_SOCKET;
					continue;
				}
				connected = true;
			}
		}

		address = *new(struct sockaddr);
		address = *ptr->ai_addr;
		addressLen = *new(int);
		addressLen = ptr->ai_addrlen;


		break;
	}

	freeaddrinfo(result);

	if (s == INVALID_SOCKET) {
		statusMsg = *("Unable to connect to server!\n");

		return 1;
	}

	return 0;
};

int Socket::Send(string input){
	if (status != 0){
		return status;
	}
	sendbuf = (char*)input.c_str();

	switch (protocol){
	case 1:
		// Send an initial buffer
		iResult = send(s, sendbuf, (int)strlen(sendbuf), 0);
		break;
	case 2:
		iResult = sendto(s, sendbuf, strlen(sendbuf), 0, &address, addressLen);
		break;

	default:
		iResult = -1;

	};
	if (iResult == SOCKET_ERROR) {
		statusMsg = ("send failed with error: %d\n", WSAGetLastError());
		closesocket(s);

		return 1;
	}
	memset(sendbuf, '\0', DEFAULT_BUFLEN);

	statusMsg = ("Bytes Sent: %ld\n", iResult);
	return 0;


};

string Socket::Recieve() {
	if (status != 0){
		return to_string(status);
	}
	string rtn = "";
	//statusMsg = ((rtn.c_str()));
	//memset(recvbuf, '\0', DEFAULT_BUFLEN);
	statusMsg = *("\nwiped buffer\n");
	// Receive data until the server closes the connection
	do {

		iResult = recv(s, recvbuf, recvbuflen, 0);

		statusMsg = *((to_string(iResult)).c_str());

		if (iResult > 0){

			rtn += recvbuf;
			statusMsg = *(recvbuf);


			return rtn;
		}

		else{
			statusMsg = ("recv failed: %d\n", WSAGetLastError());

		}
	} while (iResult > 0);

	return to_string(WSAGetLastError());
};

int Socket::Disconnect(){
	freeaddrinfo(result);
	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(s, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		//printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(s);

		status = 1;
	}
	return 0;

};

string Socket::SendRecieve(string input){
	Send(input);
	return Recieve();

};

int Socket::getStatus(){

	return status;
}
char Socket::getStatusMsg(){
	return statusMsg;
}
Socket::~Socket()
{
	// cleanup
	closesocket(s);


	status = 0;
};

bool Socket::WinsockInitialized()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED){
		return false;
	}

	closesocket(s);
	return true;
}


int Socket::initWinsock(){
	if (!WinsockInitialized()){
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			statusMsg = ("WSAStartup failed with error: %d\n", iResult);
			status = 1;
			return  1;
		}

	}
	return 0;

}