#include "Engine.h"

Engine* engine;

Engine::Engine()
{
	time = 0;
	startTime = lastUpdate = std::chrono::high_resolution_clock::now();

	js::ObjectTemplate engineBuilder;
	engineBuilder.Begin()
		->Function("addModulator", Modulator::Register)
		->Function("addUniverse", Universe::Register)
		->Function("addFixturePreset", Fixture::RegisterPreset)
		->BuildGlobal("engine");
}

void Engine::Update()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto delta = now - lastUpdate;
	time += delta.count() / 1000000000.0;
	lastUpdate = now;

	for (const auto& u : universes) {
		u->PrepareUpdate();
	}
	for (const auto& u : universes) {
		u->Update();
	}
}

