#include "Server.h"

#include "../FuelClub-Server/GUI.h"

void WINAPI ListenStub(void* ptr) {
	return ((Server*)ptr)->Listen();
}

struct ReceiveStubParam {
	void* server;
	void* client;
};
void WINAPI ReceiveStub(ReceiveStubParam* ptr) {
	return ((Server*)ptr->server)->Receive((Server::ServerClient*)ptr->client);
}

Server::Server(int port) {

	//Initialize Variables
	listenThread = NULL;
	listenSocket = NULL;
	clientId = 0;
	recv_callback = NULL;

	WSADATA wsaData;

	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error : WSAStartup failed.\n");
        return;
    }

	char hostName[256];
	gethostname(hostName, sizeof(hostName));
    hostent* host_entry = gethostbyname(hostName);
    this->localIP = inet_ntoa (*(in_addr*)*host_entry->h_addr_list);
	printf("Local IP : %s\n\n", localIP);

    if ((listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Error : listenSocket failed (%d).\n", WSAGetLastError());
        WSACleanup();
        return;
    }

	sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listenSocket, (SOCKADDR*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        printf("Error : bind failed (%d).\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

	listenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenStub, this, 0, NULL);

		if (!listenThread) {
			printf("Error : could not create listenThread.\n");
		}
}

Server::~Server(void) {
	closesocket(listenSocket);
    WSACleanup();
}

void Server::Listen() {
	while (listen(listenSocket, SOMAXCONN) != SOCKET_ERROR) {

		ServerClient* client = new ServerClient();
		client->id = clientId;
		client->clientSocket = accept(listenSocket, (sockaddr*)&client->socketAddress, NULL);

			if (client->clientSocket == INVALID_SOCKET) {
				printf("Error : accept failed (%d).\n", WSAGetLastError());
				closesocket(listenSocket);
				WSACleanup();
				return;
			}

		printf("Client %d Connected - %s\n",client->id, inet_ntoa(client->socketAddress.sin_addr));
		GUI::addListItem(ID_LIST, "STATION #%d - Connected", client->id);

		ReceiveStubParam* param = new ReceiveStubParam();
		param->server = this;
		param->client = client;
		client->receiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveStub, param, 0, NULL);

			if (!client->receiveThread) {
				printf("Error : could not create receiveThread.\n");
			}

		clients.push_back(client);
		clientId++;
    }
	printf("Error : listen failed (%d).\n", WSAGetLastError());
    closesocket(listenSocket);
    WSACleanup();
}

void Server::Receive(ServerClient* client) {
	unsigned char buffer[RECV_BUFFER];
	int len = 0;

	Packet* packet = NULL;
	Packet::Header header = {0};
	unsigned long recvPacketLen = 0;

	do {
		ZeroMemory(buffer, sizeof(buffer));
		len = recv(client->clientSocket, (char*)buffer, sizeof(buffer), 0);

		if (len > 0) {

			//Received Packet
			if (recvPacketLen == 0 && (len >= sizeof(Packet::Header)) && ((*(unsigned long*)buffer) == PACKET_MAGIC || (*(unsigned long*)buffer) == PACKET_MAGIC_CRYPT)) {
				memcpy(&header, buffer, sizeof(Packet::Header));
				packet = new Packet(&header);

				packet->AddData(buffer+sizeof(Packet::Header), len-sizeof(Packet::Header));
				recvPacketLen += len-sizeof(Packet::Header);

			//Received Packet Data
			} else if (recvPacketLen && (recvPacketLen < header.length)) {
				packet->AddData(buffer, len);
				recvPacketLen += len;

			//Invalid Packet
			} else {
				printf("Error : Could NOT receive valid packet!\n");
			}

			//Received Full Packet
			if (recvPacketLen && recvPacketLen == header.length) {
				if (recv_callback) recv_callback(client, packet);
				recvPacketLen = 0;
				ZeroMemory(&header, sizeof(Packet::Header));
			}

		} else if (len == 0) {
			printf("Client %d Disconnected\n", client->id);
			closesocket(client->clientSocket);
		}
	} while(len > 0);
}

int Server::Send(ServerClient* client, unsigned char* data, unsigned long length) {
	return send(client->clientSocket, (char*)data, length, 0);
}
int Server::Send(ServerClient* client, Packet* packet) {
	return Send(client, packet->raw(), packet->rawLength());
}

void Server::SetRecvCallback(Server_RecvCallback callback){
	recv_callback = callback;
}

char* Server::getLocalIP() {
	return this->localIP;
}
