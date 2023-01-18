#include "PanelChannelMonitor.h"

void PanelChannelMonitor::DrawIntern() {
	ImGuiStyle& style = ImGui::GetStyle();
	float windowPadding = style.WindowPadding.x;
	float elementPadding = style.ItemSpacing.x;
	ImVec2 windowSize = ImGui::GetWindowSize();
	int entriesPerRow;
	if (horizontalOverflow) {
		entriesPerRow = UNIVERSE_SIZE;
	}
	else {
		entriesPerRow = (windowSize.x / (entrySize + elementPadding));
	}
	if (entriesPerRow <= 0) {
		return;
	}

	int history = universe->GetHistorySize();
	for (int i = 0; i < UNIVERSE_SIZE; i++) {
		universe->GetSlotHistory(slotBuffer, i);
		for (int v = 0; v < history; v++) {
			valueBuffer[v] = slotBuffer[v];
		}

		ImGui::PushID(i);
		if (i % entriesPerRow != 0) {
			ImGui::SameLine();
		}
		ImGui::PlotLines("##v", valueBuffer, history, 0, NULL, 0.0f, 255.0f, ImVec2(entrySize, entrySize * 2));
		ImGui::PopID();
	}
}

void PanelChannelMonitor::DrawMenuBar() {
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
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}
