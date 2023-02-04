#ifdef PIDMX_ENABLE_JAVASCRIPT

#include "globals.h"

namespace js {
	namespace global {
		std::unique_ptr<v8::Platform> platform;
		v8::Isolate* isolate;
		v8::Global<v8::Context> context;
	}
}

#endif
