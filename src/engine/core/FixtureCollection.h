#pragma once

#include "NbtFileIO.h"
#include "IIdentifiable.h"

struct FixtureCollection : IIdentifiable {
    std::string name;
    IDSet assignedFixtures;

    FixtureCollection() = default;

    FixtureCollection(Hash id, const std::string &name): name(name) {
        this->id = id;
    }

    nbt::tag_compound Save() const override;

    void Load(const nbt::tag_compound &data) override;
};

typedef std::shared_ptr<FixtureCollection> FixtureCollectionInstance;
