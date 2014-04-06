//FuelClub-Server
#include "../Network/Server.h"
#include "../Network/Packet.h"

//GUI
#include "GUI.h"

//IO
#include "../FileHandler/FileHandler.h"

FileHandler* serverLog = NULL;
Server* server = NULL;

//Server receive call back
void Server_Receive(Server::ServerClient* client, Packet* packet) {
	if (packet->isEncrypted()) packet->crypt();

	GUI::addListItem(ID_LIST, "STATION #%d - %s", client->id, packet->data());
	serverLog->Write("STATION #%d - %s", client->id, packet->data());
}

bool isValidPort(char* port) {
	if (strlen(port) == 0) return false;
	for (int i=0;i<strlen(port);i++)
		if (port[i] < '0' || port[i] > '9')
			return false;
	return true;
}

BOOL CALLBACK DialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {

				//'LISTEN' button
				case ID_LISTEN: {

					//Validate port
					if (isValidPort(GUI::getText(ID_PORT))) {
						GUI::addListItem(ID_LIST, "Listening on port %s...", GUI::getText(ID_PORT));

						//Create server log
						serverLog = new FileHandler("data.log");

						//Create server
						server = new Server(atoi(GUI::getText(ID_PORT)));

						//Set server receive call back
						server->SetRecvCallback(Server_Receive);

						//Update GUI
						GUI::setText(ID_IP, server->getLocalIP());
						GUI::setEnabled(ID_LISTEN, false);

					//Invalid port
					} else {
						MessageBox(0, "Error : Invalid Port!", "Error", MB_ICONERROR);
					}
				}
				break;
			}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return FALSE;
    }
    return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//Create GUI
	if (!GUI::setDialogHandle(CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DIALOG), 0, DialogProc)))
		return -1;

	//Initialize GUI objects
	GUI::setText(ID_PORT, "2660");
	GUI::setText(ID_IP, "127.0.0.1");
	GUI::setFocus(ID_LISTEN);

	MSG  msg;
    int status;
    while ((status = GetMessage(&msg, 0, 0, 0)) != 0) {
        if (status == -1) return -1;
		if (!IsDialogMessage (GUI::getDialogHandle(), &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}
