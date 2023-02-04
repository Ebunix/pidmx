//
// Created by ebu on 04.02.23.
//

#include "ItemFixtureSheet.h"

void Blackboard::ItemFixtureSheet::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
    if (ImGui::BeginTable("table0", 4, ImGuiTableFlags_SizingStretchSame)) {

        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Patch");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("aaaaa");

        ImGui::EndTable();
    }
}
