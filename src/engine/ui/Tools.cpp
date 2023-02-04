#include "engine/command/CommandBlackboard.h"
#include "ImGuiExt.h"
#include "engine/core/Show.h"
#include "BlackboardPanel.h"
#include "Tools.h"
#include <string>
#include <cmath>
#include "engine/core/Engine.h"

UI::ColorPreset UI::ColorPresets[ColorPresetType_Count_] = {
    { ImVec4(1.00f, 1.00f, 1.00f, 0.10f), ImVec4(1.00f, 1.00f, 1.00f, 0.27f), ImVec4(0.00f, 0.00f, 0.00f, 0.27f) }, // Button
    { ImVec4(1.00f, 0.25f, 0.25f, 0.10f), ImVec4(1.00f, 0.25f, 0.25f, 0.27f), ImVec4(0.75f, 0.00f, 0.00f, 0.27f) }, // ButtonRed
    { ImVec4(0.18f, 0.40f, 0.79f, 1.00f), ImVec4(0.22f, 0.56f, 0.94f, 1.00f), ImVec4(0.13f, 0.22f, 0.55f, 1.00f) }, // PanelItemMain

    { ImVec4(1.00f, 1.00f, 1.00f, 0.04f), ImVec4(1.00f, 1.00f, 1.00f, 0.27f), ImVec4(0.00f, 0.00f, 0.00f, 0.27f) }, // ButtonBlackboardItemEmpty
    { ImVec4(1.00f, 1.00f, 1.00f, 0.12f), ImVec4(1.00f, 1.00f, 1.00f, 0.27f), ImVec4(0.00f, 0.00f, 0.00f, 0.27f) }, // ButtonBlackboardItemFull
};

bool UI::BeginMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8 * globalEngine->dpiScale, 8 * globalEngine->dpiScale));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16 * globalEngine->dpiScale, 16 * globalEngine->dpiScale));
    return ImGui::BeginMenuBar();
}

bool UI::BeginMainMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8 * globalEngine->dpiScale, 8 * globalEngine->dpiScale));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16 * globalEngine->dpiScale, 16 * globalEngine->dpiScale));
    return ImGui::BeginMainMenuBar();
}

bool UI::BeginPopupDialog(bool openNow, const char *title)
{
	if (openNow) {
		ImGui::OpenPopup(title);
	}
	ImGui::SetNextWindowSizeConstraints(ImVec2(300 * globalEngine->dpiScale, 100 * globalEngine->dpiScale), ImVec2(50000, 50000));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8 * globalEngine->dpiScale, 8 * globalEngine->dpiScale));
	bool result = ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	if (!result) {
		ImGui::PopStyleVar();
	}
	else if (openNow) {
		ImGui::SetKeyboardFocusHere();
	}
	return result;
}

static inline int DrawButton(const char* title, int options, int compare, int disableMask, int result) {
    if (options & compare) {
        ImGui::BeginDisabled(compare & disableMask);
        if (ImGui::Button(title, ImVec2(80 * globalEngine->dpiScale, 0)))
            result = compare;
        ImGui::SameLine();
        ImGui::EndDisabled();
    }
    return result;
}

DialogOption UI::EndPopupDialog(int options, int result, int disableMask)
{
	if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
		ImGui::CloseCurrentPopup();
	}

	ImGui::Separator();
    result = DrawButton("Cancel", options, DialogOption_Cancel, disableMask, result);
    result = DrawButton("OK", options, DialogOption_OK, disableMask, result);

	if (result != DialogOption_None) {
		ImGui::CloseCurrentPopup();
	}
	
	ImGui::PopStyleVar();
	ImGui::EndPopup();
	return (DialogOption)result;
}

DialogOption UI::ShowMessagePopup(bool openNow, int options, const char *message, const char* title) {
    if (BeginPopupDialog(openNow, title)) {
        ImGui::Spacing();
        ImGui::Text("%s", message);
        ImGui::Spacing();
        return EndPopupDialog(options);
    }
    return DialogOption_None;
}

std::string built;
ImVec2 UI::CenterTextWrap(const char *str, float maxWidth, float maxHeight) {
    ImVec2 cursorPos = ImGui::GetCursorPos();
    built.clear();
    float available = maxWidth;
    char* source = (char*)str;
    ImVec2 extent;
    int segments = 1;

    while (source) {
        char* chr = strchr(source, ' ');
        ImVec2 size;
        if (chr) {
            size = ImGui::CalcTextSize(source, chr + 1);
        }
        else {
            size = ImGui::CalcTextSize(source);
        }

        if (available - size.x > 0) {
            if (!built.empty())
            {
                built.append(" ");
            }
            built.append(source, chr ? chr - source : strlen(source));
            ImVec2 bounds = ImGui::CalcTextSize(built.c_str());
            available = maxWidth - bounds.x;
            extent.x = fmax(extent.x, bounds.x);
            extent.y = fmax(extent.y, bounds.y);
            if (chr) {
                chr += 1;
            }
            source = chr;
        }
        else {
            if (built.empty()) {
                if (chr) {
                    built.append(source, chr + 1);
                }
                else {
                    built.append(source);
                }
                source = chr;
            }
            ImVec2 actualSize = ImGui::CalcTextSize(built.c_str());
            extent.y += actualSize.y;
            extent.x = fmax(extent.x, actualSize.x);
            available = maxWidth;
            segments++;
            built.append("\0", 1);
        }
    }

    char* finalStr = (char*)built.c_str();
    float yStartOffset = (maxHeight - extent.y) / 2;
    if (yStartOffset > 0) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yStartOffset);
    }

    for (int i = 0; i < segments; i++) {
        ImVec2 actualSize = ImGui::CalcTextSize(finalStr);
        ImVec2 drawPos((maxWidth - actualSize.x) / 2, cursorPos.y);
        ImGui::SetCursorPosX(cursorPos.x + drawPos.x);
        ImGui::Text("%s", built.c_str());
        ImGui::SetCursorPosX(cursorPos.x);
        cursorPos.y += actualSize.y;
        ImGui::SetCursorPosY(cursorPos.y);
    }

    return extent;
}

bool UI::OutlinedButton(ImDrawList *dl, const ImColor &borderColor, const ColorPreset &bgColor, const ImVec2 &tl, const ImVec2 &br, float thickness) {
    ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
    ImRect rect(tl, br);
    ImGui::ItemAdd(rect, id);

    bool hovering;
    bool holding;
    bool clicked = ImGui::ButtonBehavior(rect, id, &hovering, &holding);

    ImColor bg = holding ? bgColor.active : (hovering ? bgColor.hovered : bgColor.regular);

    OutlinedPanel(dl, borderColor, bg, tl, br, thickness);

    return clicked;
}

void UI::OutlinedPanel(ImDrawList *dl, const ImColor &borderColor, const ImColor &bgColor, const ImVec2 &tl, const ImVec2 &br, float thickness) {
    dl->AddRectFilled(tl, br, bgColor, BlackboardPanelButtonRounding * globalEngine->dpiScale);
    OutlinedPanelBorder(dl, borderColor, tl, br, thickness);
}
void UI::OutlinedPanelBorder(ImDrawList *dl, const ImColor &borderColor, const ImVec2 &tl, const ImVec2 &br, float thickness) {
    dl->AddRect(tl, br, borderColor, BlackboardPanelButtonRounding * globalEngine->dpiScale, 0, thickness * globalEngine->dpiScale);
}
