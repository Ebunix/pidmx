#pragma once
#include <pidmx_utils.h>
#include <memory>
#include "js/v8ObjectInterface.h"
#include "NbtFileIO.h"
#include "Collections.h"

enum FixtureParameterType {
	FixtureParameterType_Unassigned,
	FixtureParameterType_Dim,
	FixtureParameterType_ColorR,
	FixtureParameterType_ColorG,
	FixtureParameterType_ColorB,
};

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


