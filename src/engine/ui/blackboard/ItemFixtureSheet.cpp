//
// Created by ebu on 04.02.23.
//

#include "ItemFixtureSheet.h"
#include "engine/core/Engine.h"
#include "engine/core/Fixture.h"
#include "engine/core/ShowData.h"

void Blackboard::ItemFixtureSheet::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
    Engine& engine = Engine::Instance();
    ShowData &show = engine.Show();

    if (ImGui::BeginTable("Fixture Sheet", engine.availableParametersOnFixtures.size() + 1)) {
        ImGui::TableSetupColumn("FixID");
        for (const auto& param : engine.availableParametersOnFixtures) {
            ImGui::TableSetupColumn(FixtureParameterTypeNames[param]);
        }
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        for (const auto& fixtureId : engine.activeFixtures) {
            FixtureInstance fixture = show.fixtures.at(fixtureId);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Selectable(fixture->data.name.c_str(), false);
        }

        ImGui::EndTable();
    }
}
