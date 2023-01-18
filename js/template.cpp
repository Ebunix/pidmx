#include "template.h"
#include "globals.h"
#include "jsUtils.h"

js::ObjectTemplate* js::ObjectTemplate::Begin()
{
	functions.clear();
	getters.clear();
	setters.clear();
	return this;
}

js::ObjectTemplate *js::ObjectTemplate::Function(const char *name, v8::FunctionCallback callback)
{
	functions.insert(std::make_pair(name, callback));
	return this;
}

js::ObjectTemplate* js::ObjectTemplate::Accessor(const char *name, v8::AccessorGetterCallback getCallback, v8::AccessorSetterCallback setCallback)
{
	getters.insert(std::make_pair(name, getCallback));
	setters.insert(std::make_pair(name, setCallback));
	return this;
}

js::ObjectTemplate* js::ObjectTemplate::Object(const char *name, v8::Local<v8::ObjectTemplate> obj)
{
	objects.insert(std::make_pair(name, obj));
	return this;	
}

v8::Local<v8::ObjectTemplate> js::ObjectTemplate::Build()
{
	v8::Isolate* iso = js::global::isolate;

	v8::EscapableHandleScope handleScope(iso);
	for (const auto& entry : functions) {
		v8::Local<v8::FunctionTemplate> func = v8::FunctionTemplate::New(iso, entry.second);
		func->ReadOnlyPrototype();
		target->Set(iso, entry.first, func);
	}
	for (const auto& get : getters) {
		const v8::AccessorSetterCallback& set = setters.at(get.first);
		target->SetAccessor(V8StrCheck(get.first), get.second, set);
	}
	for (const auto& obj : objects) {
		target->Set(V8StrCheck(obj.first), obj.second);
	}
	target->SetInternalFieldCount(kNumFields);

	return handleScope.Escape(target);
}

void js::ObjectTemplate::BuildGlobal(const char *name)
{
	v8::Isolate* iso = js::global::isolate;
	v8::HandleScope handleScope(iso);
	v8::Local<v8::ObjectTemplate> templ = Build();
	v8::Local<v8::Context> context = js::global::context.Get(iso);
	v8::Local<v8::Object> instance;
	if (templ->NewInstance(context).ToLocal(&instance)) {
		context->Global()->Set(context, V8StrCheck(name), instance).Check();
	}
}
