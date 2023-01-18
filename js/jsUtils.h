#pragma once
#include "v8.h"
#include "../src/pidmx_utils.h"
#include "globals.h"
#include "template.h"

inline v8::MaybeLocal<v8::String> V8Str(const std::string& str) {
	return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), str.c_str());
}
inline v8::Local<v8::String> V8StrCheck(const std::string& str) {
	return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), str.c_str()).ToLocalChecked();
}
inline v8::Local<v8::String> ValueToString(const v8::Local<v8::Value>& val) {
	v8::Local<v8::String> result;
	if (val->ToString(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocal(&result)) {
		return result;
	}
	return V8StrCheck("<unknown>");
}
inline std::string V8CStr(const v8::Local<v8::Value>& val) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::String::Utf8Value result(isolate, ValueToString(val));
	std::string cppStr(*result);
	return cppStr;
}

template<typename T>
inline T* Unwrap(const v8::Local<v8::Object>& obj) {
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(obj->GetInternalField(js::ObjectTemplateField::DataPointer));
	return (T*)wrap->Value();
}
template<typename T>
inline v8::Local<T> Walk(const v8::Local<v8::Value>& obj, const char* name) {
	return obj.As<v8::Object>()->Get(js::global::context.Get(js::global::isolate), V8StrCheck(name)).ToLocalChecked().As<T>();
}
namespace js {
	template<typename T>
	struct BackedField {
		T value;
		v8::Persistent<v8::Private> backing;
	};
}