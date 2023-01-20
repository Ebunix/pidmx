#include "require.h"
#include "../jsUtils.h"
#include "../exec.h"

#include "../../src/engine/Console.h"
#include <stdio.h>

namespace js {
	namespace impl {
		void require(const v8::FunctionCallbackInfo<v8::Value>& info) {
			if (!info[0]->IsString()) {
				LogMessage(ConsoleMessageType_Error, "%s is not a string", V8CStr(info[0]).c_str());
				info.GetReturnValue().SetUndefined();
				return;
			}

			std::string str = V8CStr(info[0]);
			const char* path = str.c_str();
			v8::Local<v8::Value> result = js::execFile(path);
			info.GetReturnValue().Set(result);
		}
	}
} 