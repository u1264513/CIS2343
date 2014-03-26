#pragma once
#include <stdio.h>
#include <vector>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

#include "Packet.h"

#define RECV_BUFFER 1024

class Server {
public:

	struct ServerClient {
		int id;
		HANDLE receiveThread;
		SOCKET clientSocket;
		sockaddr_in socketAddress;
	};
	typedef void (*Server_RecvCallback)(ServerClient* client, Packet* packet);

	Server(int port);
	~Server();

	void Listen();

	void Receive(ServerClient* client);

	int Send(ServerClient* client, unsigned char* data, unsigned long length);
	int Send(ServerClient* client, Packet* packet);

	void SetRecvCallback(Server_RecvCallback callback);

private:
	SOCKET listenSocket;
	HANDLE listenThread;

	Server_RecvCallback recv_callback;

	vector<ServerClient*> clients;
	int clientId;

};
