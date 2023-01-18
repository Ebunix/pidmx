#pragma once
#include <stdint.h>
#include <stdio.h>
#include <engine/Modulator.h>

#define UNIVERSE_SIZE 512

class Universe {
public:
	Universe(int numHistoryStates, int id);
	~Universe();

	const inline int GetHistorySize() const { return historyStates; }
	uint8_t* GetBufferAtHistory(int history) const;
	void SetCurrentValues(uint8_t* data);
	void PrepareUpdate();
	void Update();
	bool Open(int id);
	void GetSlotHistory(uint8_t* target, int slot) const;
	void SetSlotModulationSource(int slot, Modulator* mod);

private:
	uint8_t* slotBuffer = nullptr;
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
