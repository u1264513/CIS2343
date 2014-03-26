#include "../Network/Server.h"
#include "../Network/Packet.h"
#include "../Network/Client.h"

void Server_Receive(Server::ServerClient* client, Packet* packet) {
	if (packet->isEncrypted()) packet->crypt();
	printf("\nClientID : %d\nServer Received Packet :\n\nMagic : 0x%08X (%s)\nLength : %d Bytes\nChecksum : 0x%08X (%s)\n\nData : %s\n\n", client->id, packet->magic(), (packet->isValidMagic() ? "VALID" : "INVALID"), packet->dataLength(), packet->checksum(), (packet->isValidChecksum() ? "VALID" : "INVALID"), packet->data());
}

void main(){
	Server* server = new Server(2663);
	Client* client = new Client("localhost", 2663);

	unsigned long BIG_TEST_SIZE = 32;
	char* BIG_TEST = (char*)malloc(BIG_TEST_SIZE);
	memset(BIG_TEST, 'A', BIG_TEST_SIZE);
	BIG_TEST[BIG_TEST_SIZE - 1] = 0x00;

	//Create packet
	Packet pkt((unsigned char*)BIG_TEST, BIG_TEST_SIZE);

	//Set server receive callback
	server->SetRecvCallback(Server_Receive);

	//Client send data
	//pkt.crypt();
	client->Send(&pkt);


	while (1)
		Sleep(100);

	system("pause");
}