#include "FixtureCollection.h"

std::shared_ptr<FixtureCollection> FixtureCollection::New(Hash id, const std::string& name)
{
	FixtureCollectionInstance inst = std::make_shared<FixtureCollection>();
	inst->id = id;
	inst->name = name;
	return inst;
}

void FixtureCollection::Load(const nbt::tag_compound &pack)
{
    ISerializable::Load(pack);
	nbt::tag_long_array fix = pack.at("fixtures").as<nbt::tag_long_array>();
	for (auto val : fix) {
		assignedFixtures.push_back(val);
	}
}

nbt::tag_compound FixtureCollection::Save()
{
    nbt::tag_compound comp = ISerializable::Save();
	nbt::tag_long_array fixtureIds;
	for (auto id : assignedFixtures) {
		fixtureIds.push_back(id);
	}
	comp.insert("fixtures", std::move(fixtureIds));
	return comp;
}