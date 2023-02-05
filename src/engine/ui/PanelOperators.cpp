//
// Created by ebu on 04.02.23.
//

#include "PanelOperators.h"
#include "engine/core/Engine.h"
#include "ImGuiExt.h"

namespace UI {
    void PanelOperators::Draw() {
        Engine& engine = Engine::Instance();
        static ImVec2 buttonSize = ImVec2(70, 70) * engine.dpiScale;

        if (ActionButton("Store", EngineAction_Store)) {
            engine.SetAction(EngineAction_Store);
        }
        if (ActionButton("Delete", EngineAction_Delete)) {
            engine.SetAction(EngineAction_Delete);
        }
        ImGui::NewLine();
        if (ImGui::Button("Clear", buttonSize)) {
            engine.Clear();
        }
    }

    bool PanelOperators::ActionButton(const char *text, EngineAction action) {
        Engine& engine = Engine::Instance();
        static ImVec2 buttonSize = ImVec2(70, 70) * engine.dpiScale;

        if (engine.action == action) {
            ImGui::PushButtonColorPreset(ColorPresets[ColorPresetType_ButtonOperatorActive]);
        }
        bool result = ImGui::Button(text, buttonSize);
        ImGui::SameLine();

        if (engine.action == action) {
            ImGui::PopButtonColorPreset();
        }

        return result;
    }
}