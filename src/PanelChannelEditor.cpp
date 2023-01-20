#include "PanelChannelEditor.h"
#include <engine/Engine.h>

void PanelChannelEditor::DrawIntern() {
	if (!universe) {
		ImGui::Text("No universe selected!");
		return;
	}
	
	slots.ReadFrom(universe);

	ImGuiStyle& style = ImGui::GetStyle();
	float windowPadding = style.WindowPadding.x;
	float elementPadding = style.ItemSpacing.x;
	ImVec2 windowSize = ImGui::GetWindowSize();
	int entriesPerRow;
	if (horizontalOverflow) {
		entriesPerRow = UNIVERSE_SIZE;
	}
	else {
		entriesPerRow = (windowSize.x / (entrySize + elementPadding)) - 1;
	}
	if (entriesPerRow <= 0) {
		return;
	}

	bool changed = false;
	char label[20];
	
	for (int i = 0; i < UNIVERSE_SIZE; i++) {
		const Fixture* occ = universe->GetOccupant(i);
		const FixturePreset* preset = nullptr;
		int slotOffset = -1;
		if (occ) {
			slotOffset = i - occ->GetSlot();
			preset = occ->GetPreset();
			ImGui::PushSliderColor(preset ? preset->slots[slotOffset].tint : ImGui::sliderTintPresetAssigned);
		}
		else {
			ImGui::PushSliderColor(ImGui::sliderTintPresetUnassigned);
		}

		ImGui::PushID(i);
		if (i % entriesPerRow != 0) {
			ImGui::SameLine();
		}

		snprintf(label, sizeof(label), "Slot %i\n\n%%d", i + 1);
		int value = *slots[i];
		changed = ImGui::VSliderInt("##v", ImVec2(entrySize, entrySize * 2), &value, 0, 255, label) || changed;
		if (preset && (ImGui::IsItemHovered() || ImGui::IsItemActive())) {
			ImGui::SetTooltip("%s", preset->slots[slotOffset].name.c_str());
		}
		*slots[i] = (uint8_t)value;
		ImGui::PopID();

		ImGui::PopSliderColor();
	}

	if (changed) {
		slots.WriteTo(universe);
	}
}

void PanelChannelEditor::DrawMenuBar() {
	if (ImGui::BeginMenu("Menu")) {
		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Horizontal overflow", "", &horizontalOverflow)) {
				if (horizontalOverflow) {
					windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
				}
				else {
					windowFlags &= ~ImGuiWindowFlags_HorizontalScrollbar;
				}
			}
			if (ImGui::BeginCombo("Universe", universe ? universe->name.c_str() : "None")) {
				if (ImGui::Selectable("None")) {
					universe = nullptr;
				}
				for (const auto& u : engine->universes) {
					if (ImGui::Selectable(u->name.c_str())) {
						universe = u;
					}
				}
				ImGui::EndCombo();
			} 
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Tools")) {
		if (ImGui::MenuItem("Set all to max")) {
			memset(slots.buffer, 255, UNIVERSE_SIZE);
			slots.WriteTo(universe);
		}
		if (ImGui::MenuItem("Set all to min")) {
			memset(slots.buffer, 0, UNIVERSE_SIZE);
			slots.WriteTo(universe);
		}

		ImGui::EndMenu();
	}
}
