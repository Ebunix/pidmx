#pragma once

#include "imgui.h"
#include <string>
#include "engine/core/NbtFileIO.h"
#include "engine/core/IIdentifiable.h"

namespace Blackboard {
    class Panel;

    const ImVec2 ItemOuterPadding(1.5f, 1.5f);
    const ImVec2 ItemInnerPadding(4.0f, 4.0f);

    enum ItemType {
        ItemType_None,
        ItemType_Collections,
        ItemType_Groups,
        ItemType_FixtureSheet,
        ItemType_Count_,
    };

    class Item : public IIdentifiable {
    public:
        std::string name;
        int x = 0, y = 0;
        int width = 1, height = 1;
        ItemType type = ItemType_None;
        std::shared_ptr<Panel> parent;

        Item(std::string name = "", ItemType type = ItemType_None);

        nbt::tag_compound Save() override;
        void Load(const nbt::tag_compound &c) override;

        void Render(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight);

        void Move(int x, int y);
        void Resize(int w, int h);

        virtual void Draw(ImDrawList *list, ImVec2 screenTL, ImVec2 screenBR, int itemIndex) {}
        virtual void OnResize(int newW, int newH) {}
        virtual void OnMove(int newX, int newY) {}
        virtual void AfterLoad();

    private:
        void RenderGrid(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight);
        void RenderWindow(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight);

        inline bool UseGridLayout() {
            return type == ItemType_Collections || type == ItemType_Groups;
        }
    };


    typedef std::shared_ptr<Item> ItemInstance;
    ItemInstance CreateItem(ItemType type);
}

namespace nbt {
    template<> std::shared_ptr<Blackboard::Item> Deserialize(const value &input, const std::shared_ptr<Blackboard::Item> &fallback) {
        bool valid = input.get_type() == tag_type::Compound;
        if (!valid) {
            return fallback;
        }
        const tag_compound &comp = input.as<tag_compound>();
        if (!comp.has_key("id") || !comp.has_key("type")) {
            return fallback;
        }
        auto ptr = Blackboard::CreateItem((Blackboard::ItemType)input.at("type").as<tag_int>().get());
        ptr->Load(comp);
        return ptr;
    }
}