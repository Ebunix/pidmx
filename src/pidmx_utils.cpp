#include "pidmx_utils.h"
#include <cstring>

bool FixturePatchStringToUniverseChannel(char *text, size_t len, int *universe, int *channel)
{
	char data[10];
	if (len > sizeof(data) || strlen(text) == 0) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if ((text[i] < '0' || text[i] > '9') && text[i] != '.' && text[i] != 0) {
			return false;
		}
	}
	strncpy(data, text, sizeof(data));

	char* channelText = nullptr;
	char* universeText = nullptr;

	char* separator = strchr(data, '.');
	if (separator) {
		*separator = 0;
		channelText = separator + 1;
		universeText = data;
	}
	else {
		channelText = data;
	}

	if (universeText) {
		*universe = atoi(universeText);
	}
	else {
		*universe = 0;
	}
	if (channelText) {
		*channel = atoi(channelText);
	}
	else {
		*channel = 0;
	}

	return *channel >= 1 && *channel <= 512;
}
