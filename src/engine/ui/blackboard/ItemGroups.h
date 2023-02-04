#pragma once
#include "ItemCollection.h"
#include "engine/core/Fixture.h"

namespace Blackboard {
    class ItemGroups : public ItemCollection<Fixture>  {
    public:
        ItemGroups() : ItemCollection<Fixture>("Fixture Groups") {}

        void Draw(ImDrawList* list, ImVec2 tl, ImVec2 br, int itemIndex) override;
    };
}