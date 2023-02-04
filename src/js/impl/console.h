#pragma once
#ifdef PIDMX_ENABLE_JAVASCRIPT
#include "../v8.h"
namespace js {
	namespace impl {
		namespace console {
			void log(const v8::FunctionCallbackInfo<v8::Value>& info);
			void warn(const v8::FunctionCallbackInfo<v8::Value>& info);
			void error(const v8::FunctionCallbackInfo<v8::Value>& info);
		}
	}
}
#endif
