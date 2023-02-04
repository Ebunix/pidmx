#ifdef PIDMX_ENABLE_JAVASCRIPT
#include "require.h"
#include "../jsUtils.h"
#include "../exec.h"

#include "../../src/engine/Console.h"
#include <stdio.h>

namespace js {
	namespace impl {
		void require(const v8::FunctionCallbackInfo<v8::Value>& info) {
			v8::Local<v8::Context> ctx = info.GetIsolate()->GetCurrentContext();

			if (!info[0]->IsString()) {
				LogMessage(LogMessageType_Error, "%s is not a string", V8CStr(ctx, info[0]).c_str());
				info.GetReturnValue().SetUndefined();
				return;
			}

			std::string str = V8CStr(ctx, info[0]);
			const char* path = str.c_str();
			v8::Local<v8::Value> result = js::execFile(path);
			info.GetReturnValue().Set(result);
		}
	}
}

#endif