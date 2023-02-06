#pragma once
#include "Item.h"
#include "engine/ui/Tools.h"

namespace Blackboard {
    class ItemCollection : virtual public Item {
    public:
        Map<int, Hash> collection;

        ItemCollection() : Item("Collections", ItemType_Collections) {}

        nbt::tag_compound Save() override {
            nbt::tag_compound c = Item::Save();
            c.insert("collection", nbt::Serialize(collection.begin(), collection.end()));
            return c;
        }

        void Load(const nbt::tag_compound &c) override {
            Item::Load(c);
            collection = nbt::Deserialize(c, "collection", Map<int, Hash>());
        }

        void Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) override {
            ImGui::PushID(itemIndex);

            if (!HasItemAt(itemIndex)) {
                if (DrawButtonUnassigned(list, topLeft, bottomRight, itemIndex)) {
                    OnClick(itemIndex);
                }
            }
            else if (UI::OutlinedButton(list, 0, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemFull], topLeft, bottomRight)) {
                OnClick(itemIndex);
            }

            ImGui::PopID();
        }

        virtual void OnClick(int itemIdOneBased) {}

        bool HasItemAt(int index) {
            return (index < collection.size() && collection[index] != INVALID_HASH);
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
            return collection.at(i);
        }

    private:
        bool DrawButtonUnassigned(ImDrawList* list, const ImVec2 &tl, const ImVec2 &br, int index) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            bool result = UI::OutlinedButton(list, UI::ColorOutlineHint, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemEmpty], tl, br);
            ImGui::PopStyleColor();
            return result;
        }
    };

}