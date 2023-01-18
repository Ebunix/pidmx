#pragma once
#include "Panel.h"
#include <stdio.h>
#include <Universe.h>

class PanelChannelMonitor : public Panel {
public:
	PanelChannelMonitor(Universe* universe): Panel("Channel monitor", true), universe(universe) {
		valueBuffer = new float[universe->GetHistorySize()];
		slotBuffer = new uint8_t[universe->GetHistorySize()];
	}
	~PanelChannelMonitor() {
		delete[] valueBuffer;
		delete[] slotBuffer;
	}
	void DrawIntern() override;
	void DrawMenuBar() override;

private:
	Universe* universe;
	Slots slots;

	float* valueBuffer;
	uint8_t* slotBuffer;
	const int entrySize = 96;
	bool horizontalOverflow = false;
};