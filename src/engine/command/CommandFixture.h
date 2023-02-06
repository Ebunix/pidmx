#pragma once

#include "ICommand.h"
#include "engine/core/Engine.h"
#include <engine/core/Fixture.h>
#include <set>
#include <utility>

class CommandFixtureAdd : public ICommand {
public:
    explicit CommandFixtureAdd(FixtureData data) : data(std::move(data)) {}

    void execute() override { redo(); }

    void undo() override { Engine::Instance().Show().fixtures.erase(id); }

    void redo() override {
        FixtureInstance fix = std::make_shared<Fixture>(data);
        id = fix->id;
        Engine::Instance().Show().fixtures.insert_or_assign(id, fix);
    }

private:
    Hash id;
    FixtureData data;
};


class CommandFixtureRemove : public ICommand {
public:
    explicit CommandFixtureRemove(Hash id) : id(id) {}

    void execute() override { redo(); }

    void undo() override {
        FixtureInstance fix = std::make_shared<Fixture>(data);
        fix->id = id;
        Engine::Instance().Show().fixtures.insert_or_assign(id, fix);
    }

    void redo() override {
        ShowData& show = Engine::Instance().Show();
        data = show.fixtures.at(id)->data;
        show.fixtures.erase(id);
    }

private:
    Hash id;
    FixtureData data;
};
