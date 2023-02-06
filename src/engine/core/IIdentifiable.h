#pragma once

#include "pidmx_utils.h"
#include "ISer.h"
#include "NbtSerializer.h"

class ShowData;

struct IIdentifiable : ISer {
    Hash id;

    IIdentifiable();

    nbt::tag_compound Save() override {
        nbt::tag_compound c;
        c.insert("id", nbt::Serialize(id));
        return c;
    }
    void Load(const nbt::tag_compound &c) override {
        id = nbt::Deserialize(c, "id", INVALID_HASH);
    }
};
