#include "Client.h"

#include "../FuelClub-Client/GUI.h"

void WINAPI ReceiveStub(void* ptr) {
	return ((Client*)ptr)->Receive();
}

/** Initialize client object and create connection using given parameters
 *  @param ip IP address of connection
 *  @param port Port of connection
 */
Client::Client(char* ip, int port) {

	serverSocket = NULL;
	receiveThread = NULL;
	recv_callback = NULL;
	connected = false;

	WSADATA wsaData;

	//Initialize winsock
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error : WSAStartup failed.\n");
        return;
    }

	//Create socket
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

	//Connect to server
	if (connect(serverSocket, (SOCKADDR*)(&addr), sizeof(sockaddr_in)) == SOCKET_ERROR) {
		printf("Error : connect failed (%d).", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

	GUI::addListItem(ID_LIST, "Connected to server");

	//Create receive thread
	receiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveStub, this, 0, NULL);

	if (!receiveThread) {
		printf("Error : could not create receiveThread.\n");
	}

	//Set connection status
	this->connected = true;
}

/** Close socket and clean up winsock
 */
Client::~Client() {
	closesocket(serverSocket);
	WSACleanup();
}

/** Send data to server
 *  @param data Data to send to server
 *  @param length Length of data to send to server
 */
int Client::Send(unsigned char* data, int length) {
	return send(serverSocket, (char*)data, length, 0);
}

/** Send packet to server
 *  @param packet Pointer to packet object to send to server
 */
int Client::Send(Packet* packet) {
	return Send(packet->raw(), packet->rawLength());
}

/** Receive thread, handles received data and initiates received callback
 */
void Client::Receive() {
	unsigned char buffer[RECV_BUFFER];
	int len = 0;

	Packet* packet = NULL;
	Packet::Header header = {0};
	unsigned long recvPacketLen = 0;

	do {
		ZeroMemory(buffer, sizeof(buffer));
		len = recv(serverSocket, (char*)buffer, sizeof(buffer), 0);

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
				if (recv_callback) recv_callback(packet);
				recvPacketLen = 0;
				ZeroMemory(&header, sizeof(Packet::Header));
			}

		//Disconnected?
		} else if (len == 0) {
			printf("Disconnected\n");
			closesocket(serverSocket);
			this->connected = false;
		}

	//Loop while receiving data
	} while(len > 0);
}

/** Set receive callback to call in receive thread
 *  @param callback Call back function
 */
void Client::SetRecvCallback(Client_RecvCallback callback){
	recv_callback = callback;
}

/** Returns true if client is connected to a server
 */
bool Client::isConnected() {
	return this->connected;
}
