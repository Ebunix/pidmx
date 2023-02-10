#pragma once
#include "Item.h"
#include "engine/ui/Tools.h"

namespace Blackboard {
    class ItemCollection : virtual public Item {
    public:
        Map<int, Hash> collection;

        ItemCollection() : Item("Collections", ItemType_Collections) {}

        nbt::tag_compound Save() const override {
            nbt::tag_compound c = Item::Save();
            c.insert("collection", nbt::Serialize(collection.begin(), collection.end()));
            return c;
        }

        void Load(const nbt::tag_compound &c) override {
            Item::Load(c);
            collection = nbt::Deserialize(c, "collection", Map<int, Hash>());
        }

        void Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) override;
        virtual void DrawEdit(bool justOpened) {}

        virtual void OnClick(int itemIdOneBased) {}

        bool HasItemAt(int index) {
            return GetAt(index) != INVALID_HASH;
        }

        void AssignAt(int i, Hash value) {
            if (value == INVALID_HASH && collection.contains(i)) {
                collection.erase(i);
            }
            else if (value != INVALID_HASH) {
                collection.insert_or_assign(i, value);
            }
        }
        Hash GetAt(int i) {
            return collection.contains(i) ? collection.at(i) : INVALID_HASH;
        }

        void BeginEditing(int index) {
            editDialog = true;
            editTargetIndex = index;
        }
        void FinishEditing() {
            editDialog = false;
            editTargetIndex = 0;
        }

    protected:
        int editTargetIndex = 0;

    private:
        bool editDialog = false;

        bool DrawButtonUnassigned(ImDrawList* list, const ImVec2 &tl, const ImVec2 &br, int index) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            bool result = UI::OutlinedButton(list, UI::ColorOutlineHint, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemEmpty], tl, br);
            ImGui::PopStyleColor();
            return result;
        }
    };

}