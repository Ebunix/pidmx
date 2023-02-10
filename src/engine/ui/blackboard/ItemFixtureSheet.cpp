//
// Created by ebu on 04.02.23.
//

#include "ItemFixtureSheet.h"
#include "engine/core/Engine.h"
#include "engine/core/Fixture.h"
#include "engine/core/ShowData.h"
#include "engine/ui/ImGuiExt.h"

void Blackboard::ItemFixtureSheet::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
    Engine &engine = Engine::Instance();
    ShowData &show = engine.Show();

    if (ImGui::BeginTable("Fixture Sheet", engine.availableParametersOnFixtures.size() + 2, ImGui::tableFlagsPidmx | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("FixID");
        ImGui::TableSetupColumn("Name");
        for (const auto &param: engine.availableParametersOnFixtures) {
            ImGui::TableSetupColumn(FixtureParameterTypeNames[param]);
        }
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        static char idTextBuffer[10];

        for (const auto &fixtureId: engine.activeFixtures) {

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            if (!show.fixtures.contains(fixtureId)) {
                ImGui::Text("Fixture Error!");
                continue;
            }
            FixtureInstance fixture = show.fixtures.at(fixtureId);

            snprintf(idTextBuffer, sizeof(idTextBuffer), "%lu", fixture->data.fixtureId);
            ImGui::PushFont(ImGui::fontMonospace);
            ImGui::Selectable(idTextBuffer, false, ImGuiSelectableFlags_SpanAllColumns);
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Text("%s", fixture->data.name.c_str());

            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, UI::ColorOutputValue);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            for (FixtureParameterType paramType = FixtureParameterType_Unassigned;
                 paramType < FixtureParameterType_Count_; paramType = (FixtureParameterType) (((int) paramType) + 1)) {
                bool anyFixtureHasParam = false;

                for (const auto &fixId: engine.activeFixtures) {
                    if (show.fixtures.at(fixId)->HasParam(paramType)) {
                        anyFixtureHasParam = true;
                        break;
                    }
                }
                if (!anyFixtureHasParam) {
                    continue;
                }


                bool trackedInProgrammer = engine.programmer.IsTracking(fixtureId, paramType);
                bool isDiverged = show.fixtures.at(fixtureId)->IsOutputDiverged(paramType);

                if (trackedInProgrammer) {
                    ImGui::PushStyleColor(ImGuiCol_Text, UI::ColorRecordingValue);
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, UI::ColorRecordingValue);
                }
                else if (isDiverged) {
                    ImGui::PushStyleColor(ImGuiCol_Text, UI::ColorDivergedValue);
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, UI::ColorDivergedValue);
                }

                ImGui::TableNextColumn();
                float paramValue = 0;

                if (fixture->GetParamValue(paramType, &paramValue)) {
                    ImGui::Text("%.02f", paramValue);
                    ImGui::ProgressBar(paramValue, ImVec2(-1, 6 * engine.dpiScale), "");
                }

                if (trackedInProgrammer || isDiverged) {
                    ImGui::PopStyleColor(2);
                }
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }

        ImGui::EndTable();
    }
}
