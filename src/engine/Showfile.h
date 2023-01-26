#pragma once 
#include "EngineCereal.h"
#include "Fixture.h"

class Showfile {
public:
	std::vector<std::shared_ptr<FixtureNew>> fixtures;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(cereal::make_nvp("fixtures", fixtures)); // serialize things by passing them to the archive
	}
};
