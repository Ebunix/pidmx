#include "PanelPatch.h"
#include <engine/Engine.h>

void PanelPatch::DrawIntern() {
	if (ImGui::BeginTable("table0", 2, ImGuiTableFlags_SizingStretchSame)) {
		ImGui::TableSetupColumn("Collections", ImGuiTableColumnFlags_None, 0.35f);
		ImGui::TableSetupColumn("Fixtures");
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("table1", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
			ImGui::TableSetupColumn("Collection");
			ImGui::TableHeadersRow();

			for (auto fixture : engine->showFile->fixtures) {
				ImGui::TableNextRow();
				
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", fixture->patch.name.c_str());
			}
			ImGui::EndTable();
		}		
		
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("table2", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
			ImGui::TableSetupColumn("ID");
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Patch");
			ImGui::TableHeadersRow();

			for (auto fixture : engine->showFile->fixtures) {
				ImGui::TableNextRow();
				
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%i", fixture->patch.fixtureId);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", fixture->patch.name.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("Unknown fixture type");
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%i.%03i", fixture->patch.universe + 1, fixture->patch.channel + 1);
			}
			ImGui::EndTable();
		}

		ImGui::EndTable();
	}
}

void PanelPatch::DrawMenuBar() {
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

void PanelPatch::DrawAddFixtureModal() {
	static int step = 1;
	static int quantity = 1;
	static int fixtureId = 1;
	static char patchIdBuffer[6];
	static int targetUniverse = 0;
	static int targetChannel = 0;
	static FixturePreset* selectedPreset = nullptr;
	static int footprint = 0;
	static const char* errorMessage = nullptr;

	if (ImGui::BeginCombo("Fixture type", selectedPreset ? selectedPreset->name.c_str() : "Custom...")) {
		if (ImGui::Selectable("Custom...")) {
			selectedPreset = nullptr;
		}
		for (const auto& preset : engine->fixturePresets) {
			if (ImGui::Selectable(preset->name.c_str())) {
				selectedPreset = preset;
				footprint = selectedPreset->slots.size();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Spacing();

	if (!selectedPreset) {
		ImGui::InputIntClamp("Footprint", &footprint, 1, UNIVERSE_SIZE, &step);
	}
	ImGui::InputIntClamp("Quantity", &quantity, 1, UNIVERSE_SIZE, &step);
	ImGui::InputIntClamp("Fixture ID", &fixtureId, 1, 999999, &step);
	ImGui::InputText("Patch", patchIdBuffer, sizeof(patchIdBuffer));

	if (!FixturePatchStringToUniverseChannel(patchIdBuffer, sizeof(patchIdBuffer), &targetUniverse, &targetChannel)) {
		ImGui::PushColorPreset(ImGui::ColorPreset::TextNegative);
		ImGui::TextWrapped("'%s' is not a valid patch string", patchIdBuffer);
		ImGui::PopColorPreset(ImGui::ColorPreset::TextNegative);
	}
	ImGui::Spacing();

	if (errorMessage) {
		ImGui::PushColorPreset(ImGui::ColorPreset::TextNegative);
		ImGui::TextWrapped("%s", errorMessage);
		ImGui::PopColorPreset(ImGui::ColorPreset::TextNegative);
		ImGui::Spacing();
	}

	ImGui::PushColorPreset(ImGui::ColorPreset::ButtonNegative);
	if (ImGui::Button("Cancel")) {
		ImGui::CloseCurrentPopup();
	}
	ImGui::PopColorPreset(ImGui::ColorPreset::ButtonNegative);
	ImGui::SameLine();
	ImGui::PushColorPreset(ImGui::ColorPreset::ButtonPositive);
	if (ImGui::Button("Okay")) {
		
		bool valid = true;
		int slotCount = selectedPreset ? selectedPreset->slots.size() : footprint;
		FixturePatch* patchInfo = new FixturePatch[quantity];
		for (int i = 0; i < quantity; i++) {
			patchInfo[i].channel = targetChannel - 1 + (i * slotCount);
			patchInfo[i].fixtureId = fixtureId + i;
			patchInfo[i].name = selectedPreset ? selectedPreset->name.c_str() : "Unnamed Fixture";
			patchInfo[i].universe = targetUniverse - 1;
			if (engine->GetFixture(patchInfo[i].fixtureId)) {
				valid = false;
				break;
			}
		}
		if (!valid) {
			errorMessage = "Fixture ID collision!";
			delete[] patchInfo;
		}
		else {
			engine->PatchFixtures(patchInfo, quantity);
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::PopColorPreset(ImGui::ColorPreset::ButtonPositive);
}
