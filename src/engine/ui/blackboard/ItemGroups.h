#pragma once

#include "ItemCollection.h"
#include "engine/core/Fixture.h"

namespace Blackboard {
    struct GroupData {
        Hash id;
        std::string name;
        std::set<Hash> fixtures;
    };
    typedef std::shared_ptr<GroupData> GroupDataInstance;

    class ItemGroups : public ItemCollection {
    public:
        const ImU32 ColorGroupIndicatorInactive = 0xff888888;
        const ImU32 ColorGroupIndicatorSolo = 0xff88ff88;
        const ImU32 ColorGroupIndicatorMulti = 0xff88ffff;

        ItemGroups();

        void Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) override;

        void OnClick(int itemIndex) override;

        nbt::tag_compound Save() override;

        void Load(const nbt::tag_compound &comp) override;

    };
}

NBT_SAVE(Blackboard::GroupDataInstance, {
    NBT_SAVE_MEMBER_PTR(id);
    NBT_SAVE_MEMBER_PTR(name);
    NBT_SAVE_MEMBER_PTR(fixtures);
})
namespace nbt {
    inline Blackboard::GroupDataInstance Load(const nbt::tag_compound &comp, const std::string &tag,
                                              const Blackboard::GroupDataInstance &fallback) {
        Blackboard::GroupDataInstance value = std::make_shared<Blackboard::GroupData>();
        value->id = Load(comp, "id", value->id);
        value->name = Load(comp, "name", value->name);
        value->fixtures = Load(comp, "fixtures", value->fixtures);
        return value;
    }
}