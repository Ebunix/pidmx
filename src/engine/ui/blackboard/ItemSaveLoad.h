#pragma once

#include "Item.h"
#include "ItemGroups.h"
#include "ItemCollection.h"
#include "ItemFixtureSheet.h"

NBT_SAVE_LOAD_ENUM(Blackboard::ItemType)

static inline void LoadBlackboardItemCommon(
        const nbt::tag_compound &comp, const std::string &tag, void *input, const std::shared_ptr<Blackboard::Item> &fallback
) {
    auto *value = (Blackboard::Item *) input;
    NBT_LOAD_MEMBER_PTR(id);
    NBT_LOAD_MEMBER_PTR(name);
    NBT_LOAD_MEMBER_PTR(type);
    NBT_LOAD_MEMBER_PTR(x);
    NBT_LOAD_MEMBER_PTR(y);
    NBT_LOAD_MEMBER_PTR(width);
    NBT_LOAD_MEMBER_PTR(height);
}

static inline void SaveBlackboardItemCommon(
        nbt::tag_compound &comp, const std::string &tag, const std::shared_ptr<Blackboard::Item> &value
) {
    NBT_SAVE_MEMBER_PTR(id);
    NBT_SAVE_MEMBER_PTR(name);
    NBT_SAVE_MEMBER_PTR(type);
    NBT_SAVE_MEMBER_PTR(x);
    NBT_SAVE_MEMBER_PTR(y);
    NBT_SAVE_MEMBER_PTR(width);
    NBT_SAVE_MEMBER_PTR(height);
}

namespace nbt {
    inline void Save(nbt::tag_compound &comp, const std::string &tag, const std::shared_ptr<Blackboard::ItemCollection> &value) {
        Save(comp, "collection", value->collection);
    }
};
namespace nbt {
    inline std::shared_ptr<Blackboard::ItemCollection> Load(const nbt::tag_compound &comp, const std::string &tag, void *input,
                                                            const std::shared_ptr<Blackboard::ItemCollection> &fallback) {
        std::shared_ptr<Blackboard::ItemCollection> value = std::make_shared<Blackboard::ItemCollection>();
        value->collection = Load(comp, "collection", nullptr, value->collection);
        return value;
    }
};

namespace nbt {
    inline void Save(nbt::tag_compound &comp, const std::string &tag, const std::shared_ptr<Blackboard::Item> &value) {
        switch (value->type) {
            case Blackboard::ItemType_Collections: {
                const auto val = std::dynamic_pointer_cast<Blackboard::ItemCollection>(value);
                Save(comp, tag, val);
                break;
            }
            case Blackboard::ItemType_None:
            default:
                break;
        }
        SaveBlackboardItemCommon(comp, tag, value);
    }
}

NBT_LOAD(std::shared_ptr<Blackboard::Item>, {
    std::shared_ptr<Blackboard::Item> value;
    switch (Load(comp, "type", nullptr, 0)) {
        case Blackboard::ItemType_Collections:
            value = Load(comp, tag, nullptr, std::shared_ptr<Blackboard::ItemCollection>());
            break;
        case Blackboard::ItemType_None:
        default:
            value = std::make_shared<Blackboard::Item>();
            break;
    }
    LoadBlackboardItemCommon(comp, tag, value.get(), value);
    return value;
});