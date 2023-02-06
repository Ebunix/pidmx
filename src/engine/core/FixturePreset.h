#pragma once
#include <pidmx_utils.h>
#include <memory>
#include "js/v8ObjectInterface.h"
#include "ISerializable.h"
#include "Collections.h"

enum FixtureParameterType {
	FixtureParameterType_Unassigned,
	FixtureParameterType_Dim,
	FixtureParameterType_ColorR,
	FixtureParameterType_ColorG,
	FixtureParameterType_ColorB,
};
NBT_SAVE_LOAD_SIMPLE(FixtureParameterType, int32_t, tag_int, tag_type::Int)

static const char* FixtureParameterTypeNames[] = {
        "Unassigned",
        "Dim",
        "Red",
        "Green",
        "Blue"
};

struct FixtureParameter {
    Hash id = 0;
	std::string name;
	int channelCoarse = -1;
	int channelFine = -1;
	int channelUltraFine = -1;
	int defaultValue = 0;
	int highlightValue = -1;
	FixtureParameterType type = FixtureParameterType_Unassigned;

    bool operator==(const FixtureParameter& other) const {
        return other.id == id;
    }
};
ANKERL_HASH(FixtureParameter)
NBT_SAVE(FixtureParameter, {
    FixtureParameter defaultValues;
    NBT_SAVE_MEMBER_COND(name, value.name != defaultValues.name);
    NBT_SAVE_MEMBER_COND(channelCoarse, value.channelCoarse != defaultValues.channelCoarse);
    NBT_SAVE_MEMBER_COND(channelFine, value.channelFine != defaultValues.channelFine);
    NBT_SAVE_MEMBER_COND(channelUltraFine, value.channelUltraFine != defaultValues.channelUltraFine);
    NBT_SAVE_MEMBER_COND(defaultValue, value.defaultValue != defaultValues.defaultValue);
    NBT_SAVE_MEMBER_COND(highlightValue, value.highlightValue != defaultValues.highlightValue);
    NBT_SAVE_MEMBER_COND(type, value.type != defaultValues.type);
})
NBT_LOAD(FixtureParameter, {
    FixtureParameter value;
    NBT_LOAD_MEMBER(name);
    NBT_LOAD_MEMBER(channelCoarse);
    NBT_LOAD_MEMBER(channelFine);
    NBT_LOAD_MEMBER(channelUltraFine);
    NBT_LOAD_MEMBER(defaultValue);
    NBT_LOAD_MEMBER(highlightValue);
    NBT_LOAD_MEMBER(type);
    return value;
})

struct FixturePreset {
    Hash id = 0;
    std::string name;
    std::string manufacturer;
    int footprint = 0;
    Set<FixtureParameter> parameters;

    bool operator==(const FixturePreset& other) const {
        return other.id == id;
    }
};
typedef std::shared_ptr<FixturePreset> FixturePresetInstance;
ANKERL_HASH(FixturePreset)
NBT_SAVE(FixturePreset, {
    FixturePreset defaultValues;
    NBT_SAVE_MEMBER(name);
    NBT_SAVE_MEMBER_COND(manufacturer, value.manufacturer != defaultValues.manufacturer);
    NBT_SAVE_MEMBER_COND(footprint, value.footprint != defaultValues.footprint);
    NBT_SAVE_MEMBER(parameters);
})
NBT_LOAD(FixturePreset, {
    FixturePreset value;
    NBT_LOAD_MEMBER(name);
    NBT_LOAD_MEMBER(manufacturer);
    NBT_LOAD_MEMBER(footprint);
    NBT_LOAD_MEMBER(parameters);
    return value;
})


