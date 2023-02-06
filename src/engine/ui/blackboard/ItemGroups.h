#pragma once

#include "ItemCollection.h"
#include "engine/core/Fixture.h"

namespace Blackboard {
    struct GroupData {
        Hash id;
        std::string name;
        IDSet fixtures;
    };
    typedef std::shared_ptr<GroupData> GroupDataInstance;

    class ItemGroups : public ItemCollection {
    public:
        const ImU32 ColorGroupIndicatorInactive = 0xff888888;
        const ImU32 ColorGroupIndicatorSolo = 0xff88ff88;
        const ImU32 ColorGroupIndicatorMulti = 0xff88ffff;

        ItemGroups();

        void Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) override;

        void OnClick(int itemIdOneBased) override;
    };
}

NBT_SAVE(Blackboard::GroupDataInstance, {
    NBT_SAVE_MEMBER_PTR(id);
    NBT_SAVE_MEMBER_PTR(name);
    NBT_SAVE_MEMBER_PTR(fixtures);
})
NBT_LOAD(Blackboard::GroupDataInstance, {
    Blackboard::GroupDataInstance value = std::make_shared<Blackboard::GroupData>();
    NBT_LOAD_MEMBER_PTR(id);
    NBT_LOAD_MEMBER_PTR(name);
    NBT_LOAD_MEMBER_PTR(fixtures);
    return value;
})