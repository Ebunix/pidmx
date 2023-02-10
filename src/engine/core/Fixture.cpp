#include "Fixture.h"
#include "Engine.h"

nbt::tag_compound Fixture::Save() const {
    nbt::tag_compound c = IIdentifiable::Save();
    c.insert("data", data.Save());
    c.insert("values", nbt::Serialize(calledValues.begin(), calledValues.end()));
    c.insert("calledValues", nbt::Serialize(outputValues.begin(), outputValues.end()));
    return c;
}

void Fixture::Load(const nbt::tag_compound &c) {
    IIdentifiable::Load(c);
    data.Load(c.at("data").as<nbt::tag_compound>());
    calledValues = nbt::Deserialize(c, "values", Map<FixtureParameterType, float>());
    outputValues = nbt::Deserialize(c, "calledValues", Map<FixtureParameterType, float>());

    ShowData &show = Engine::Instance().Show();
    FixturePresetInstance preset = show.fixturePresets.at(data.presetId);
    for (const auto &param : preset->parameters) {
        availableParameters.insert(param->type);
    }
}

Fixture::Fixture(FixtureData data): data(std::move(data)) {
    ShowData &show = Engine::Instance().Show();
    if (show.fixturePresets.contains(data.presetId)) {
        FixturePresetInstance preset = show.fixturePresets.at(data.presetId);
        for (const auto &param : preset->parameters) {
            float defaultValue = (1.0f / 255) * param->defaultValue;
            calledValues.insert_or_assign(param->type, defaultValue);
            outputValues.insert_or_assign(param->type, defaultValue);
            availableParameters.insert(param->type);
        }
    }
}

bool Fixture::IsAnyOutputDiverged() {
    for (const auto& val : availableParameters) {
        if (IsOutputDiverged(val)) {
            return true;
        }
    }
    return false;
}

bool Fixture::IsOutputDiverged(FixtureParameterType param) {
    return outputValues.at(param) != calledValues.at(param);
}


nbt::tag_compound FixtureData::Save() const {
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
