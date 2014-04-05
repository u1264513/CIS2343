#pragma once
#include <Windows.h>
#include <stdio.h>

#define FILE_MAX_BUFFER 256

class FileHandler {
public:
	FileHandler(char* file);
	~FileHandler();

	bool Write(char* text, ...);

	void FileHandler::Close();

private:
	FILE* pFile;
};
