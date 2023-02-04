#include "Console.h"
#include <stdio.h>
#include <pidmx_utils.h>
#include <errno.h>  
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
int vasprintf(char** strp, const char* fmt, va_list ap)
{
    va_list ap_copy;
    int formattedLength, actualLength;
    size_t requiredSize;

    // be paranoid
    *strp = NULL;

    // copy va_list, as it is used twice 
    va_copy(ap_copy, ap);

    // compute length of formatted string, without NULL terminator
    formattedLength = _vscprintf(fmt, ap_copy);
    va_end(ap_copy);

    // bail out on error
    if (formattedLength < 0)
    {
        return -1;
    }

    // allocate buffer, with NULL terminator
    requiredSize = ((size_t)formattedLength) + 1;
    *strp = (char*)malloc(requiredSize);

    // bail out on failed memory allocation
    if (*strp == NULL)
    {
        errno = ENOMEM;
        return -1;
    }

    // write formatted string to buffer, use security hardened _s function
    actualLength = vsnprintf_s(*strp, requiredSize, requiredSize - 1, fmt, ap);

    // again, be paranoid
    if (actualLength != formattedLength)
    {
        free(*strp);
        *strp = NULL;
        errno = EOTHER;
        return -1;
    }

    return formattedLength;
}
#endif

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
