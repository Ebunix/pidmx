#include "exec.h"
#include "../engine/Console.h"
#include "globals.h"
#include "jsUtils.h"

namespace js {
	v8::Local<v8::Value> exec(const char* code, const std::string& context) {
		if (code == nullptr) {
			LogMessage(LogMessageType_Error, "Passed nullptr as JavaScript source");
			return v8::Undefined(js::global::isolate);
		}

		v8::Isolate* iso = v8::Isolate::GetCurrent();
		v8::Isolate::Scope isolateScope(iso);
		v8::EscapableHandleScope handleScope(iso);
		v8::Local<v8::Context> ctx = iso->GetCurrentContext();
		if (ctx.IsEmpty()) {
			ctx = js::global::context.Get(iso);
		}
		v8::Context::Scope contextScope(ctx);
		{
			v8::TryCatch tryCatch(iso);
			v8::ScriptOrigin origin(iso, V8StrCheck(context));
			v8::Local<v8::String> source = v8::String::NewFromUtf8(iso, code).ToLocalChecked();
			v8::MaybeLocal<v8::Script> scriptMaybe = v8::Script::Compile(ctx, source, &origin);
			v8::Local<v8::Script> script;
			if (scriptMaybe.ToLocal(&script)) {
				v8::MaybeLocal<v8::Value> result = script->Run(ctx);
				if (tryCatch.HasCaught()) {
					LogMessage(LogMessageType_Error, "%s", V8CStr(ctx, tryCatch.Message()->Get()).c_str());
				}
				v8::Local<v8::Value> temp;
				if (result.ToLocal(&temp)) {
					return handleScope.Escape(temp);
				}
				return handleScope.Escape(v8::Undefined(iso));
			}
			else {
				if (tryCatch.HasCaught()) {
					LogMessage(LogMessageType_Error, "%s", V8CStr(ctx, tryCatch.Message()->Get()).c_str());
				}
				LogMessage(LogMessageType_Error, "Failed to compile script");
			}
		}
		return handleScope.Escape(v8::Undefined(iso));
	}
	v8::Local<v8::Value> execFile(const char* path) {
		FILE* f = fopen(path, "rb");
		if (!f) {
			LogMessage(LogMessageType_Error, "%s was not found", path);
			return v8::Undefined(js::global::isolate);
		}
		fseeko(f, 0, SEEK_END);
		off_t len = ftello(f);
		fseeko(f, 0, SEEK_SET);
		if (len > 0x2000000) { // 32 MB
			LogMessage(LogMessageType_Error, "%s exceeds 32 MB script limit", path);
			fclose(f);
			return v8::Undefined(js::global::isolate);
		}

		char* data = (char*)malloc(len+1);
		size_t read = fread(data, 1, len, f);
		data[len] = 0;
		fclose(f);

		if (read != len) {
			LogMessage(LogMessageType_Error, "Invalid byte count read from %s: expected %lu, got %lu", path, len, read);
			return v8::Undefined(js::global::isolate);
		}

		LogMessage(LogMessageType_Info, "require %s", path);
		v8::Local<v8::Value> result = js::exec(data);
		free(data);
		return result;
	}
}
	