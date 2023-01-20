#pragma once
#include "Panel.h"

class PanelFixtureMap : public Panel {
public:
	PanelFixtureMap(): Panel("Fixture map", true) {}

	void DrawIntern() override;
	void DrawMenuBar() override;

private:
	void DrawAddFixtureModal();
};
