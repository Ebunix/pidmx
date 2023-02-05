#pragma once
#include <pidmx_utils.h>
#include <memory>
#include "ISerializable.h"
#include <js/js.h>

struct FixtureData {
	std::string name;
	Hash fixtureId = 0;
	Hash presetId = 0;
	int universe = 0;
	int channel = 0;
};

#ifdef PIDMX_ENABLE_JAVASCRIPT
V8_INTEROP_CONVERT_FUNC(FixtureData, ObjectToFixtureData) {
	V8_INTEROP_CONVERT_FUNC_PREAMBLE(FixtureData);
	V8_INTEROP_CONVERT_FUNC_MEMBER_STRING(name)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(Hash, fixtureId)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(Hash, presetId)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, universe)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, channel)
	return result;
}
#endif

class Fixture : public ISerializable {
public:
	Hash presetId = INVALID_HASH;
	int universe = 0;
	int channel = 0;

#ifdef PIDMX_ENABLE_JAVASCRIPT
    static void Patch(const v8::FunctionCallbackInfo<v8::Value> &info);
#endif

	static std::shared_ptr<Fixture> New(const FixtureData& data);

	void Load(const nbt::tag_compound& pack) override;
	nbt::tag_compound Save() override;

};
typedef std::shared_ptr<Fixture> FixtureInstance;
