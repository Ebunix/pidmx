#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include "Tools.h"
//
// Created by ebu on 30.01.23.
//

namespace ImGui {
    extern ImFont* fontMonospace;
    extern ImFont* fontPixel;
    extern ImFont* fontPixelGigantic;
    extern ImFont* fontRegular;
    extern ImFont* fontRegularSmall;

    void PushButtonColorPreset(UI::ColorPreset preset);
    void PopButtonColorPreset();
};
