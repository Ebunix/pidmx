#pragma once 
#include "ISerializable.h"

class FixtureCollection : public ISerializable {
public:
	std::vector<Hash> assignedFixtures;

	static std::shared_ptr<FixtureCollection> New(Hash id, const std::string& name);

	void Load(const nbt::tag_compound& pack) override;
	nbt::tag_compound Save() override;
};
typedef std::shared_ptr<FixtureCollection> FixtureCollectionInstance;
