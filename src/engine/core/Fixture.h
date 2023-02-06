#pragma once
#include <pidmx_utils.h>
#include <memory>
#include "ISerializable.h"
#include <js/js.h>

struct FixtureData {
	std::string name;
	Hash presetId = 0;
	int universe = 0;
	int channel = 0;
};

#ifdef PIDMX_ENABLE_JAVASCRIPT
V8_INTEROP_CONVERT_FUNC(FixtureData, ObjectToFixtureData) {
	V8_INTEROP_CONVERT_FUNC_PREAMBLE(FixtureData);
	V8_INTEROP_CONVERT_FUNC_MEMBER_STRING(name)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(Hash, presetId)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, universe)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, channel)
	return result;
}
#endif

class Fixture {
public:
    Hash id = 0;
    FixtureData data;

#ifdef PIDMX_ENABLE_JAVASCRIPT
    static void Patch(const v8::FunctionCallbackInfo<v8::Value> &info);
#endif

};
typedef std::shared_ptr<Fixture> FixtureInstance;

NBT_SAVE(FixtureData, {
    NBT_SAVE_MEMBER(name);
    NBT_SAVE_MEMBER(presetId);
    NBT_SAVE_MEMBER(universe);
    NBT_SAVE_MEMBER(channel);
})
NBT_LOAD(FixtureData, {
    FixtureData value;
    NBT_LOAD_MEMBER(name);
    NBT_LOAD_MEMBER(presetId);
    NBT_LOAD_MEMBER(universe);
    NBT_LOAD_MEMBER(channel);
    return value;
})
NBT_SAVE(FixtureInstance, {
    NBT_SAVE_MEMBER_PTR(id);
    NBT_SAVE_MEMBER_PTR(data);
})
NBT_LOAD(FixtureInstance, {
    FixtureInstance value = std::make_shared<Fixture>();
    NBT_LOAD_MEMBER_PTR(id);
    NBT_LOAD_MEMBER_PTR(data);
    return value;
})