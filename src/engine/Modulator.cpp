#include "Modulator.h"
#include <pidmx_utils.h>
#include "Engine.h"

using namespace v8;

void Modulator::Register(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() != 2) {
		LogMessage(ConsoleMessageType_Error,  "Wrong number of arguments. Expected 2, got %i", info.Length());
		js::global::isolate->ThrowError(V8StrCheck("Wrong number of arguments."));
		return;
	}

	Local<Context> ctx = info.GetIsolate()->GetCurrentContext();
	Local<Value> temp;

	std::string identifier = V8CStr(ctx, info[0]);
	Local<Object> data = info[1].As<Object>();
	Local<Value> name;
	Local<Function> op;
	Local<Object> parameters;
	Walk(ctx, data, "name", &name);
	Walk(ctx, data, "operator", &op);
	Walk(ctx, data, "parameters", &parameters);

	Modulator* mod = new Modulator(identifier, true);
	Local<Array> keys = parameters->GetOwnPropertyNames(ctx).ToLocalChecked();

	for (int i = 0; i < keys->Length(); i++) {
		Local<Value> key = keys->Get(ctx, i).ToLocalChecked();
		float value = (float)parameters->Get(ctx, key).ToLocalChecked().As<Number>()->Value();
		mod->SetParameter(V8CStr(ctx, key), value);
	}
	mod->name = V8CStr(ctx, name);
	mod->op.Reset(info.GetIsolate(), op);

	engine->modulators.push_back(mod);
}

void Modulator::SetParameter(const std::string &name, float value)
{
	for (auto& pair : parameters) {
		if (name.compare(pair.first) == 0) {
			pair.second = value;
			return;
		}
	}
	parameters.push_back(std::make_pair(std::string(name), value));
}

void Modulator::SetInput(const std::string &name, Modulator *mod)
{
	uint64_t hash = HashString(name);
	auto index = inputs.find(hash);
	if (mod == nullptr && index != inputs.end()) {
		inputs.erase(index);
	}
	else if (mod) {
		inputs.insert_or_assign(hash, mod);
	}
}

void Modulator::PrepareUpdate()
{
	isUpdated = false;
	for (const auto& pair : inputs) {
		pair.second->PrepareUpdate();
	}
}

void Modulator::Update()
{
	if (isRoot) {
		fprintf(stderr, "Trying to update a root modulator, which is invalid.\n");
		return;
	}
	isUpdated = true;
	
	HandleScope scope(js::global::isolate);
	Isolate* iso = js::global::isolate;
	Local<Context> ctx = js::global::context.Get(iso);
	ctx->Enter();
	Local<Object> params = Object::New(iso);

	for (const auto& pair : parameters) {
		float value = pair.second;
		uint64_t hash = HashString(pair.first);
		const auto& modIndex = inputs.find(hash);
		if (modIndex != inputs.end()) {
			value = modIndex->second->GetValue();
		}
		params->Set(ctx, V8StrCheck(pair.first), Number::New(iso, value)).Check();
	}
	
	Local<Value> argv[] = { Number::New(iso, engine->GetTime()), params };
	Local<Value> maybe;
	if (op.Get(iso)->Call(ctx, Undefined(iso), 2, argv).ToLocal(&maybe)) {
		if (!maybe->IsNumber()) {
			LogMessage(ConsoleMessageType_Error,  "Operator `%s` returned non-number!", identifier.c_str());
			value = 0;
		}
		else {
			value = maybe.As<Number>()->Value();
		}
	}
}

float Modulator::GetValue()
{
	if (!isUpdated) {
		Update();
	}
	return value;
}

Modulator *Modulator::Clone()
{
	Modulator* mod = new Modulator(name);
	mod->op.Reset(js::global::isolate, op);
	mod->isRoot = false;
	for (const auto& param : parameters) {
		mod->parameters.push_back(std::make_pair(param.first, param.second));
	}
	return mod;
}
