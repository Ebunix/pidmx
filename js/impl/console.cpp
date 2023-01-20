#include "console.h"
#include "../jsUtils.h"

extern std::vector<std::string> consolePanelLogBuffer;

namespace js {
	namespace impl {
		namespace console {

			void PrintMessage(const char* fmt, const v8::FunctionCallbackInfo<v8::Value>& info) {
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
					printf(fmt, json.c_str());
				}
				consolePanelLogBuffer.push_back(str);
			}

			void log(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage("%s ", info);
				printf("\n");
			}
			void warn(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(CON_YELLOW "%s ", info);
				printf(CON_RESET "\n");
			}
			void error(const v8::FunctionCallbackInfo<v8::Value>& info) {
				PrintMessage(CON_RED "%s ", info);
				printf(CON_RESET "\n");
			}
		}
	}
}