#include "console.h"
#include "../jsUtils.h"

namespace js {
	namespace impl {
		namespace console {

			void PrintMessage(ConsoleMessageType type, const char* fmt, const v8::FunctionCallbackInfo<v8::Value>& info) {
				std::string str;
				for (int i = 0; i < info.Length(); ++i) {
					std::string json;
					if (info[i]->IsString()) {
						json = V8CStr(info[i]);
					}
					else {
						json = V8CStr(v8::JSON::Stringify(info.GetIsolate()->GetCurrentContext(), info[i]).ToLocalChecked());

					}
					str.append(json.c_str());
					str.append(" ");
				}
				LogMessage(type, str.c_str());
			}

			void log(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(ConsoleMessageType_Info, "%s ", info);
			}
			void warn(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(ConsoleMessageType_Warn, "%s ", info);
			}
			void error(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(ConsoleMessageType_Error, "%s ", info);
			}
		}
	}
}
