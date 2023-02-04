#pragma once
#include "Item.h"
#include "engine/ui/Tools.h"

namespace Blackboard {
    template <typename T = ISerializable>
    class ItemCollection : public Item {
    public:
        explicit ItemCollection(const std::string& name = "Collections") : Item(name, ItemType_Collections) {}

        void Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) override {
            ImVec2 size = bottomRight - topLeft;
            float halfHeight = size.y / 2.0f;

            if (itemIndex >= collection.size() || !collection[itemIndex] || collection[itemIndex]->id == INVALID_HASH) {
                DrawButtonUnassigned(list, topLeft, bottomRight, itemIndex);
            }
            else {
                UI::OutlinedButton(list, 0, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemFull], topLeft, bottomRight);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + halfHeight);
                UI::CenterTextWrap(collection[itemIndex]->name.c_str(), size.x, halfHeight);
            }
        }

        void OnResize(int w, int h) override {
            if (w * h > collection.size()) {
                collection.resize(w * h);
            }
        }

    private:
        std::vector<std::shared_ptr<T>> collection;

        void DrawButtonUnassigned(ImDrawList* list, const ImVec2 &tl, const ImVec2 &br, int index) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            ImGui::PushID(index);
            if (UI::OutlinedButton(list, UI::ColorOutlineHint, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemEmpty], tl, br)) {
                // Todo: Handle store collections
            }
            ImGui::PopID();
            ImGui::PopStyleColor();
        }
    };
}