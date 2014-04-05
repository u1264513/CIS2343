#include "FileHandler.h"

FileHandler::FileHandler(char* file) {
	pFile = fopen(file, "wb");
}

FileHandler::~FileHandler() {
	Close();
}

bool FileHandler::Write(char* text, ...) {
	char* buffer = (char*)malloc(FILE_MAX_BUFFER);
	ZeroMemory(buffer, FILE_MAX_BUFFER);
	va_list arg;
	va_start(arg, text);
	vsnprintf(buffer, FILE_MAX_BUFFER, text, arg);
	va_end(arg);

	int ret = fprintf(pFile, "%s\n", buffer);
	free(buffer);

	if (ret > 0)
		return true;
	return false;
}

void FileHandler::Close() {
	fclose(pFile);
}
