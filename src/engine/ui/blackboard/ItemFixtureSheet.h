#pragma once

#include "Item.h"

namespace Blackboard {

    class ItemFixtureSheet: public Item {
    public:
        ItemFixtureSheet(): Item("Fixture sheet", ItemType_FixtureSheet) {}

        void Draw(ImDrawList* list, ImVec2 tl, ImVec2 br, int itemIndex) override;
    };

}