#pragma once
#include <string>
#include <ImGuiExt.h>

class Panel {
public:
	std::string title;
	ImGuiWindowFlags windowFlags = 0;
	bool open = true;

	Panel(const std::string& title, bool menuBar): title(title), menuBar(menuBar) {
		windowFlags |= menuBar ? ImGuiWindowFlags_MenuBar : 0;
	}
	void Draw() {
		if (!open) {
			return;
		}
		ImGui::SetNextWindowSize(ImVec2(200, 75), ImGuiCond_Once);
		if (ImGui::Begin(title.c_str(), &open, windowFlags)) {
			if (menuBar && ImGui::BeginMenuBar()) {
				DrawMenuBar();
				ImGui::EndMenuBar();
			}
			DrawIntern();
		}
		ImGui::End();
	}

	virtual void DrawIntern() = 0;
	virtual void DrawMenuBar() {};

private:
	
	bool menuBar = false;
};

