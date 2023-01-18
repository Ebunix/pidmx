#pragma once
#include "Panel.h"
#include <stdio.h>
#include <Universe.h>

class PanelChannelEditor : public Panel {
public:
	PanelChannelEditor(Universe* universe): Panel("Channel editor", true), universe(universe) {}
	void DrawIntern() override;
	void DrawMenuBar() override;

private:
	Universe* universe;
	Slots slots;
	
	const int entrySize = 96;
	bool horizontalOverflow = false;
};