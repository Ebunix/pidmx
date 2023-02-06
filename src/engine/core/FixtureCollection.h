#pragma once 
#include "ISerializable.h"

struct FixtureCollection {
    Hash id;
    std::string name;
	IDSet assignedFixtures;
};
typedef std::shared_ptr<FixtureCollection> FixtureCollectionInstance;

NBT_SAVE(FixtureCollectionInstance, {
    NBT_SAVE_MEMBER_PTR(id);
    NBT_SAVE_MEMBER_PTR(name);
    NBT_SAVE_MEMBER_PTR(assignedFixtures);
})
NBT_LOAD(FixtureCollectionInstance, {
    FixtureCollectionInstance value = std::make_shared<FixtureCollection>();
    NBT_LOAD_MEMBER_PTR(id);
    NBT_LOAD_MEMBER_PTR(name);
    NBT_LOAD_MEMBER_PTR(assignedFixtures);
    return value;
})