#include "Client.h"

#include "../FuelClub-Client/GUI.h"

void WINAPI ReceiveStub(void* ptr) {
	return ((Client*)ptr)->Receive();
}

Client::Client(char* ip, int port) {

	//Initialize Variables
	serverSocket = NULL;
	receiveThread = NULL;
	recv_callback = NULL;
	connected = false;

	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error : WSAStartup failed.\n");
        return;
    }

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Error : serverSocket failed (%d).\n", WSAGetLastError());
        WSACleanup();
        return;
    }

	hostent* host = gethostbyname(ip);

	sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    addr.sin_port = htons(port);

	if (connect(serverSocket, (SOCKADDR*)(&addr), sizeof(sockaddr_in)) == SOCKET_ERROR) {
		printf("Error : connect failed (%d).", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

	GUI::addListItem(ID_LIST, "Connected to server");

	receiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveStub, this, 0, NULL);
	if (!receiveThread) {
		printf("Error : could not create receiveThread.\n");
	}

	this->connected = true;
}

Client::~Client() {
	closesocket(serverSocket);
	WSACleanup();
}

int Client::Send(unsigned char* data, int length) {
	return send(serverSocket, (char*)data, length, 0);
}

int Client::Send(Packet* packet) {
	return Send(packet->raw(), packet->rawLength());
}

void Client::Receive() {
	unsigned char buffer[RECV_BUFFER];
	int len = 0;

	Packet* packet = NULL;
	Packet::Header header = {0};
	unsigned long recvPacketLen = 0;

	do {
		ZeroMemory(buffer, sizeof(buffer));
		len = recv(serverSocket, (char*)buffer, sizeof(buffer), 0);

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
				if (recv_callback) recv_callback(packet);
				recvPacketLen = 0;
				ZeroMemory(&header, sizeof(Packet::Header));
			}

		} else if (len == 0) {
			printf("Disconnected\n");
			closesocket(serverSocket);
		}
	} while(len > 0);
}

void Client::SetRecvCallback(Client_RecvCallback callback){
	recv_callback = callback;
}

bool Client::isConnected() {
	return this->connected;
}
