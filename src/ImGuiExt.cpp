#include "ImGuiExt.h"
#include <math.h>

const ImVec4 redDefault(0.6f, 0.2f, 0.2f, 1.0f);
const ImVec4 redHover(0.7f, 0.3f, 0.3f, 1.0f);
const ImVec4 redActive(0.5f, 0.1f, 0.1f, 1.0f);

const ImVec4 redBgDefault(0.6f * 0.5f, 0.2f * 0.5f, 0.2f * 0.5f, 1.0f);
const ImVec4 redBgHover(0.7f * 0.5f, 0.3f * 0.5f, 0.3f * 0.5f, 1.0f);
const ImVec4 redBgActive(0.5f * 0.5f, 0.1f * 0.5f, 0.1f * 0.5f, 1.0f);

const ImVec4 greenDefault(0.2f, 0.6f, 0.2f, 1.0f);
const ImVec4 greenHover(0.3f, 0.7f, 0.3f, 1.0f);
const ImVec4 greenActive(0.1f, 0.5f, 0.1f, 1.0f);

const ImVec4 greenBgDefault(0.2f * 0.5f, 0.6f * 0.5f, 0.2f * 0.5f, 1.0f);
const ImVec4 greenBgHover(0.3f * 0.5f, 0.7f * 0.5f, 0.3f * 0.5f, 1.0f);
const ImVec4 greenBgActive(0.1f * 0.5f, 0.5f * 0.5f, 0.1f * 0.5f, 1.0f);

const ImVec4 blueDefault(0.2f, 0.2f, 0.6f, 1.0f);
const ImVec4 blueHover(0.3f, 0.3f, 0.7f, 1.0f);
const ImVec4 blueActive(0.1f, 0.1f, 0.5f, 1.0f);

const ImVec4 blueBgDefault(0.2f * 0.5f, 0.2f * 0.5f, 0.6f * 0.5f, 1.0f);
const ImVec4 blueBgHover(0.3f * 0.5f, 0.3f * 0.5f, 0.7f * 0.5f, 1.0f);
const ImVec4 blueBgActive(0.1f * 0.5f, 0.1f * 0.5f, 0.5f * 0.5f, 1.0f);

const ImVec4 textRed(0.85f, 0.4f, 0.4f, 1.0f);
const ImVec4 textGreen(0.4f, 0.85f, 0.4f, 1.0f);
const ImVec4 textBlue(0.4f, 0.4f, 0.85f, 1.0f);

const ImVec4 unassignedDefault(0.51f * 0.35f, 0.51f * 0.35f, 0.51f * 0.35f, 1.00f);
const ImVec4 unassignedActive(0.86f * 0.35f, 0.86f * 0.35f, 0.86f * 0.35f, 1.00f);
const ImVec4 unassignedBgDefault(0.20f * 0.35f, 0.21f * 0.35f, 0.22f * 0.35f, 0.54f);
const ImVec4 unassignedBgHover(0.40f * 0.35f, 0.40f * 0.35f, 0.40f * 0.35f, 0.40f);
const ImVec4 unassignedBgActive(0.18f * 0.35f, 0.18f * 0.35f, 0.18f * 0.35f, 0.67f);
ImGui::SliderTintPreset ImGui::sliderTintPresetUnassigned {
	unassignedDefault,
	unassignedActive,
	unassignedBgDefault,
	unassignedBgHover,
	unassignedBgActive
};

const ImVec4 assignedDefault(0.61f, 0.61f, 0.61f, 1.00f);
const ImVec4 assignedActive(0.76f, 0.76f, 0.76f, 1.00f);
const ImVec4 assignedBgDefault(0.30f, 0.30f, 0.30f, 0.54f);
const ImVec4 assignedBgHover(0.50f, 0.50f, 0.50f, 0.40f);
const ImVec4 assignedBgActive(0.28f, 0.28f, 0.28f, 0.67f);
ImGui::SliderTintPreset ImGui::sliderTintPresetAssigned {
	assignedDefault,
	assignedActive,
	assignedBgDefault,
	assignedBgHover,
	assignedBgActive
};

ImFont* ImGui::fontRegular;
ImFont* ImGui::fontMonospace;

void ImGui::PushColorPreset(ColorPreset preset) {
	switch (preset) {
		case ColorPreset::ButtonNegative: {
			ImGui::PushStyleColor(ImGuiCol_Button, redDefault);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, redActive);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redHover);
			break;
		}
		case ColorPreset::ButtonPositive: {
			ImGui::PushStyleColor(ImGuiCol_Button, greenDefault);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, greenActive);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, greenHover);
			break;
		}
		case ColorPreset::TextNegative: {
			ImGui::PushStyleColor(ImGuiCol_Text, textRed);
			ImGui::PushStyleColor(ImGuiCol_TextDisabled, textRed);
			break;
		}
		case ColorPreset::TextPositive: {
			ImGui::PushStyleColor(ImGuiCol_Text, textGreen);
			ImGui::PushStyleColor(ImGuiCol_TextDisabled, textGreen);
			break;
		}
	}
}

void ImGui::PopColorPreset(ColorPreset preset) {
	switch (preset) {
		case ColorPreset::ButtonNegative:
		case ColorPreset::ButtonPositive: {
			ImGui::PopStyleColor(3);
			break;
		}
		case ColorPreset::TextNegative:
		case ColorPreset::TextPositive: {
			ImGui::PopStyleColor(2);
			break;
		}
	}
}

ImVec4 MulVec4(const ImVec4& a, const ImVec4& b) {
	return ImVec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

ImGui::SliderTintPreset ImGui::MakeTintPreset(const SliderTintPreset &original, const ImVec4 &tint) {
	return SliderTintPreset {
		MulVec4(original.grab, tint),
		MulVec4(original.grabActive, tint),
		MulVec4(original.bg, tint),
		MulVec4(original.bgHover, tint),
		MulVec4(original.bgActive, tint)
	};
}

void ImGui::PushSliderColor(const ImGui::SliderTintPreset& tint)
{
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, tint.grab);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, tint.grabActive);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, tint.bg);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, tint.bgHover);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, tint.bgActive);
}

void ImGui::PopSliderColor()
{
	ImGui::PopStyleColor(5);
}

bool ImGui::InputIntClamp(const char* label, int* v, int min, int max, int* step, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    bool result = InputScalar(label, ImGuiDataType_S32, (void*)v, step, NULL, format, flags);
	*v = fmin(fmax(*v, min), max);
	return result;
}