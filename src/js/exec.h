#pragma once
#include "v8.h"

namespace js {
	v8::Local<v8::Value> exec(const char* code, const std::string& context = "undefined context");
	v8::Local<v8::Value> execFile(const char* path);
}