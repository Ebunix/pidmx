#pragma once
#include <vector>
#include <chrono>
#include "Modulator.h"
#include "Universe.h"
#include "Fixture.h"
#include "Console.h"
#include <rewind.h>
#include "Collection.h"
#include "Showfile.h"


class Engine {
public:
	std::unique_ptr<Showfile> showFile;
	std::vector<Modulator*> modulators;
	std::vector<Universe*> universes;
	std::vector<FixturePreset*> fixturePresets;
	std::vector<Collection<FixtureNew*>> fixtureCollections;
	float displayScale = 1.0f;

	static void BeginUndoStep(const v8::FunctionCallbackInfo<v8::Value> &info);
	static void EndUndoStep(const v8::FunctionCallbackInfo<v8::Value> &info);

	Engine();
	void Update();
	inline const double GetTime() const { return time; }

	void PatchFixtures(FixturePatch* fixtures, int count);
	std::shared_ptr<FixtureNew> GetFixture(int fixtureId) const;

	void SaveShowFile(const std::string& file);
	void LoadShowFile(const std::string& file);

private:
	std::chrono::_V2::system_clock::time_point startTime;
	std::chrono::_V2::system_clock::time_point lastUpdate;
	double time;

};

extern Engine* engine;
