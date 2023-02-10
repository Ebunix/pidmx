#pragma once
#include <pidmx_utils.h>
#include <memory>
#include "js/v8ObjectInterface.h"
#include "NbtFileIO.h"
#include "Collections.h"
#include "IIdentifiable.h"

enum FixtureParameterType {
	FixtureParameterType_Unassigned,
	FixtureParameterType_Dim,
	FixtureParameterType_ColorR,
	FixtureParameterType_ColorG,
	FixtureParameterType_ColorB,
    FixtureParameterType_Count_
};
NBT_ENUM_SERIALIZER_IMPL(FixtureParameterType)

static const char* FixtureParameterTypeNames[] = {
        "Unassigned",
        "Dim",
        "Red",
        "Green",
        "Blue"
};

struct FixtureParameter: IIdentifiable {
	std::string name;
	int channelCoarse = -1;
	int channelFine = -1;
	int channelUltraFine = -1;
	int defaultValue = 0;
	int highlightValue = -1;
	FixtureParameterType type = FixtureParameterType_Unassigned;

    nbt::tag_compound Save() const override {
        nbt::tag_compound c = IIdentifiable::Save();
        c.insert("name", nbt::Serialize(name));
        c.insert("channelCoarse", nbt::Serialize(channelCoarse));
        c.insert("channelFine", nbt::Serialize(channelFine));
        c.insert("channelUltraFine", nbt::Serialize(channelUltraFine));
        c.insert("defaultValue", nbt::Serialize(defaultValue));
        c.insert("highlightValue", nbt::Serialize(highlightValue));
        c.insert("type", nbt::Serialize(type));
        return c;
    }

    void Load(const nbt::tag_compound &c) override {
        IIdentifiable::Load(c);
        name = nbt::Deserialize(c, "name", std::string(""));
        channelCoarse = nbt::Deserialize(c, "channelCoarse", -1);
        channelFine = nbt::Deserialize(c, "channelFine", -1);
        channelUltraFine = nbt::Deserialize(c, "channelUltraFine", -1);
        defaultValue = nbt::Deserialize(c, "defaultValue", 0);
        highlightValue = nbt::Deserialize(c, "highlightValue", -1);
        type = (FixtureParameterType)nbt::Deserialize(c, "type", FixtureParameterType_Unassigned);
    }

};
typedef std::shared_ptr<FixtureParameter> FixtureParameterInstance;

struct FixturePreset: IIdentifiable {
    std::string name;
    std::string manufacturer;
    int footprint = 0;
    Set<FixtureParameterInstance> parameters;

    nbt::tag_compound Save() const override {
        nbt::tag_compound c = IIdentifiable::Save();
        c.insert("name", nbt::Serialize(name));
        c.insert("manufacturer", nbt::Serialize(manufacturer));
        c.insert("footprint", nbt::Serialize(footprint));
        c.insert("parameters", nbt::Serialize(parameters.begin(), parameters.end()));
        return c;
    }

    void Load(const nbt::tag_compound &c) override {
        IIdentifiable::Load(c);
        name = nbt::Deserialize(c, "name", std::string(""));
        manufacturer = nbt::Deserialize(c, "manufacturer", std::string(""));
        footprint = nbt::Deserialize(c, "footprint", 0);
        parameters = nbt::Deserialize(c, "parameters", Set<FixtureParameterInstance>());
    }
};
typedef std::shared_ptr<FixturePreset> FixturePresetInstance;

