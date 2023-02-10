#pragma once

#include "pidmx_utils.h"
#include "ISer.h"
#include "NbtSerializer.h"

class ShowData;

struct IIdentifiable : ISer {
    Hash id;

    IIdentifiable();

    nbt::tag_compound Save() const override {
        nbt::tag_compound c;
        c.insert("id", nbt::Serialize(id));
        return c;
    }
    void Load(const nbt::tag_compound &c) override;

    bool operator==(const IIdentifiable& other) const {
        return other.id == id;
    }
};
