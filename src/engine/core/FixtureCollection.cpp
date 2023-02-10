#include "FixtureCollection.h"

nbt::tag_compound FixtureCollection::Save() const {
    auto c = IIdentifiable::Save();
    c.insert("assignedFixtures", nbt::Serialize(assignedFixtures.begin(), assignedFixtures.end()));
    return c;
}

void FixtureCollection::Load(const nbt::tag_compound &data) {
    assignedFixtures = nbt::Deserialize(data, "assignedFixtures", IDSet());
}
