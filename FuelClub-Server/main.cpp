//FuelClub-Server
#include "../Network/Server.h"
#include "../Network/Packet.h"

//GUI
#include "GUI.h"

//IO
#include "../FileHandler/FileHandler.h"

FileHandler* serverLog = NULL;
Server* server = NULL;

void Server_Receive(Server::ServerClient* client, Packet* packet) {
	if (packet->isEncrypted()) packet->crypt();

	GUI::addListItem(ID_LIST, "STATION #%d - %s", client->id, packet->data());
	serverLog->Write("STATION #%d - %s", client->id, packet->data());
}

BOOL CALLBACK DialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case ID_LISTEN: {
					GUI::addListItem(ID_LIST, "Listening on port %s...", GUI::getText(ID_PORT));
					serverLog = new FileHandler("data.log");
					server = new Server(atoi(GUI::getText(ID_PORT)));
					server->SetRecvCallback(Server_Receive);
					GUI::setText(ID_IP, server->getLocalIP());
					GUI::setEnabled(ID_LISTEN, false);
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
	if (!GUI::setDialogHandle(CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DIALOG), 0, DialogProc)))
		return -1;

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
