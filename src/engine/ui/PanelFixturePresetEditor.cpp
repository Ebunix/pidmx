//
// Created by ebu on 04.02.23.
//

#include "PanelFixturePresetEditor.h"
#include "ImGuiExt.h"
#include "engine/core/Engine.h"

namespace UI {
    void PanelFixturePresetEditor::Draw() {
            DrawPresetTable();
    }

    void PanelFixturePresetEditor::DrawPresetTable() {
        ImGui::BeginDisabled(selectedPreset == INVALID_HASH);
        ImGui::Button("Edit preset");

        ImGui::EndDisabled();

        // TODO: Figure out table sorting for these 
        if (UI::BeginPresetList()) {

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
            Engine& engine = Engine::Instance();

            for (const auto &preset: engine.fixturePresets) {
                UI::DrawPresetTableEntry(preset);
            }

            ImGui::PopStyleVar();

            ImGui::EndTable();
        }
    }

    void PanelFixturePresetEditor::DrawPresetDetails() {

    }
} // UI