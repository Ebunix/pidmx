#pragma once

#include "ItemCollection.h"
#include "engine/core/Fixture.h"

namespace Blackboard {
    struct GroupData: IIdentifiable {
        std::string name;
        IDSet fixtures;
        nbt::tag_compound Save() const override;
        void Load(const nbt::tag_compound &c) override;
    };

    class ItemGroups : public ItemCollection {
    public:

        ItemGroups();

        void Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) override;
        void DrawEdit(bool justOpened) override;

        void OnClick(int itemId) override;
    };
}
