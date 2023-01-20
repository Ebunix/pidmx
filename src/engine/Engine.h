#pragma once
#include <vector>
#include <chrono>
#include "Modulator.h"
#include "Universe.h"
#include "Fixture.h"

class Engine {
public:
	std::vector<Modulator*> modulators;
	std::vector<Universe*> universes;
	std::vector<Fixture*> fixtures;
	std::vector<FixturePreset*> fixturePresets;
	float displayScale = 1.0f;

	Engine();
	void Update();
	inline const double GetTime() const { return time; }


private:
	std::chrono::_V2::system_clock::time_point startTime;
	std::chrono::_V2::system_clock::time_point lastUpdate;
	double time;

};

extern Engine* engine;
