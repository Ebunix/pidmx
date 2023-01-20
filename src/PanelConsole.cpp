#include "PanelConsole.h"
#include <pidmx_utils.h>
#include <js.h>

char commandBuffer[5000];
std::vector<std::string> consolePanelLogBuffer;

void PanelConsole::DrawIntern()
{
	static bool lastActive = false;
	ImGui::PushFont(ImGui::fontMonospace);

	ImVec2 size = ImGui::GetContentRegionAvail();
	size.y -= 70;
	ImGui::BeginChildFrame(ImGui::GetID("frame"), size);
	for (const auto& str : consolePanelLogBuffer) {
		ImGui::TextWrapped("%s", str.c_str());
	}
	ImGui::EndChild();

	ImGui::PushID(1);
	ImGui::SetNextItemWidth(-1);
	ImGui::InputTextWithHint("##v", "Command...", commandBuffer, sizeof(commandBuffer));
	bool active = ImGui::IsItemActive();
	if (lastActive && ImGui::IsKeyDown(ImGuiKey_Enter)) {
		js::exec(commandBuffer);
		commandBuffer[0] = 0;
	}
	lastActive = active;
	ImGui::PopID();

	ImGui::PopFont();
}