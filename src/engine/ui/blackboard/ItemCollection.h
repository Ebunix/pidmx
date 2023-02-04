#pragma once
#include "Item.h"
#include "engine/ui/Tools.h"

namespace Blackboard {
    template <typename T>
    class ItemCollection : public Item {
    public:
        explicit ItemCollection(const std::string& name = "Collections", ItemType type = ItemType_Collections) : Item(name, type) {}

        void Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) override {
            ImVec2 size = bottomRight - topLeft;
            ImGui::PushID(itemIndex);

            if (!HasItemAt(itemIndex)) {
                DrawButtonUnassigned(list, topLeft, bottomRight, itemIndex);
            }
            else {
                UI::OutlinedButton(list, 0, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemFull], topLeft, bottomRight);
            }

            ImGui::PopID();
        }

        void OnResize(int w, int h) override {
            if (w * h > collection.size()) {
                collection.resize(w * h);
            }
        }

        bool HasItemAt(int index) {
            return (index < collection.size() && collection[index] != nullptr);
        }

        void AssignAt(int i, std::shared_ptr<T> value) {
            if (i == 0) {
                LogMessage(LogMessageType_Error, "AssignAt needs to be called with 1-based index!");
                return;
            }
            if (collection.size() <= i) {
                collection.resize(i);
            }
            if (!collection[i - 1] && value) {
                assignedCount++;
            }
            else if (collection[i - 1] && !value) {
                assignedCount--;
            }
            collection[i - 1] = value;
        }

    protected:
        std::vector<std::shared_ptr<T>> collection;
        int assignedCount = 0;

    private:
        void DrawButtonUnassigned(ImDrawList* list, const ImVec2 &tl, const ImVec2 &br, int index) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            if (UI::OutlinedButton(list, UI::ColorOutlineHint, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemEmpty], tl, br)) {
                // Todo: Handle store collections
            }
            ImGui::PopStyleColor();
        }
    };
}