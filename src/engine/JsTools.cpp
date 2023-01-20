#include "JsTools.h"
#include "../src/engine/Console.h"


using namespace v8;

ImVec4 JsValueToImVec4(v8::Local<v8::Context> ctx, const v8::MaybeLocal<v8::Value> &value, ImVec4 fallback)
{
	Local<Value> val;
	if (value.ToLocal(&val)) {
		return JsValueToImVec4(ctx, val, fallback);
	}
	LogMessage(ConsoleMessageType_Error,  "Empty value");
	return fallback;
}

ImVec4 JsValueToImVec4(v8::Local<v8::Context> ctx, const v8::Local<v8::Value> &value, ImVec4 fallback)
{
	ImVec4 result;
	Local<Value> temp;
	if (value->IsUndefined()) {
		return fallback;
	}
	else if (value->IsArray()) {
		Local<Array> array = value.As<Array>();
		for (int i = 0; i < array->Length() && i < 4; i++) {
			if (i == 0 && array->Get(ctx, i).ToLocal(&temp) && temp->IsNumber()) {
				result.x = (float)temp.As<Number>()->Value();
			}
			else if (i == 1 && array->Get(ctx, i).ToLocal(&temp) && temp->IsNumber()) {
				result.y = (float)temp.As<Number>()->Value();
			}
			else if (i == 2 && array->Get(ctx, i).ToLocal(&temp) && temp->IsNumber()) {
				result.z = (float)temp.As<Number>()->Value();
			}
			else if (i == 3 && array->Get(ctx, i).ToLocal(&temp) && temp->IsNumber()) {
				result.w = (float)temp.As<Number>()->Value();
			}
			else {
				LogMessage(ConsoleMessageType_Error,  "Invalid value");
			}
		}
	}
	else if (value->IsObject()) {
		Local<Object> obj = value.As<Object>();
		if (obj->Get(ctx, V8StrCheck("x")).ToLocal(&temp) && temp->IsNumber()) {
			result.x = (float)temp.As<Number>()->Value();
		}
		if (obj->Get(ctx, V8StrCheck("y")).ToLocal(&temp) && temp->IsNumber()) {
			result.y = (float)temp.As<Number>()->Value();
		}
		if (obj->Get(ctx, V8StrCheck("z")).ToLocal(&temp) && temp->IsNumber()) {
			result.z = (float)temp.As<Number>()->Value();
		}
		if (obj->Get(ctx, V8StrCheck("w")).ToLocal(&temp) && temp->IsNumber()) {
			result.w = (float)temp.As<Number>()->Value();
		}
	}
	else {
		LogMessage(ConsoleMessageType_Error,  "Value is not an array or object");
		return fallback;
	}
	return result;
}

std::vector<int> JsValueToIntArray(v8::Local<v8::Context> ctx, const v8::MaybeLocal<v8::Value> &value)
{
	Local<Value> val;
	if (value.ToLocal(&val)) {
		return JsValueToIntArray(ctx, val);
	}
	LogMessage(ConsoleMessageType_Error,  "Empty value");
	return std::vector<int>();
}

std::vector<int> JsValueToIntArray(v8::Local<v8::Context> ctx, const v8::Local<v8::Value> &value) {
	if (!value->IsArray()) {
		LogMessage(ConsoleMessageType_Error,  "Value is not an array");
		return std::vector<int>();
	}
	std::vector<int> result;
	Local<Value> temp;
	Local<Array> array = value.As<Array>();
	for (int i = 0; i < array->Length() && i < 4; i++) {
		if (array->Get(ctx, i).ToLocal(&temp) && temp->IsNumber()) {
			result.push_back((int)temp.As<Number>()->Value());
		}
	}
	return result;
}

std::vector<std::string> JsValueToStringArray(v8::Local<v8::Context> ctx, const v8::MaybeLocal<v8::Value> &value)
{
	Local<Value> val;
	if (value.ToLocal(&val)) {
		return JsValueToStringArray(ctx, val);
	}
	LogMessage(ConsoleMessageType_Error,  "Empty value");
	return std::vector<std::string>();
}

std::vector<std::string> JsValueToStringArray(v8::Local<v8::Context> ctx, const v8::Local<v8::Value> &value)
{
	if (!value->IsArray()) {
		LogMessage(ConsoleMessageType_Error,  "Value is not an array");
		return std::vector<std::string>();
	}
	std::vector<std::string> result;
	Local<Value> temp;
	Local<Array> array = value.As<Array>();
	for (int i = 0; i < array->Length() && i < 4; i++) {
		if (array->Get(ctx, i).ToLocal(&temp) && temp->IsString()) {
			result.push_back(V8CStr(temp));
		}
	}
	return result;
}

bool JsValueToSlot(v8::Local<v8::Context> ctx, v8::Local<v8::Value> &value, std::string *name, ImVec4 *tint) {
	if (value->IsString()) {
		*name = V8CStr(value);
		*tint = ImVec4(1, 1, 1, 1);
		return true;
	}
	if (!value->IsObject()) {
		*tint = ImVec4(1, 1, 1, 1);
		return false;
	}
	Local<Value> temp;
	Local<Object> obj = value.As<Object>();

	if (obj->Get(ctx, V8StrCheck("name")).ToLocal(&temp)) {
		*name = V8CStr(temp);
	}
	*tint = JsValueToImVec4(ctx, obj->Get(ctx, V8StrCheck("tint")), ImVec4(1, 1, 1, 1));
	return true;
}
