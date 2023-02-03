#pragma once
#include <pidmx_utils.h>
#include <memory>

struct FixturePreset {
	Hash id;
	std::string name;
	std::string manufacturer;
	int footprint;
};

typedef std::shared_ptr<FixturePreset> FixturePresetInstance;
