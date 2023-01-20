#pragma once
#include "Panel.h"
#include <stdio.h>
#include <engine/Universe.h>

class PanelChannelMonitor : public Panel {
public:
	PanelChannelMonitor(): Panel("Channel monitor", true) {}
	~PanelChannelMonitor() {
		delete[] valueBuffer;
		delete[] slotBuffer;
	}
	void DrawIntern() override;
	void DrawMenuBar() override;
	inline void SetUniverse(Universe* universe) {
		if (valueBuffer) {
			delete[] valueBuffer;
		}
		if (slotBuffer) {
			delete[] slotBuffer;
		}
		valueBuffer = new float[universe->GetHistorySize()];
		slotBuffer = new uint8_t[universe->GetHistorySize()];
		this->universe = universe;
	}

private:
	Slots slots;

	Universe* universe = nullptr;
	float* valueBuffer = nullptr;
	uint8_t* slotBuffer = nullptr;
	const int entrySize = 112;
	bool horizontalOverflow = false;
};