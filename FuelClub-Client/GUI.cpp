#include "GUI.h"

HWND GUI::hDialog = NULL;

HWND GUI::getDialogHandle() {
	return hDialog;
}

HWND GUI::setDialogHandle(HWND handle) {
	hDialog = handle;
	return hDialog;
}

void GUI::addListItem(int ID, char* item, ...) {
	if (hDialog) {
		char* buffer = (char*)malloc(GUI_MAX_BUFFER);
		ZeroMemory(buffer, GUI_MAX_BUFFER);
		va_list arg;
		va_start(arg, item);
		vsnprintf(buffer, GUI_MAX_BUFFER, item, arg);
		va_end(arg);
		SendDlgItemMessage(hDialog, ID, LB_ADDSTRING, 0, (LPARAM)buffer);
		
		//Auto-Scroll
		SendDlgItemMessage(hDialog, ID, LB_SETCURSEL, SendDlgItemMessage(hDialog, ID, LB_GETCOUNT, 0, NULL)-1, NULL);
	}
}

void GUI::setEnabled(int ID, bool enabled) {
	EnableWindow(GetDlgItem(hDialog, ID), enabled);
}

void GUI::setText(int ID, char* text, ...) {
	if (hDialog) {
		char* buffer = (char*)malloc(GUI_MAX_BUFFER);
		ZeroMemory(buffer, GUI_MAX_BUFFER);
		va_list arg;
		va_start(arg, text);
		vsnprintf(buffer, GUI_MAX_BUFFER, text, arg);
		va_end(arg);
		SetDlgItemText(hDialog, ID, buffer);
	}
}

char* GUI::getText(int ID) {
	if (hDialog) {
		char* buffer = (char*)malloc(GUI_MAX_BUFFER);
		ZeroMemory(buffer, GUI_MAX_BUFFER);
		GetDlgItemText(hDialog, ID, buffer, GUI_MAX_BUFFER);
		return buffer;
	}
	return NULL;
}

void GUI::setFocus(int ID) {
	SetFocus(GetDlgItem(hDialog, ID));
}
