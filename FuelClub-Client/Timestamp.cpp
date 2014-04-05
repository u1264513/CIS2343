#include "Timestamp.h"
#include <stdlib.h>

char* Timestamp::getTimestamp(time_t timestamp) {
	char* buffer = (char*)malloc(32);
	struct tm* t = localtime(&timestamp);
	strftime(buffer, 32, "%Y-%m-%d-%X", t);
	return buffer;
}
