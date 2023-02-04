#pragma once
#ifdef PIDMX_ENABLE_JAVASCRIPT

#include <memory>
#include "v8.h"

namespace js {
	namespace global {
		extern std::unique_ptr<v8::Platform> platform;
		extern v8::Isolate* isolate;
		extern v8::Global<v8::Context> context;
	}
}

#endif