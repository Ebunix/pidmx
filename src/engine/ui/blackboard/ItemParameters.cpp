//
// Created by ebu on 08.02.23.
//

#include "ItemParameters.h"
#include "engine/core/Engine.h"

namespace Blackboard {
    void ItemParameters::Draw(ImDrawList *list, ImVec2 screenTL, ImVec2 screenBR, int itemIndex) {
        Engine &engine = Engine::Instance();

        for (const auto &trackParam: engine.availableParametersOnFixtures) {
            float value = storedValues.contains(trackParam) ? storedValues.at(trackParam) : 0.0f;
            if (DrawParameter(trackParam, &value)) {
                for (const auto &fixId : engine.activeFixtures) {
                    engine.programmer.AddTrackingParameter(fixId, trackParam);
                }
                storedValues.insert_or_assign(trackParam, value);
                engine.programmer.SetParamAcrossFixtures(trackParam, value);
            }
        }
    }

    bool ItemParameters::DrawParameter(FixtureParameterType type, float *input) {
        float temp = *input * 100.0f;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6) * Engine::Instance().dpiScale);
        bool result = ImGui::SliderFloat(FixtureParameterTypeNames[type], &temp, 0.0f, 100.0f, "%.01f");
        if (result) {
            *input = temp / 100.0f;
        }
        ImGui::PopStyleVar();
        return result;
    }
} // Blackboard