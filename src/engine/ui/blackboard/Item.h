#pragma once

#include "imgui.h"
#include <string>
#include "engine/core/ISerializable.h"

namespace Blackboard {
    class Panel;

    const ImVec2 ItemOuterPadding(1.5f, 1.5f);
    const ImVec2 ItemInnerPadding(4.0f, 4.0f);

    enum ItemType {
        ItemType_None,
        ItemType_Collections,
        ItemType_Groups,
        ItemType_Count_,
    };

    class Item : public ISerializable {
    public:
        int x = 0, y = 0;
        int width = 1, height = 1;
        ItemType type = ItemType_None;
        std::shared_ptr<Panel> parent;

        Item(std::string name, ItemType type);

        void load(const nbt::tag_compound& pack) override;
        nbt::tag_compound save() override;
        void afterLoad() override;

        void Render(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight);

        void Move(int x, int y);
        void Resize(int w, int h);

        virtual void Draw(ImDrawList *list, ImVec2 screenTL, ImVec2 screenBR, int itemIndex) {}
        virtual void OnResize(int newW, int newH) {}
        virtual void OnMove(int newX, int newY) {}
    };


    typedef std::shared_ptr<Item> ItemInstance;
    ItemInstance CreateItem(ItemType type);
}
