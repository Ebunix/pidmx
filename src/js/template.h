#pragma once
#ifdef PIDMX_ENABLE_JAVASCRIPT

#include "globals.h"
#include "v8.h"
#include <map>

namespace js {
	enum ObjectTemplateField {
		DataPointer,
		PrivateMap,
		kNumFields
	};

	typedef std::vector<std::pair<std::string, v8::Global<v8::Private>*>> PrivateMapObject;

	struct ObjectTemplateResult {
		v8::Local<v8::ObjectTemplate> templ;
		PrivateMapObject* privates;
	};

	class ObjectTemplate {
	private:
		std::map<const char*, v8::FunctionCallback> functions;
		std::map<const char*, v8::AccessorGetterCallback> getters;
		std::map<const char*, v8::AccessorSetterCallback> setters;
		std::map<const char*, v8::Local<v8::ObjectTemplate>> objects;
		std::vector<const char*> privates;
		v8::Local<v8::ObjectTemplate> target;
	
	public:
		ObjectTemplate() : target(v8::ObjectTemplate::New(js::global::isolate)) {};
		ObjectTemplate(v8::Local<v8::ObjectTemplate> templ) : target(templ) {};
		ObjectTemplate* Begin();
		ObjectTemplate* Function(const char* name, v8::FunctionCallback callback);
		ObjectTemplate* Accessor(const char* name, v8::AccessorGetterCallback getCallback, v8::AccessorSetterCallback setCallback);
		ObjectTemplate* Object(const char* name, v8::Local<v8::ObjectTemplate> obj);
		ObjectTemplate* Private(const char* name);
	
		v8::Local<v8::ObjectTemplate> Build();
		void BuildGlobal(const char* name);
	};
}

#endif
