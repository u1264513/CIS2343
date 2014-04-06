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

/** Constructor to create server object and create a listen thread
 *  @param port Port to listen on
 */
Server::Server(int port) {

	//Initialize Variables
	listenThread = NULL;
	listenSocket = NULL;
	clientId = 0;
	recv_callback = NULL;

	WSADATA wsaData;

	//Initialize winsock
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error : WSAStartup failed.\n");
        return;
    }

	//Obtain local IP
	char hostName[256];
	gethostname(hostName, sizeof(hostName));
    hostent* host_entry = gethostbyname(hostName);
    this->localIP = inet_ntoa (*(in_addr*)*host_entry->h_addr_list);
	printf("Local IP : %s\n\n", localIP);

	//Create socket
    if ((listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Error : listenSocket failed (%d).\n", WSAGetLastError());
        WSACleanup();
        return;
    }

	sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

	//Bind socket
    if (bind(listenSocket, (SOCKADDR*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        printf("Error : bind failed (%d).\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

	//Create listen thread
	listenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenStub, this, 0, NULL);

	if (!listenThread) {
		printf("Error : could not create listenThread.\n");
	}
}

/** Destructor
 */
Server::~Server() {
	closesocket(listenSocket);
    WSACleanup();
}

/** Listens for incoming client connections
 */
void Server::Listen() {

	//Listen for clients
	while (listen(listenSocket, SOMAXCONN) != SOCKET_ERROR) {

		//Accept client connection
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

		//Create receive thread
		ReceiveStubParam* param = new ReceiveStubParam();
		param->server = this;
		param->client = client;
		client->receiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveStub, param, 0, NULL);

		if (!client->receiveThread) {
			printf("Error : could not create receiveThread.\n");
		}

		//Add client to client list
		clients.push_back(client);
		clientId++;
    }

	printf("Error : listen failed (%d).\n", WSAGetLastError());
    closesocket(listenSocket);
    WSACleanup();
}

/** Receive thread which handles the raw data received from a client and initiates the receive callback.
 *  @param client Pointer to client object we are receiving data from
 */
void Server::Receive(ServerClient* client) {
	unsigned char buffer[RECV_BUFFER];
	int len = 0;

	Packet* packet = NULL;
	Packet::Header header = {0};
	unsigned long recvPacketLen = 0;

	do {
		ZeroMemory(buffer, sizeof(buffer));
		len = recv(client->clientSocket, (char*)buffer, sizeof(buffer), 0);

		//Received data?
		if (len > 0) {

			//Received packet, create packet object
			if (recvPacketLen == 0 && (len >= sizeof(Packet::Header)) && ((*(unsigned long*)buffer) == PACKET_MAGIC || (*(unsigned long*)buffer) == PACKET_MAGIC_CRYPT)) {
				memcpy(&header, buffer, sizeof(Packet::Header));
				packet = new Packet(&header);
				packet->AddData(buffer+sizeof(Packet::Header), len-sizeof(Packet::Header));
				recvPacketLen += len-sizeof(Packet::Header);

			//Received packet data, add data to packet object
			} else if (recvPacketLen && (recvPacketLen < header.length)) {
				packet->AddData(buffer, len);
				recvPacketLen += len;

			//Invalid packet
			} else {
				printf("Error : Could NOT receive valid packet!\n");
			}

			//Received entire packet, initiate recv_callback
			if (recvPacketLen && recvPacketLen == header.length) {
				if (recv_callback) recv_callback(client, packet);
				recvPacketLen = 0;
				ZeroMemory(&header, sizeof(Packet::Header));
			}

		//Disconnected?
		} else if (len == 0) {
			printf("Client %d Disconnected\n", client->id);
			closesocket(client->clientSocket);
		}

	//Loop while receiving data
	} while(len > 0);
}

/** 
 *  @param client Pointer to client object to send data to
 *  @param data Data to send to client
 *  @param length Length of data to send to client
 */
int Server::Send(ServerClient* client, unsigned char* data, unsigned long length) {
	return send(client->clientSocket, (char*)data, length, 0);
}

/** Send packet to client
 *  @param client Pointer to client object to send packet to
 *  @param packet Packet to send to client
 */
int Server::Send(ServerClient* client, Packet* packet) {
	return Send(client, packet->raw(), packet->rawLength());
}

/** Set receive callback to call in receive thread
 *  @param callback Call back function
 */
void Server::SetRecvCallback(Server_RecvCallback callback){
	recv_callback = callback;
}

/** Return local IP
 */
char* Server::getLocalIP() {
	return this->localIP;
}
