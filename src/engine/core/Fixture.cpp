#include "Fixture.h"

nbt::tag_compound Fixture::Save() {
    nbt::tag_compound c = IIdentifiable::Save();
    c.insert("data", data.Save());
    return c;
}

void Fixture::Load(const nbt::tag_compound &c) {
    IIdentifiable::Load(c);
    data.Load(c.at("data").as<nbt::tag_compound>());
}


nbt::tag_compound FixtureData::Save() {
    nbt::tag_compound data;
    data.insert("name", nbt::Serialize(name));
    data.insert("fixtureId", nbt::Serialize(fixtureId));
    data.insert("presetId", nbt::Serialize(presetId));
    data.insert("channel", nbt::Serialize(channel));
    data.insert("universe", nbt::Serialize(universe));
    return data;
}

void FixtureData::Load(const nbt::tag_compound &c) {
    name = nbt::Deserialize(c, "name", std::string(""));
    fixtureId = nbt::Deserialize(c, "fixtureId", INVALID_HASH);
    presetId = nbt::Deserialize(c, "presetId", INVALID_HASH);
    channel = nbt::Deserialize(c, "channel", 0);
    universe = nbt::Deserialize(c, "universe", 0);
}
