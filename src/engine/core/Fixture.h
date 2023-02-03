#pragma once
#include <pidmx_utils.h>
#include <memory>
#include "ISerializable.h"
#include <v8ObjectInterface.h>

V8_INTEROP_STRUCT(FixtureData) {
	V8_INTEROP_STRUCT_MEMBER(std::string, name);
	V8_INTEROP_STRUCT_MEMBER(Hash, fixtureId) = 0;
	V8_INTEROP_STRUCT_MEMBER(Hash, presetId) = 0;
	V8_INTEROP_STRUCT_MEMBER(int, universe) = 0;
	V8_INTEROP_STRUCT_MEMBER(int, channel) = 0;
};

V8_INTEROP_CONVERT_FUNC(FixtureData, ObjectToFixtureData) {
	V8_INTEROP_CONVERT_FUNC_PREAMBLE(FixtureData);
	V8_INTEROP_CONVERT_FUNC_MEMBER_STRING(name)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(Hash, fixtureId)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(Hash, presetId)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, universe)
	V8_INTEROP_CONVERT_FUNC_MEMBER_NUMBER(int, channel)
	return result;
}

class Fixture : public ISerializable {
public:
	Hash presetId = INVALID_HASH;
	int universe = 0;
	int channel = 0;

    static void Patch(const v8::FunctionCallbackInfo<v8::Value> &info);
	static std::shared_ptr<Fixture> New(const FixtureData& data);

	void load(const nbt::tag_compound& pack) override;
	nbt::tag_compound save() override;

};
typedef std::shared_ptr<Fixture> FixtureInstance;
