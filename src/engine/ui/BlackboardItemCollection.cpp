//
// Created by ebu on 01.02.23.
//

#include "BlackboardItemCollection.h"
#include "Tools.h"
#include "ImGuiExt.h"
#include <imgui_internal.h>
#include "engine/core/Show.h"
#include "engine/core/Engine.h"

void UI::CollectionsBlackboardItem::Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) {

    ImVec2 size = bottomRight - topLeft;

    if (choosingCollectionIndex == itemIndex) {
        static FixtureCollectionInstance selectedCollection = nullptr;
        if (UI::BeginPopupDialog(startChoosingCollectionIndex, "Choose collection")) {
            startChoosingCollectionIndex = false;
            if (ImGui::BeginCombo("Collection", selectedCollection ? selectedCollection->name.c_str() : "<none>")) {
                if (ImGui::Selectable("<none>")) {
                    selectedCollection = nullptr;
                }
                for (const auto& c : currentShow->fixtureCollections.items) {
                    if (ImGui::Selectable(c->name.c_str())) {
                        selectedCollection = c;
                    }
                }
                ImGui::EndCombo();
            }
            if (UI::EndPopupDialog(DialogOption_OK | DialogOption_Cancel) == DialogOption_OK) {
                if (selectedCollection) {
                    assignedCollections[choosingCollectionIndex] = selectedCollection;
                    choosingCollectionIndex = -1;
                }
            }
        }
    }

    if (itemIndex >= assignedCollections.size() || assignedCollections[itemIndex] == INVALID_HASH) {
        DrawButtonUnassigned(list, topLeft, bottomRight, itemIndex);
        return;
    }

    UI::OutlinedButton(list, 0, ColorPresets[ColorPresetType_ButtonBlackboardItemFull], topLeft, bottomRight);

    float halfHeight = size.y / 2.0f;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + halfHeight);

    auto collection = assignedCollections[itemIndex];
    UI::CenterTextWrap(collection->name.c_str(), size.x, halfHeight);
}

void UI::CollectionsBlackboardItem::DrawButtonUnassigned(ImDrawList* list, const ImVec2 &tl, const ImVec2 &br, int index) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::PushID(index);
    if (UI::OutlinedButton(list, ColorOutlineHint, ColorPresets[ColorPresetType_ButtonBlackboardItemEmpty], tl, br)
    ) {
        startChoosingCollectionIndex = true;
        choosingCollectionIndex = index;
    }
    ImGui::PopID();
    ImGui::PopStyleColor();
}
