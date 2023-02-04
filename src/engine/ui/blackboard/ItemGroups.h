#pragma once
#include "ItemCollection.h"
#include "engine/core/Fixture.h"

namespace Blackboard {
    struct ItemGroupsStorage {
        std::string name;
        std::vector<FixtureInstance> fixtures;
    };

    class ItemGroups : public ItemCollection<ItemGroupsStorage>  {
    public:
        ItemGroups();

        void Draw(ImDrawList* list, ImVec2 tl, ImVec2 br, int itemIndex) override;
    };
}