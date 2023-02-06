#pragma once
#include <nbt_tags.h>

struct ISer {
    virtual nbt::tag_compound Save() = 0;
    virtual void Load(const nbt::tag_compound &data) = 0;
};
