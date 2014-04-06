//FuelClub-Client
#include "../Network/Packet.h"
#include "../Network/Client.h"

#include "Simulation.h"

//Client
Client* client = NULL;

//GUI
#include "GUI.h"

bool bSimulation = false;

//Client receive call back
void Client_Receive(Packet* packet) {
	if (packet->isEncrypted()) packet->crypt();
	//Do nothing, we don't receive any data
}

//Simulation Thread
void SimulationThread() {

	//Create simulation object
	Simulation* simulation = new Simulation(client, 1);

	//Simulation tick
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

				//'CONNECT' button
				case ID_CONNECT: {

					//Validate IP and Port
					if (isValidIP(GUI::getText(ID_IP)) && isValidPort(GUI::getText(ID_PORT))) {
						GUI::addListItem(ID_LIST, "Connecting to %s:%s", GUI::getText(ID_IP), GUI::getText(ID_PORT));

						//Create client
						client = new Client(GUI::getText(ID_IP), atoi(GUI::getText(ID_PORT)));

						//Disable 'CONNECT' button
						GUI::setEnabled(ID_CONNECT, false);

						//Wait before verifying connection
						Sleep(2000);

						//Client connected?
						if (!client->isConnected()){
							MessageBox(0, "Error : Could NOT connect!", "Error", 0);
							delete client;
							GUI::setEnabled(ID_CONNECT, true);
						}

					//Invalid IP or Port
					} else {
						MessageBox(0, "Error : Invalid IP/Port!", "Error", MB_ICONERROR);
					}
				}
				break;

				//'SIMULATE' button
				case ID_SIMULATE: {

					//Start simulation
					if (!bSimulation) {
						GUI::addListItem(ID_LIST, "Simulating...");

						//Create simulation thread
						HANDLE hSimulationThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SimulationThread, NULL, 0, NULL);

						GUI::setText(ID_SIMULATE, "STOP");

					//Stop simulation
					} else {
						GUI::addListItem(ID_LIST, "Stopping Simulation...");
						GUI::setText(ID_SIMULATE, "SIMULATE");
					}

					//Toggle simulation status
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

	//Create GUI
	if (!GUI::setDialogHandle(CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DIALOG), 0, DialogProc)))
		return -1;

	//Initialize GUI objects
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
