#pragma once
#include <stdio.h>
#include <vector>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

#include "Packet.h"

#define RECV_BUFFER 1024

typedef void (*Client_RecvCallback)(Packet* packet);

class Client {
public:

	Client(char* ip, int port);
	~Client();

	int Send(unsigned char* data, int length);
	int Send(Packet* packet);

	void Receive();
	void SetRecvCallback(Client_RecvCallback callback);

	bool isConnected();

private:
	SOCKET serverSocket;

	HANDLE receiveThread;
	Client_RecvCallback recv_callback;

	bool connected;
};

