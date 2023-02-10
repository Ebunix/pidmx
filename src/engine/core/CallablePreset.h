#pragma once

#include "IIdentifiable.h"
#include "FixturePreset.h"

struct CallablePreset: IIdentifiable {
    Hash fixtureId;
    Map<FixtureParameterType, float> values;

    nbt::tag_compound Save() const override {
        auto c = IIdentifiable::Save();
        c.insert("fixtureId", nbt::Serialize(fixtureId));
        c.insert("values", nbt::Serialize(values.begin(), values.end()));
        return c;
    }

    void Load(const nbt::tag_compound &data) override {
        IIdentifiable::Load(data);
        fixtureId = nbt::Deserialize(data, "fixtureId", INVALID_HASH);
        values = nbt::Deserialize(data, "values", Map<FixtureParameterType, float>());
    }
};

ANKERL_HASH(CallablePreset)