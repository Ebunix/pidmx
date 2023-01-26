#pragma once
#include "Object.h"
#include "Universe.h"
#include "Parameter.h"
#include <js.h>
#include <ImGuiExt.h>
#include "EngineCereal.h"

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



struct FixturePatch {
	std::string name;
	int fixtureId = 0;
	int universe = 0;
	int channel = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(
			cereal::make_nvp("name", name), 
			cereal::make_nvp("fixId", fixtureId),
			cereal::make_nvp("universe", universe),
			cereal::make_nvp("channel", channel));
	}
};

class FixtureNew : public EngineObject {
public:
	FixturePatch patch;

	static bool RedoPatch(FixturePatch* patch);
	static bool UndoPatch(FixturePatch* patch);
	static void PatchFixture(const v8::FunctionCallbackInfo<v8::Value>& info);

	FixtureNew(const FixturePatch& patch): patch(patch) {}
	FixtureNew() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(
			cereal::make_nvp("patch", patch));
	}

	inline void SetFootprint(int count) {
		dmxFootprint = count;
		if (channelValues) {
			delete[] channelValues;
		}
		channelValues = new float[count];
	}
	inline int GetFootprint() const { return dmxFootprint; }

private:
	int dmxFootprint = 0;
	float* channelValues = nullptr;
};

bool FixturePatchStringToUniverseChannel(char* text, size_t len, int* universe, int* channel);


