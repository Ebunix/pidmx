#pragma once
#include "Panel.h"
#include "BlackboardItem.h"

namespace UI {
    const float BlackboardPanelButtonRounding = 5.0f;

    enum BlackboardItemEditType {
        BlackboardItemEditType_None,
        BlackboardItemEditType_Move,
        BlackboardItemEditType_Resize
    };

    class BlackboardPanel : public Panel {
    public:
        BlackboardPanel() : Panel("Blackboard", PanelType_Blackboard) {}

        void Draw() override;

        void load(const nbt::tag_compound& pack) override {
            ISerializable::load(pack);
        }
        nbt::tag_compound save() override {
            nbt::tag_compound cmp = ISerializable::save();
            return cmp;
        }

        void PlaceInstance(const BlackboardItemInstance& instance, int x, int y, int width, int height, bool skipAddToShow = false);

        void FreeInstanceArea(Hash id);
        void OccupyInstanceArea(Hash id);
        void OccupyInstanceArea(BlackboardItemInstance instance);
        bool IsOccupied(int64_t x, int64_t y);

        void EditItem(const BlackboardItemInstance& item, BlackboardItemEditType editType);
        inline BlackboardItemInstance EditingItem() const { return editingItem; }

    private:
        std::map<int64_t, BlackboardItemInstance> placedInstances;

        bool currentPlacingSelectSize = false;
        int currentPlacingItemX = 0, currentPlacingItemY = 0;
        BlackboardItemType currentPlacingItem = BlackboardItemType_None;

        BlackboardItemInstance editingItem;
        BlackboardItemEditType editingType = BlackboardItemEditType_None;

    };
}

