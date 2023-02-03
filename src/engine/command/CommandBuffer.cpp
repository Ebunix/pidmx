#include "CommandBuffer.h"

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

void CommandBuffer::Push(const std::string &name, CommandInstance command) {
    auto start = buffer.begin() + current;
    buffer.erase(start, buffer.end());
    internalBuffer.push_back({std::vector<CommandInstance>({std::move(command)}), name});
}

void CommandBuffer::Push(const std::string &name, const std::vector<CommandInstance> &commands) {
    auto start = buffer.begin() + current;
    buffer.erase(start, buffer.end());
    internalBuffer.push_back({commands, name});
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
