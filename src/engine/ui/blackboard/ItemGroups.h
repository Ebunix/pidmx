#pragma once

#include "ItemCollection.h"
#include "engine/core/Fixture.h"

namespace Blackboard {
    struct GroupData: IIdentifiable {
        std::string name;
        IDSet fixtures;
        nbt::tag_compound Save() override;
        void Load(const nbt::tag_compound &c) override;
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
