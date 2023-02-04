#ifdef PIDMX_ENABLE_JAVASCRIPT
#include "init.h"
#include "v8.h"
#include "globals.h"
#include "jsUtils.h"
#include "template.h"
#include "impl/impl.h"

using namespace v8;

#define DEFINE_GLOBAL_CLASS(templ, name) templ->Set(js::global::isolate, #name, templates::name(FunctionTemplate::New(js::global::isolate, templates::ctor::name)))

bool jsInitRuntime(const char* argv0) {	
	LogMessage(LogMessageType_Debug, "Init ICU");
	if (!V8::InitializeICUDefaultLocation(argv0)) {
		LogMessage(LogMessageType_Error, "ICU init failed\n");
		return false;
	}
	LogMessage(LogMessageType_Debug, "Init external data");
	V8::InitializeExternalStartupData(argv0);
	js::global::platform = platform::NewDefaultPlatform();
	LogMessage(LogMessageType_Debug, "Init platform");
	V8::InitializePlatform(js::global::platform.get());
	if (!V8::Initialize()) {
		LogMessage(LogMessageType_Error, "V8 init failed");
		return false;
	}

	LogMessage(LogMessageType_Debug, "Create isolate");
	Isolate::CreateParams createParams {};
	createParams.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
	js::global::isolate = Isolate::New(createParams);
	js::global::isolate->Enter();

	HandleScope handleScope(js::global::isolate);
	
	Local<ObjectTemplate> globalTemplate = ObjectTemplate::New(js::global::isolate);
	globalTemplate->Set(js::global::isolate, "require", FunctionTemplate::New(js::global::isolate, js::impl::require));
	
	LogMessage(LogMessageType_Debug, "Create context");
	Local<Context> context = Context::New(js::global::isolate, nullptr, globalTemplate);
	js::global::context.Reset(js::global::isolate, context);

	LogMessage(LogMessageType_Debug, "Create globals");
	js::ObjectTemplate consoleBuilder;
	consoleBuilder.Begin()
		->Function("log", js::impl::console::log)
		->Function("warn", js::impl::console::warn)
		->Function("error", js::impl::console::error)
		->BuildGlobal("console");

	LogMessage(LogMessageType_Info, "V8 initialized");
	return true;
}


#endif