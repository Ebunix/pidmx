#pragma once
#include <imgui.h>

namespace ImGui {
	enum class ColorPreset {
		ButtonNegative,
		ButtonPositive,
		TextNegative,
		TextPositive,
	};

	struct SliderTintPreset {
		ImVec4 grab;
		ImVec4 grabActive;
		ImVec4 bg;
		ImVec4 bgHover;
		ImVec4 bgActive;
	};

	extern ImFont* fontRegular;
	extern ImFont* fontBackgroundMassive;
	extern ImFont* fontMonospace;

	extern SliderTintPreset sliderTintPresetUnassigned;
	extern SliderTintPreset sliderTintPresetAssigned;

	void PushColorPreset(ColorPreset preset);
	void PopColorPreset(ColorPreset preset);
	SliderTintPreset MakeTintPreset(const SliderTintPreset& original, const ImVec4& tint);

	void PushSliderColor(const SliderTintPreset& preset);
	void PopSliderColor();


	bool InputIntClamp(const char* label, int* v, int min, int max, int* step = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);
}