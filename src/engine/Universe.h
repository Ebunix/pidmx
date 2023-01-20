#pragma once
#include <stdint.h>
#include <stdio.h>
#include <engine/Modulator.h>

class Fixture;

#define UNIVERSE_SIZE 512
#define UNIVERSE_HISTORY_SIZE 60

class Universe: public EngineObject {
public:
	Universe(int numHistoryStates, int id);
	~Universe();
	static void Register(const v8::FunctionCallbackInfo<v8::Value>& info);

	const inline int GetHistorySize() const { return historyStates; }
	uint8_t* GetBufferAtHistory(int history) const;
	void SetCurrentValues(uint8_t* data);
	void PrepareUpdate();
	void Update();
	bool Open(int id);
	void GetSlotHistory(uint8_t* target, int slot) const;
	void SetSlotModulationSource(int slot, Modulator* mod);
	inline const bool GetSlotRangeAvailable(int min, int max) const {
		for (int i = min; i < max; i++) {
			if (occupationMap[i] != nullptr) { return false; }
		}
		return true;
	}
	inline const bool GetSlotAvailable(int slot) const {
		return occupationMap[slot] == nullptr;
	}
	inline void SetSlotRangeOccupied(int min, int max, Fixture* who) {
		for (int i = min; i < max; i++) {
			occupationMap[i] = who;
		}
	}
	inline const Fixture* GetOccupant(int slot) {
		return occupationMap[slot];
	}

private:
	uint8_t* slotBuffer = nullptr;
	Fixture* occupationMap[UNIVERSE_SIZE] = { nullptr };
	int historyStates = 0;
	int historyOffset = 0;
	bool isChanged = false;
	FILE* universeDevice = nullptr;
	std::map<int, Modulator*> modulators;

	const inline int HistoryDataOffset(int history) const { return ((historyOffset - history) % historyStates) * UNIVERSE_SIZE; }
	void Advance();
};

struct Slots {
	uint8_t* buffer;
	uint8_t* operator[](int index) {
		if (index > UNIVERSE_SIZE || index < 0) {
			return 0;
		}
		return &buffer[index];
	}
	void ReadFrom(const Universe* universe, int history = 0) {
		buffer = universe->GetBufferAtHistory(history);
	}
	void WriteTo(Universe* universe) {
		universe->SetCurrentValues(buffer);
	}
};
