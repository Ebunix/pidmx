#include "exec.h"
#include "../src/pidmx_utils.h"
#include "globals.h"
#include "jsUtils.h"

namespace js {
	v8::Local<v8::Value> exec(const char* code) {
		if (code == nullptr) {
			LOG_ERROR("Passed nullptr as JavaScript source");
			exit(-1);
		}

		v8::Isolate* iso = v8::Isolate::GetCurrent();
		v8::Isolate::Scope isolateScope(iso);
		v8::EscapableHandleScope handleScope(iso);
		v8::Local<v8::Context> context = iso->GetCurrentContext();
		if (context.IsEmpty()) {
			context = js::global::context.Get(iso);
		}
		v8::Context::Scope contextScope(context);
		{
			v8::ScriptOrigin origin(iso, V8StrCheck("undefined context"));
			v8::Local<v8::String> source = v8::String::NewFromUtf8(iso, code).ToLocalChecked();
			v8::MaybeLocal<v8::Script> scriptMaybe = v8::Script::Compile(context, source, &origin);
			v8::Local<v8::Script> script;
			if (scriptMaybe.ToLocal(&script)) {
				v8::MaybeLocal<v8::Value> result = script->Run(context);
				if (result.IsEmpty()) {
					return handleScope.Escape(v8::Undefined(iso));
				}
				return handleScope.Escape(result.ToLocalChecked());
			}
			else {
				LOG_ERROR("Failed to compile script");
				exit(-1);
			}
		}
	}
	v8::Local<v8::Value> execFile(const char* path) {
		FILE* f = fopen(path, "rb");
		if (!f) {
			LOG_ERROR_FORMAT("%s was not found", path);
			return v8::Undefined(js::global::isolate);
		}
		fseeko(f, 0, SEEK_END);
		off_t len = ftello(f);
		fseeko(f, 0, SEEK_SET);
		if (len > 0x2000000) { // 32 MB
			LOG_ERROR_FORMAT("%s exceeds 32 MB script limit", path);
			fclose(f);
			return v8::Undefined(js::global::isolate);
		}

		char* data = (char*)malloc(len+1);
		size_t read = fread(data, 1, len, f);
		data[len] = 0;
		fclose(f);

		if (read != len) {
			LOG_ERROR_FORMAT("Invalid byte count read from %s: expected %lu, got %lu", path, len, read);
			return v8::Undefined(js::global::isolate);
		}

		LOG_INFO_FORMAT("require %s", path);
		v8::Local<v8::Value> result = js::exec(data);
		free(data);
		return result;
	}
}
	