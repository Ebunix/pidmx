#pragma once 
#include "ISerializable.h"

class FixtureCollection : public ISerializable {
public:
	std::vector<Hash> assignedFixtures;

	static std::shared_ptr<FixtureCollection> New(Hash id, const std::string& name);

	void load(const nbt::tag_compound& pack) override;
	nbt::tag_compound save() override;
};
typedef std::shared_ptr<FixtureCollection> FixtureCollectionInstance;
