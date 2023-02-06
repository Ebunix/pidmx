#pragma once
#include "ICommand.h"
#include <vector>

struct CommandBufferEntry {
    std::vector<CommandInstance> steps;
    std::string name;
};

class CommandBuffer {
public:
    std::vector<CommandBufferEntry> buffer;

	void Push(CommandInstance command);
    void BeginEntry(const char* fmt, ...);
    void EndEntry();
	inline void Undo() { if (current > 0) MoveTo(current - 1); }
	inline void Redo() { if (buffer.size() > 0 && current < buffer.size()) MoveTo(current + 1); }
    void Transfer();

private:
	size_t current = 0;
    bool inEntry = false;
    CommandBufferEntry buildingEntry;
    std::vector<CommandBufferEntry> internalBuffer;

	void MoveTo(size_t index);
};

