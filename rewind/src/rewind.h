#pragma once
#include <vector>
#include <stdint.h>

namespace Rewind {
	typedef bool(*Step)(void*);

	struct BufferStep {
		void* data;
		Step undo;
		Step redo;
	};

	struct BufferEntry {
		char* name;
		std::vector<void*> arrayDeallocs;
		std::vector<void*> ptrDeallocs;
		std::vector<BufferStep> steps;
	};

	struct Buffer {
		std::vector<BufferEntry> entries;
		int64_t index = 0;
	};

	void SetBuffer(Buffer* buffer);
	bool BeginRewindEntry(const char* fmt, ...);
	void RecordStep(Step undo, Step redo, void* data);
	void AddDeallocResourceArray(void* array);
	void AddDeallocResource(void* array);
	void EndRewindEntry(bool exec);

	bool Redo(int count);
	bool Undo(int count);
}