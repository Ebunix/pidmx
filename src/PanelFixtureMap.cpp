#include "PanelFixtureMap.h"
#include <engine/Engine.h>

void PanelFixtureMap::DrawIntern() {
	if (ImGui::BeginTable("table1", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Slot start");
		ImGui::TableSetupColumn("Slot count");
		ImGui::TableHeadersRow();

		for (const auto& fixture : engine->fixtures) {
			ImGui::TableNextRow();
			
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", fixture->name.c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%i", fixture->GetSlot());
			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%i", fixture->GetSlotCount());
		}
		ImGui::EndTable();
	}
}

void PanelFixtureMap::DrawMenuBar() {
	bool openFixturesPopup = false;
	if (ImGui::BeginMenu("Edit")) {
		openFixturesPopup = ImGui::MenuItem("Add Fixtures");
		ImGui::EndMenu();
	}

	if (openFixturesPopup) { ImGui::OpenPopup("Add fixtures"); }
	ImGui::SetNextWindowSize(ImVec2(300 * engine->displayScale, 0));
	if (ImGui::BeginPopupModal("Add fixtures", nullptr, ImGuiWindowFlags_NoResize)) {
		DrawAddFixtureModal();
		ImGui::EndPopup();
	}

}

void PanelFixtureMap::DrawAddFixtureModal() {
	static int step = 1;

	static FixturePreset* selectedPreset = nullptr;
	static int slotCount = 1;
	static int fixtureCount = 1;
	static int startSlot = 1;
	static Universe* targetUniverse = nullptr;
	static char printBuffer[2000];

	if (ImGui::BeginCombo("Fixture type", selectedPreset ? selectedPreset->name.c_str() : "Custom...")) {
		if (ImGui::Selectable("Custom...")) {
			selectedPreset = nullptr;
		}
		for (const auto& preset : engine->fixturePresets) {
			if (ImGui::Selectable(preset->name.c_str())) {
				selectedPreset = preset;
				slotCount = selectedPreset->slots.size();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Spacing();

	if (ImGui::BeginCombo("Universe", targetUniverse ? targetUniverse->name.c_str() : "None")) {
		if (ImGui::Selectable("None")) {
			targetUniverse = nullptr;
		}
		for (const auto& universe : engine->universes) {
			if (ImGui::Selectable(universe->name.c_str())) {
				targetUniverse = universe;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::InputIntClamp("Start slot", &startSlot, 1, UNIVERSE_SIZE, &step);
	ImGui::Spacing();

	ImGui::BeginDisabled(selectedPreset);
	ImGui::InputIntClamp("Slot count", &slotCount, 1, UNIVERSE_SIZE, &step);
	ImGui::EndDisabled();
	ImGui::InputIntClamp("Fixture count", &fixtureCount, 1, UNIVERSE_SIZE, &step);
	ImGui::Spacing();


	int endSlot = startSlot - 1 + slotCount * fixtureCount;

	printBuffer[0] = 0;
	static std::vector<uint16_t> collisions;
	if (targetUniverse) {
		collisions.clear();
		for (uint16_t i = startSlot - 1; i < endSlot; i++) {
			if (!targetUniverse->GetSlotAvailable(i)) {
				collisions.push_back(i + 1);
			}
		}
		static char smallPrintBuffer[10];
		int size = collisions.size();
		for (int i = 0; i < size; i++) {
			snprintf(smallPrintBuffer, sizeof(smallPrintBuffer), i == 0 ? "%i" : (i == size - 1 ? " and %i" : ", %i"), collisions[i]);
			strncat(printBuffer, smallPrintBuffer, sizeof(printBuffer) - strlen(printBuffer) - 1);
		}
	}

	const char* universeName = targetUniverse ? targetUniverse->name.c_str() : "an undefuned universe";
	if (startSlot == endSlot) {
		ImGui::TextWrapped("This will occupy slot %i on %s", startSlot, universeName);
	}
	else {
		ImGui::TextWrapped("This will occupy slots %i-%i on %s", startSlot, endSlot, universeName);
	}

	ImGui::PushColorPreset(ImGui::ColorPreset::TextNegative);
	if (endSlot > UNIVERSE_SIZE) {
		ImGui::TextWrapped("This would occupy slots beyond %s's maximum slot count. Either reduce the number of fixtures, or enable universe wrapping.", universeName);
	}

	if (collisions.size() > 0) {
		ImGui::TextWrapped("Occupied slot conflicts.");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("%s", printBuffer);
		}
	}
	ImGui::PopColorPreset(ImGui::ColorPreset::TextNegative);

	ImGui::Spacing();
	ImGui::PushColorPreset(ImGui::ColorPreset::ButtonNegative);
	if (ImGui::Button("Cancel")) {
		ImGui::CloseCurrentPopup();
	}
	ImGui::PopColorPreset(ImGui::ColorPreset::ButtonNegative);
	ImGui::SameLine();
	ImGui::PushColorPreset(ImGui::ColorPreset::ButtonPositive);
	if (ImGui::Button("Okay")) {
		ImGui::CloseCurrentPopup();

		for (int i = 0; i < fixtureCount; i++) {
			uint16_t fixtureStartSlot = startSlot - 1 + (i * slotCount);
			Fixture* f = new Fixture(fixtureStartSlot, (uint16_t)slotCount, targetUniverse, selectedPreset);
			f->name = selectedPreset ? selectedPreset->name : "Unnamed Fixture";
			engine->fixtures.push_back(f);
		}
	}
	ImGui::PopColorPreset(ImGui::ColorPreset::ButtonPositive);
}
