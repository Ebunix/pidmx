#pragma once
#include "v8.h"
#include "jsUtils.h"

#define V8_INTEROP_CONVERT_FUNC(type, func) inline type v8Interop ## func(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value)
#define V8_INTEROP_CONVERT_FUNC_PREAMBLE(structType) structType result
#define V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(type, name) v8::Local<v8::Value> name ## Temp; \
if (!value->IsUndefined() && value.As<v8::Object>()->Get(ctx, V8StrCheck(#name)).ToLocal(&name ## Temp)) { \
	if (name ## Temp->IsNumber()) { \
		result.name = (type)name ## Temp.As<v8::Number>()->Value(); \
	} \
}
#define V8_INTEROP_CONVERT_FUNC_MEMBER_STRING(name) v8::Local<v8::Value> name ## Temp; \
if (!value->IsUndefined() && value.As<v8::Object>()->Get(ctx, V8StrCheck(#name)).ToLocal(&name ## Temp)) { \
	if (name ## Temp->IsString()) { \
		result.name = V8CStr(ctx, name ## Temp); \
	} \
}

/*
V8_INTEROP_STRUCT(foo) {
	V8_INTEROP_STRUCT_MEMBER(int, valueInt);
	V8_INTEROP_STRUCT_MEMBER(float, someFloat);
	V8_INTEROP_STRUCT_MEMBER(std::string, ayyy);
};

V8_INTEROP_CONVERT_FUNC(foo, ObjectToFoo) {
	V8_INTEROP_CONVERT_FUNC_PREAMBLE(foo);
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, valueInt)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(float, someFloat)
	V8_INTEROP_CONVERT_FUNC_MEMBER_STRING(ayyy)
	return result;
}
*/