#include "Fixture.h"
#include <engine/Console.h>
#include "Show.h"
#include <engine/command/CommandFixture.h>

using namespace v8;

FixtureInstance Fixture::New(const FixtureData &data) {
    auto fix = std::make_shared<Fixture>();
    fix->id = data.fixtureId;
    fix->name = data.name;
    fix->presetId = data.presetId;
    fix->universe = data.universe;
    fix->channel = data.channel;
    return fix;
}

void Fixture::load(const nbt::tag_compound &pack) {
    ISerializable::load(pack);
    presetId = pack.at("preset").as<nbt::tag_long>().get();
    universe = pack.at("universe").as<nbt::tag_int>().get();
    channel = pack.at("channel").as<nbt::tag_int>().get();
}

nbt::tag_compound Fixture::save() {
    nbt::tag_compound comp = ISerializable::save();
    comp.insert("preset", presetId);
    comp.insert("universe", universe);
    comp.insert("channel", channel);
    return comp;
}

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
