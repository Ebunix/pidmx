#pragma once
#include <imgui.h>
#include <set>
#include "imgui_internal.h"
#include "engine/ui/blackboard/Panel.h"

extern ImVec2 buttonSmall;
extern ImVec2 buttonLarge;

enum DialogOption {
	DialogOption_None = 0,
	DialogOption_Cancel = 1,
	DialogOption_OK = 2,
};

namespace UI {
	struct ColorPreset {
		ImVec4 regular;
		ImVec4 hovered;
		ImVec4 active;
	};
	enum ColorPresetType {
		ColorPresetType_Button,
		ColorPresetType_ButtonRed,
		ColorPresetType_PanelItemMain,
		ColorPresetType_ButtonBlackboardItemEmpty,
		ColorPresetType_ButtonBlackboardItemFull,
		ColorPresetType_Count_
	};

	extern ColorPreset ColorPresets[ColorPresetType_Count_];
    const ImVec4 ColorOutlineHint(1.0f, 1.0f, 1.0f, 0.08f);
    const ImVec4 ColorTextTransparentLight(1.0f, 1.0f, 1.0f, 0.17f);

	bool BeginMenuBar();
	bool BeginMainMenuBar();
	inline void EndMenuBar() {
		ImGui::PopStyleVar(2);
		ImGui::EndMenuBar();
	}
	inline void EndMainMenuBar() {
		ImGui::PopStyleVar(3);
		ImGui::EndMainMenuBar();
	}
	bool BeginPopupDialog(bool openNow, const char* title);
	DialogOption EndPopupDialog(int options, int result = DialogOption_None, int disableMask = DialogOption_None);

    DialogOption ShowMessagePopup(bool openNow, int options, const char* message, const char* title = "Message");

    ImVec2 CenterTextWrap(const char* str, float maxWidth, float maxHeight = 0);
    bool OutlinedButton(ImDrawList *dl, const ImColor &borderColor, const ColorPreset &frameBgColor, const ImVec2 &tl, const ImVec2 &br, float thickness = 2.0f);
    void OutlinedPanel(ImDrawList *dl, const ImColor &borderColor, const ImColor &bgColor, const ImVec2 &tl, const ImVec2 &br, float thickness = 2.0f);
    void OutlinedPanelBorder(ImDrawList *dl, const ImColor &borderColor, const ImVec2 &tl, const ImVec2 &br, float thickness = 2.0f);
}
