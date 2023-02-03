#pragma once
#include <imgui.h>
#include <set>
#include "imgui_internal.h"
#include "BlackboardPanel.h"

extern ImVec2 buttonSmall;
extern ImVec2 buttonLarge;

enum DialogOption {
	DialogOption_None = 0,
	DialogOption_Cancel = 1,
	DialogOption_OK = 2,
};

namespace UI {
	inline bool BeginMenuBar() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8 * DPI_SCALE, 8 * DPI_SCALE));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16 * DPI_SCALE, 16 * DPI_SCALE));
		return ImGui::BeginMenuBar();
	}
	inline bool BeginMainMenuBar() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8 * DPI_SCALE, 8 * DPI_SCALE));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16 * DPI_SCALE, 16 * DPI_SCALE));
		return ImGui::BeginMainMenuBar();
	}
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
    void DrawOutlinedPanel(ImDrawList *dl, const ImColor &borderColor, const ImColor &frameBgColor, const ImVec2 &tl, const ImVec2 &br);
    void DrawOutlinedPanelBorder(ImDrawList *dl, const ImColor &borderColor, const ImVec2 &tl, const ImVec2 &br);

    bool BlackboardPanelButton(const char *label, const ImVec2 &tl, const ImVec2 &br, ImGuiButtonFlags flags = ImGuiButtonFlags_None);
}
