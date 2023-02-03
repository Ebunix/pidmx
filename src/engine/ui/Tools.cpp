#include "engine/command/CommandBlackboard.h"
#include "ImGuiExt.h"
#include "engine/core/Show.h"
#include "BlackboardPanel.h"
#include "Tools.h"
#include <string>
#include <cmath>

bool UI::BeginPopupDialog(bool openNow, const char* title)
{
	if (openNow) {
		ImGui::OpenPopup(title);
	}
	ImGui::SetNextWindowSizeConstraints(ImVec2(300 * DPI_SCALE, 100 * DPI_SCALE), ImVec2(50000, 50000));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8 * DPI_SCALE, 8 * DPI_SCALE));
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
        if (ImGui::Button(title, ImVec2(80 * DPI_SCALE, 0)))
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

void UI::DrawOutlinedPanel(ImDrawList *dl, const ImColor &borderColor, const ImColor &frameBgColor, const ImVec2 &tl, const ImVec2 &br) {
    dl->AddRectFilled(tl, br, frameBgColor, BlackboardPanelButtonRounding * DPI_SCALE);
    DrawOutlinedPanelBorder(dl, borderColor, tl, br);
}
void UI::DrawOutlinedPanelBorder(ImDrawList *dl, const ImColor &borderColor, const ImVec2 &tl, const ImVec2 &br) {
    dl->AddRect(tl, br, borderColor, BlackboardPanelButtonRounding * DPI_SCALE, 0, 1 * DPI_SCALE);
}

bool UI::BlackboardPanelButton(const char *label, const ImVec2 &tl, const ImVec2 &br, ImGuiButtonFlags flags) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;

    const ImRect bb(tl, br);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1 * DPI_SCALE);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, BlackboardPanelButtonRounding * DPI_SCALE);

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    ImGui::RenderNavHighlight(bb, id);
    ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

    if (g.LogEnabled)
        ImGui::LogSetNextTextDecoration("[", "]");
    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    ImGui::PopStyleVar(2);
    return pressed;
}

