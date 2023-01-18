#include "pidmx_utils.h"

int64_t __logStartTime = 0;
bool __logInitialized = false;

using namespace std::chrono;

void logInit() {
	__logInitialized = true;
	__logStartTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	LOG_INFO("Log start");
}

float __logTime() {
	if (!__logInitialized) {
		logInit();
	}
	uint64_t since = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - __logStartTime;
	return since / 1000.0f;
}
