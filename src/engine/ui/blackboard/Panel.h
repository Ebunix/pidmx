#pragma once
#include "engine/ui/Panel.h"
#include "Item.h"

namespace Blackboard {
    const float PanelButtonRounding = 5.0f;

    enum ItemEditType {
        BlackboardItemEditType_None,
        BlackboardItemEditType_Move,
        BlackboardItemEditType_Resize
    };

    class Panel : public UI::Panel {
    public:
        Panel() : UI::Panel("Blackboard", UI::PanelType_Blackboard, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollWithMouse) {}

        void Draw() override;

        void Load(const nbt::tag_compound& pack) override {
            UI::Panel::Load(pack);
            exactFit = nbt::Load(pack, "exactFit", nullptr, false);
        }
        nbt::tag_compound Save() override {
            nbt::tag_compound cmp = UI::Panel::Save();
            nbt::Save(cmp, "exactFit", exactFit);
            return cmp;
        }

        void PlaceInstance(const ItemInstance& instance, int x, int y, int width, int height, bool skipAddToShow = false);

        void FreeInstanceArea(Hash id);
        void OccupyInstanceArea(Hash id);
        void OccupyInstanceArea(ItemInstance instance);
        bool IsOccupied(int64_t x, int64_t y);

        void EditItem(const ItemInstance& item, ItemEditType editType);
        inline ItemInstance EditingItem() const { return editingItem; }

    private:
        std::map<int64_t, ItemInstance> placedInstances;

        const float cellSize = 65.0f;
        bool exactFit = true;
        bool currentPlacingSelectSize = false;
        int currentPlacingItemX = 0, currentPlacingItemY = 0;
        ItemType currentPlacingItem = ItemType_None;

        ItemInstance editingItem;
        ItemEditType editingType = BlackboardItemEditType_None;

    };
}
