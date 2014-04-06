//FuelClub-Client
#include "../Network/Packet.h"
#include "../Network/Client.h"

#include "Simulation.h"

//Client
Client* client = NULL;

//GUI
#include "GUI.h"

bool bSimulation = false;

void Client_Receive(Packet* packet) {
	if (packet->isEncrypted()) packet->crypt();
}

void SimulationThread() {
	Simulation* simulation = new Simulation(client, 1);

	while(bSimulation) {
		simulation->Tick();
		Sleep(10);
	}
}

bool isValidIP(char* ip) {
	if (strlen(ip) == 0 || strlen(ip) > 16) return false;
	for (int i=0;i<strlen(ip); i++)
		if ((ip[i] < '0' || ip[i] > '9') && ip[i] != '.')
			return false;
	return true;
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
				case ID_CONNECT: {
					if (isValidIP(GUI::getText(ID_IP)) && isValidPort(GUI::getText(ID_PORT))) {
						GUI::addListItem(ID_LIST, "Connecting to %s:%s", GUI::getText(ID_IP), GUI::getText(ID_PORT));
						client = new Client(GUI::getText(ID_IP), atoi(GUI::getText(ID_PORT)));
						GUI::setEnabled(ID_CONNECT, false);
						Sleep(2000);
						if (!client->isConnected()){
							MessageBox(0, "Error : Could NOT connect!", "Error", 0);
							delete client;
							GUI::setEnabled(ID_CONNECT, true);
						}
					} else {
						MessageBox(0, "Error : Invalid IP/Port!", "Error", MB_ICONERROR);
					}
				}
				break;
				case ID_SIMULATE: {
					if (!bSimulation) {
						GUI::addListItem(ID_LIST, "Simulating...");
						HANDLE hSimulationThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SimulationThread, NULL, 0, NULL);
						GUI::setText(ID_SIMULATE, "STOP");
					} else {
						GUI::addListItem(ID_LIST, "Stopping Simulation...");
						GUI::setText(ID_SIMULATE, "SIMULATE");
					}
					bSimulation = !bSimulation;
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

	GUI::setText(ID_IP, "127.0.0.1");
	GUI::setText(ID_PORT, "2660");
	GUI::setFocus(ID_CONNECT);

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
