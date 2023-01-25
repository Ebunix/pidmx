#pragma once
#include "v8.h"
#include "../src/engine/Console.h"
#include "globals.h"
#include "template.h"

inline v8::MaybeLocal<v8::String> V8Str(const std::string& str) {
	return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), str.c_str());
}
inline v8::Local<v8::String> V8StrCheck(const std::string& str) {
	return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), str.c_str()).ToLocalChecked();
}
inline v8::Local<v8::String> ValueToString(const v8::Local<v8::Context>& ctx, const v8::Local<v8::Value>& val) {
	v8::Local<v8::String> result;
	if (val->ToString(ctx).ToLocal(&result)) {
		return result;
	}
	return V8StrCheck("<unknown>");
}
inline std::string V8CStr(const v8::Local<v8::Context>& ctx, const v8::Local<v8::Value>& val) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::String::Utf8Value result(isolate, ValueToString(ctx, val));
	std::string cppStr(*result);
	return cppStr;
}

template<typename T>
inline T* Unwrap(const v8::Local<v8::Object>& obj) {
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(obj->GetInternalField(js::ObjectTemplateField::DataPointer));
	return (T*)wrap->Value();
}
template<typename T>
inline bool Walk(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, const std::string& name, v8::Local<T>* output) {
	v8::Local<v8::Value> temp;
	bool result = obj->Get(ctx, V8StrCheck(name)).ToLocal(&temp);
	if (result) {
		*output = temp.As<T>();
	}
	return result && !temp->IsUndefined();
}
template<typename T>
inline bool Walk(v8::Local<v8::Context> ctx, v8::Local<v8::Array> obj, int i, v8::Local<T>* output) {
	v8::Local<v8::Value> temp;
	bool result = obj->Get(ctx, i).ToLocal(&temp);
	if (result) {
		*output = temp.As<T>();
	}
	return result && !temp->IsUndefined();
}
namespace js {
	template<typename T>
	struct BackedField {
		T value;
		v8::Persistent<v8::Private> backing;
	};
}