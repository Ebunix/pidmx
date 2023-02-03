#include "console.h"
#include "../jsUtils.h"

namespace js {
	namespace impl {
		namespace console {

			void PrintMessage(LogMessageType type, const char* fmt, const v8::FunctionCallbackInfo<v8::Value>& info) {
				v8::Local<v8::Context> ctx = info.GetIsolate()->GetCurrentContext();
				std::string str;
				for (int i = 0; i < info.Length(); ++i) {
					std::string json;
					if (info[i]->IsString()) {
						json = V8CStr(ctx, info[i]);
					}
					else {
						json = V8CStr(ctx, v8::JSON::Stringify(ctx, info[i]).ToLocalChecked());

					}
					str.append(json.c_str());
					str.append(" ");
				}
				LogMessage(type, str.c_str());
			}

			void log(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(LogMessageType_Info, "%s ", info);
			}
			void warn(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(LogMessageType_Warn, "%s ", info);
			}
			void error(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(LogMessageType_Error, "%s ", info);
			}
		}
	}
}
