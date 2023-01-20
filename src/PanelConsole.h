#pragma once
#include "Panel.h"
#include <vector>

extern std::vector<std::string> consolePanelLogBuffer;

class PanelConsole : public Panel {
public:
	PanelConsole(): Panel("Console", false) {}

	void DrawIntern() override;
};