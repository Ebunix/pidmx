#pragma once
#include "Panel.h"
#include <stdio.h>
#include <engine/Universe.h>

class PanelChannelEditor : public Panel {
public:
	PanelChannelEditor(): Panel("Channel editor", true) {}
	void DrawIntern() override;
	void DrawMenuBar() override;

private:
	Slots slots;

	Universe* universe = nullptr;	
	const int entrySize = 112;
	bool horizontalOverflow = false;
};