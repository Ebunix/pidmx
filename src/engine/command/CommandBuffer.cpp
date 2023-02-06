#include "CommandBuffer.h"
#include "engine/Console.h"
#include <stdarg.h>
#include <utility>

void CommandBuffer::MoveTo(size_t index) {
    while (current < index && current < buffer.size()) {
        auto stack = buffer[current];
        for (auto cmd: stack.steps) {
            cmd->redo();
        }
        current++;
    }
    while (current > index && current > 0) {
        current--;
        auto stack = buffer[current];
        for (auto iter = stack.steps.end() - 1;; iter--) {
            (*iter)->undo();
            if (iter == stack.steps.begin()) {
                break;
            }
        }
    }
}

void CommandBuffer::Push(CommandInstance command) {
    if (!inEntry) {
        LogMessage(LogMessageType_Error, "Can't push to undo/redo stack outside of BeginEntry()/EndEntry()!");
        return;
    }
    buildingEntry.steps.push_back(command);
}

void CommandBuffer::Transfer() {
    for (const auto &entry: internalBuffer) {
        for (const auto &cmd: entry.steps) {
            cmd->execute();
        }
        buffer.push_back(entry);
        current++;
    }
    internalBuffer.clear();
}

void CommandBuffer::BeginEntry(const char *fmt, ...) {
    char* title;
    va_list va;
    va_start(va, fmt);
    vasprintf(&title, fmt, va);
    va_end(va);

    LogMessage(LogMessageType_Debug, "Push undo: %s", title);

    inEntry = true;
    buildingEntry.name = title;
    buildingEntry.steps.clear();
    auto start = buffer.begin() + current;
    buffer.erase(start, buffer.end());
}

void CommandBuffer::EndEntry() {
    inEntry = false;
    internalBuffer.push_back(buildingEntry);
}
