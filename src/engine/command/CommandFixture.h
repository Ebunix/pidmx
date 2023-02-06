#pragma once

#include "ICommand.h"
#include "engine/core/Show.h"
#include <engine/core/Fixture.h>
#include <set>
#include <utility>

class CommandFixtureAdd : public ICommand {
public:
    static std::vector<CommandInstance> New(const std::vector<FixtureData> &data) {
        std::vector<CommandInstance> commands;
        for (const auto& d : data) {
            commands.push_back(std::make_shared<CommandFixtureAdd>(d));
        }
        return commands;
    }

    explicit CommandFixtureAdd(FixtureData data) : data(std::move(data)) {}

    void execute() override { redo(); }

    void undo() override {} // { currentShow->fixtures.Remove(data.fixtureId); }

    void redo() override {} // { currentShow->fixtures.Add(Fixture::New(data)); }

private:
    FixtureData data;
};


class CommandFixtureRemove : public ICommand {
public:
    static std::vector<CommandInstance> New(const std::set<Hash> &ids) {
        std::vector<CommandInstance> commands;
        for (const auto& d : ids) {
            commands.push_back(std::make_shared<CommandFixtureRemove>(d));
        }
        return commands;
    }

    explicit CommandFixtureRemove(Hash id) : id(id) {}

    void execute() override { redo(); }

    void undo() override {} // { currentShow->fixtures.Add(Fixture::New(data)); }

    void redo() override {
        /*
        FixtureInstance removed = currentShow->fixtures.Remove(id);
        data.fixtureId = removed->id;
        data.name = removed->name;
        data.universe = removed->universe;
        data.channel = removed->channel;
        data.presetId = removed->presetId;
         */
    }

private:
    Hash id;
    FixtureData data;
};
