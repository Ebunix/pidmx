#include "Engine.h"
#include <rewind.h>

using namespace v8;

Engine* engine;
Rewind::Buffer rewindBuffer;

void Engine::BeginUndoStep(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	if (info.Length() >= 1) {
		Rewind::BeginRewindEntry("Script: %s", V8CStr(info.GetIsolate()->GetCurrentContext(), info[0]).c_str());
	}
	else {
		Rewind::BeginRewindEntry("Script: Undo step %li", rewindBuffer.index);
	}
}

void Engine::EndUndoStep(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	if (info.Length() >= 1 && info[0]->IsBoolean() && !info[0].As<Boolean>()->Value()) {
		Rewind::EndRewindEntry(false);
	}
	else {
		Rewind::EndRewindEntry(true);
	}
}

Engine::Engine()
{
	showFile = std::make_unique<Showfile>();
	Rewind::SetBuffer(&rewindBuffer);
	time = 0;
	startTime = lastUpdate = std::chrono::high_resolution_clock::now();

	js::ObjectTemplate engineBuilder;
	engineBuilder.Begin()
		->Function("addModulator", Modulator::Register)
		->Function("addUniverse", Universe::Register)
		->Function("addFixturePreset", Fixture::RegisterPreset)
		->Function("patchFixture", FixtureNew::PatchFixture)
		->Function("beginUndoStep", Engine::BeginUndoStep)
		->Function("endUndoStep", Engine::EndUndoStep)
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

void Engine::PatchFixtures(FixturePatch *fixtures, int count)
{
	Rewind::BeginRewindEntry("Patch %i fixtures", count);
	Rewind::AddDeallocResourceArray(fixtures);
	for (int i = 0; i < count; i++) {
		Rewind::RecordStep((Rewind::Step)FixtureNew::UndoPatch, (Rewind::Step)FixtureNew::RedoPatch, fixtures + i);
	}
	Rewind::EndRewindEntry(true);
}

std::shared_ptr<FixtureNew> Engine::GetFixture(int fixtureId) const
{
	for (auto fix : showFile->fixtures) {
		if (fix->patch.fixtureId == fixtureId) {
			return fix;
		}
	}
	return std::shared_ptr<FixtureNew>();
}

void Engine::SaveShowFile(const std::string &file)
{
    std::ofstream os(file);
    cereal::XMLOutputArchive archive(os);

	archive(showFile);
}

void Engine::LoadShowFile(const std::string &file)
{
    std::ifstream is(file);
    cereal::XMLInputArchive archive(is);

	archive(showFile);
}
