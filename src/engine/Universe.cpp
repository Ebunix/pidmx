#include "Universe.h"
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "Engine.h"

using namespace v8;

Universe::Universe(int numHistoryStates, int id): EngineObject(), historyStates(numHistoryStates) {
	slotBuffer = new uint8_t[numHistoryStates * UNIVERSE_SIZE];
	Open(id);
}

Universe::~Universe() {
	delete[] slotBuffer;
}

void Universe::Register(const v8::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() != 2) {
		LOG_ERROR_FORMAT("Wrong number of arguments. Expected 2, got %i", info.Length());
		js::global::isolate->ThrowError(V8StrCheck("Wrong number of arguments."));
		return;
	}

	Local<Context> ctx = info.GetIsolate()->GetCurrentContext();

	std::string name = V8CStr(info[0]);
	Local<Number> data = info[1].As<Number>();
	int id = (int)data->Value();

	Universe* universe = new Universe(UNIVERSE_HISTORY_SIZE, id);
	universe->name = name;
	engine->universes.push_back(universe);
}

uint8_t* Universe::GetBufferAtHistory(int history) const {
	return slotBuffer + HistoryDataOffset(history);
}

void Universe::SetCurrentValues(uint8_t* data) {
	uint8_t* target = slotBuffer + HistoryDataOffset(0);
	memcpy(target, data, UNIVERSE_SIZE);
	isChanged = true;
}

void Universe::PrepareUpdate()
{
	for (const auto& mod : modulators) {
		mod.second->PrepareUpdate();
	}
}

void Universe::Update() {
	if (!universeDevice) {
		return;
	}

	Advance();
	uint8_t* buffer = GetBufferAtHistory(0);
	for (const auto& mod : modulators) {
		float value = mod.second->GetValue();
		uint8_t slotValue = (uint8_t)fmin(fmax(value * 255, 0.0f), 255.0f);
		if (slotValue != buffer[mod.first]) {
			buffer[mod.first] = slotValue;
			isChanged = true;
		}
	}

	if (isChanged) {
		fwrite(buffer, 1, UNIVERSE_SIZE, universeDevice);
	}
}

bool Universe::Open(int id) {
	if (universeDevice) {
		fclose(universeDevice);
	}
	char devBuffer[50];
	snprintf(devBuffer, sizeof(devBuffer), "/dev/dmx%i", id);

	isChanged = false;
	historyOffset = 0;
	memset(slotBuffer, 0, UNIVERSE_SIZE * historyStates);
	universeDevice = fopen(devBuffer, "r+b");
	if (universeDevice) {
		fread(GetBufferAtHistory(0), 1, UNIVERSE_SIZE, universeDevice);
	}
	return universeDevice != nullptr;
}

void Universe::GetSlotHistory(uint8_t* target, int slot) const {
	for (int i = 0; i < historyStates; i++) {
		uint8_t* src = slotBuffer + HistoryDataOffset(i);
		target[i] = src[slot];
	}
}

void Universe::SetSlotModulationSource(int slot, Modulator *mod)
{
	auto index = modulators.find(slot);
	if (mod == nullptr && index != modulators.end()) {
		modulators.erase(index);
	}
	else if (mod) {
		modulators.insert_or_assign(slot, mod);
	}
}

void Universe::Advance()
{
	uint8_t* buffer = GetBufferAtHistory(0);
	historyOffset++;
	memcpy(GetBufferAtHistory(0), buffer, UNIVERSE_SIZE);
}