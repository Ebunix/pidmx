//
// Created by ebu on 30.01.23.
//

#include "ImGuiExt.h"
#include "imgui_internal.h"
#include "engine/ui/blackboard/Panel.h"
#include "Tools.h"

ImFont* ImGui::fontMonospace;
ImFont* ImGui::fontPixel;
ImFont* ImGui::fontPixelGigantic;
ImFont* ImGui::fontRegular;
ImFont* ImGui::fontRegularSmall;

void ImGui::PushButtonColorPreset(UI::ColorPreset preset) {
    ImGui::PushStyleColor(ImGuiCol_Button, preset.regular);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, preset.hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, preset.active);
}

void ImGui::PopButtonColorPreset() {
    ImGui::PopStyleColor(3);
}

