#pragma once
#include "Panel.h"

class PanelPatch : public Panel {
public:
	PanelPatch(): Panel("Patch", true) {}

	void DrawIntern() override;
	void DrawMenuBar() override;

private:
	void DrawAddFixtureModal();
};
