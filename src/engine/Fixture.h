#pragma once
#include "Object.h"
#include "Universe.h"
#include "Parameter.h"
#include <js.h>
#include <ImGuiExt.h>

enum class FixtureControlType {
	Simple,
	Color,
	Select
};
FixtureControlType ControlTypeFromName(const std::string& name);
const std::string NameFromControlType(FixtureControlType param);

struct FixtureControlRange {
	int from;
	int to;
	std::string label;
};

struct FixtureControl {
	FixtureControlType type;
	std::vector<int> slots;
	std::vector<ImVec4> tint;
	std::string colorOrder;
	std::vector<FixtureControlRange> selects;
};

struct FixtureSlot {
	std::string name;
	ImGui::SliderTintPreset tint;
};

struct FixturePreset {
	std::string name;
	std::vector<FixtureSlot> slots;
	std::vector<FixtureControl> controls;
};

class Fixture: public EngineObject {
public:
	Fixture(uint16_t startSlot, uint16_t slotCount, Universe* universe, FixturePreset* preset);
	static void RegisterPreset(const v8::FunctionCallbackInfo<v8::Value>& info);

	bool SetUniverse(Universe* universe);
	inline const uint16_t GetSlot() const { return slot; }
	inline const uint16_t GetSlotCount() const { return slotCount; }
	inline const FixturePreset* GetPreset() const { return sourcePreset; }

private:
	uint16_t slot = 0;
	uint16_t slotCount = 0;
	Universe* universe = nullptr;
	FixturePreset* sourcePreset = nullptr;

	void UnOccupy();
	void Occupy();
};