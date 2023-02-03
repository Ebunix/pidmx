#include "Console.h"
#include <stdio.h>
#include <pidmx_utils.h>
#include <stdarg.h>

int64_t __logStartTime = 0;
bool __logInitialized = false;
std::vector<ConsoleMessage> consolePanelLogBuffer;

using namespace std::chrono;

void logInit() {
	__logInitialized = true;
	__logStartTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	LogMessage(LogMessageType_Info, "Log start");
}

float __logTime() {
	if (!__logInitialized) {
		logInit();
	}
	uint64_t since = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - __logStartTime;
	return since / 1000.0f;
}

void LogMessage(LogMessageType type, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char* buffer;
    vasprintf(&buffer, fmt, args);
    va_end(args);

    consolePanelLogBuffer.push_back(ConsoleMessage { buffer, type });

    switch (type) {
        case LogMessageType_Debug: LOG_DEBUG_FORMAT("%s", buffer); break;
        case LogMessageType_Warn: LOG_WARN_FORMAT("%s", buffer); break;
        case LogMessageType_Error: LOG_ERROR_FORMAT("%s", buffer); break;
        default: LOG_INFO_FORMAT("%s", buffer); break;
    }
}
