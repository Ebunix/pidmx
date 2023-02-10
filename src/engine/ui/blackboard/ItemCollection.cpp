#include "ItemCollection.h"

namespace Blackboard {
    void ItemCollection::Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) {
        ImGui::PushID(itemIndex);

        if (editTargetIndex && editTargetIndex == itemIndex) {
            if (UI::BeginPopupDialog(editDialog, "Edit data")) {
                DrawEdit(editDialog);
                editDialog = false;
            }
        }
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
}