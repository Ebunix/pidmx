#include "rewind.h"
#include <iostream>

Rewind::Buffer* currentBuffer = nullptr;
Rewind::BufferEntry* currentEntry = nullptr;
bool inStep = false;

void Rewind::SetBuffer(Rewind::Buffer *buffer)
{
    currentBuffer = buffer;
}

bool Rewind::BeginRewindEntry(const char *fmt, ...)
{
    if (!currentBuffer) {
        return false;
    }
    currentEntry = new BufferEntry();

    va_list va;
    va_start(va, fmt);

    vasprintf(&currentEntry->name, fmt, va);

    va_end(va);

    inStep = true;
    return true;
}

void Rewind::RecordStep(Step undo, Step redo, void* data)
{
    if (currentEntry) {
        currentEntry->steps.push_back(BufferStep { data, undo, redo });
    }
}

void Rewind::AddDeallocResourceArray(void *array)
{
    if (currentEntry) {
        currentEntry->arrayDeallocs.push_back(array);
    }
}

void Rewind::AddDeallocResource(void *array)
{
    if (currentEntry) {
        currentEntry->ptrDeallocs.push_back(array);
    }
}

void Rewind::EndRewindEntry(bool exec)
{
    if (!inStep || !currentBuffer) {
        return;
    }

    auto start = currentBuffer->entries.begin() + currentBuffer->index;
    auto end = currentBuffer->entries.end();
    for (auto iter = start; iter < end; iter++) {
        for (auto dealloc : iter->arrayDeallocs) {
            delete[] dealloc;
        }
        for (auto dealloc : iter->ptrDeallocs) {
            delete dealloc;
        }
    }

    currentBuffer->entries.erase(start, end);
    currentBuffer->entries.push_back(*currentEntry);
    delete currentEntry;
    currentEntry = nullptr;
    
    if (exec) {
        Redo(1);
    }
}

bool Rewind::Redo(int count)
{
    for (int64_t i = 0; i < count; i++) {
        if (currentBuffer->index + i >= currentBuffer->entries.size()) {
            break;
        }
        for (const BufferStep& step : currentBuffer->entries[currentBuffer->index + i].steps) {
            if (!step.redo(step.data)) {
                return false;
            }
        }
    }

    if (currentBuffer->index + count <= currentBuffer->entries.size()) {
        currentBuffer->index += count;
	    return true;
    }

    return false;
}

bool Rewind::Undo(int count)
{
    for (int64_t i = 1; i <= count; i++) {
        if (currentBuffer->index - i < 0) {
            break;
        }
        for (const BufferStep& step : currentBuffer->entries[currentBuffer->index - i].steps) {
            if (!step.undo(step.data)) {
                return false;
            }
        }
    }

    if (currentBuffer->index - count >= 0) {
        currentBuffer->index -= count;
    	return true;
    }

    return false;
}
