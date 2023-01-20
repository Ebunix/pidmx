#pragma once
#include <imgui.h>
#include <js.h>

ImVec4 JsValueToImVec4(v8::Local<v8::Context> ctx, const v8::MaybeLocal<v8::Value>& value, ImVec4 fallback = ImVec4());
ImVec4 JsValueToImVec4(v8::Local<v8::Context> ctx, const v8::Local<v8::Value>& value, ImVec4 fallback = ImVec4());
std::vector<int> JsValueToIntArray(v8::Local<v8::Context> ctx, const v8::MaybeLocal<v8::Value>& value);
std::vector<int> JsValueToIntArray(v8::Local<v8::Context> ctx, const v8::Local<v8::Value>& value);
std::vector<std::string> JsValueToStringArray(v8::Local<v8::Context> ctx, const v8::MaybeLocal<v8::Value>& value);
std::vector<std::string> JsValueToStringArray(v8::Local<v8::Context> ctx, const v8::Local<v8::Value>& value);

bool JsValueToSlot(v8::Local<v8::Context> ctx, v8::Local<v8::Value>& value, std::string* name, ImVec4* tint);
