#pragma once
#include <Windows.h>
#include <stdio.h>

#include "resource.h"

#define GUI_MAX_BUFFER 256

static class GUI {
public:
	static HWND getDialogHandle();
	static HWND setDialogHandle(HWND handle);

	static void addListItem(int ID, char* item, ...);
	static void setEnabled(int ID, bool enabled);
	static void setText(int ID, char* text, ...);
	static char* getText(int ID);
	static void setFocus(int ID);

private:
	static HWND hDialog;
};
