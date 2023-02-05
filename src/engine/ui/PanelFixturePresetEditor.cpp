//
// Created by ebu on 04.02.23.
//

#include "PanelFixturePresetEditor.h"
#include "ImGuiExt.h"
#include "engine/core/Show.h"

namespace UI {
    void PanelFixturePresetEditor::Draw() {
            DrawPresetTable();
    }

    void PanelFixturePresetEditor::DrawPresetTable() {
        ImGui::BeginDisabled(selectedPreset == INVALID_HASH);
        ImGui::Button("Edit preset");

        ImGui::EndDisabled();

        // TODO: Figure out table sorting for these 
        if (ImGui::BeginTable("table1", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable)) {
            ImGui::TableSetupColumn("Fixture");
            ImGui::TableSetupColumn("Manufacturer");
            ImGui::TableSetupColumn("Mode");
            ImGui::TableSetupColumn("Footprint");
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
            Engine& engine = Engine::Instance();

            static char buffer[20];
            for (const auto &preset: engine.fixturePresets) {
                ImGui::PushID((int)preset.id);
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                bool selected = selectedPreset == preset.id;
                if (ImGui::Selectable(preset.name.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap)) {
                    selectedPreset = preset.id;
                }
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", preset.manufacturer.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("Mode 1");

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%i", preset.footprint  );
                ImGui::PopID();
            }

            ImGui::PopStyleVar();

            ImGui::EndTable();
        }
    }

    void PanelFixturePresetEditor::DrawPresetDetails() {

    }
} // UI