#include "Fixture.h"
#include <engine/Console.h>
#include "Show.h"
#include <engine/command/CommandFixture.h>

#ifdef PIDMX_ENABLE_JAVASCRIPT
using namespace v8;

void Fixture::Patch(const v8::FunctionCallbackInfo<v8::Value> &info) {
    std::vector<FixtureData> data;
    Local<Context> ctx = info.GetIsolate()->GetCurrentContext();

    if (info[0]->IsArray()) {
        Local<Array> array = info[0].As<Array>();
        for (uint32_t i = 0; i < array->Length(); i++) {
            Local<Value> val;
            if (array->Get(ctx, i).ToLocal(&val)) {
                data.push_back(v8InteropObjectToFixtureData(ctx, val));
            }
        }
    } else if (info[0]->IsObject()) {
        data.push_back(v8InteropObjectToFixtureData(ctx, info[0]));
    }

    currentShow->commandHistory.Push("Patch fixtures", CommandFixtureAdd::New(data));
}
#endif
