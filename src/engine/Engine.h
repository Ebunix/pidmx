#pragma once
#include <vector>
#include <chrono>
#include "Modulator.h"

class Engine {
public:
	std::vector<Modulator*> modulators;
	
	Engine();
	void Update();
	inline const double GetTime() const { return time; }

	void RegisterModulator();


private:
	std::chrono::_V2::system_clock::time_point startTime;
	std::chrono::_V2::system_clock::time_point lastUpdate;
	double time;

};

extern Engine* engine;
