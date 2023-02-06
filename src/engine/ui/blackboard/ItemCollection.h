#pragma once
#include "Item.h"
#include "engine/ui/Tools.h"

namespace Blackboard {
    class ItemCollection : virtual public Item {
    public:
        ItemCollection() : Item("Collections", ItemType_Collections) {}

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

        void OnResize(int w, int h) override {
            if (w * h > collection.size()) {
                collection.resize(w * h);
            }
        }

        virtual void OnClick(int index) {}

        bool HasItemAt(int index) {
            return (index < collection.size() && collection[index] != INVALID_HASH);
        }

        void AssignAt(int i, Hash value) {
            if (i == 0) {
                LogMessage(LogMessageType_Error, "AssignAt needs to be called with 1-based index!");
                return;
            }
            if (collection.size() <= i) {
                collection.resize(i);
            }
            if (!collection[i - 1] && value != INVALID_HASH) {
                assignedCount++;
            }
            else if (collection[i - 1] && value == INVALID_HASH) {
                assignedCount--;
            }
            collection[i - 1] = value;
        }

        nbt::tag_compound SaveSpecifics() override {
            nbt::tag_compound comp;
            nbt::Save(comp, "collection", collection);
            return comp;
        }

        void LoadSpecifics(const nbt::tag_compound &comp) override {
            collection = nbt::Load(comp, "collection", std::vector<Hash>());
        }

    protected:
        std::vector<Hash> collection;
        int assignedCount = 0;

    private:
        bool DrawButtonUnassigned(ImDrawList* list, const ImVec2 &tl, const ImVec2 &br, int index) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            bool result = UI::OutlinedButton(list, UI::ColorOutlineHint, UI::ColorPresets[UI::ColorPresetType_ButtonBlackboardItemEmpty], tl, br);
            ImGui::PopStyleColor();
            return result;
        }
    };
}