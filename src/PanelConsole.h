#pragma once
#include "Panel.h"
#include <vector>

class PanelConsole : public Panel {
public:
	PanelConsole(): Panel("Console", false) {}

	void DrawIntern() override;
};