#pragma once

#include "ICommand.h"
#include "engine/core/Engine.h"
#include <engine/core/Fixture.h>
#include <utility>

class CommandFixtureCollectionAdd : public ICommand {
public:
    CommandFixtureCollectionAdd(Hash id, std::string name) : id(id), name(std::move(name)) {}

    void execute() override { redo(); }

    void undo() override { Engine::Instance().Show().fixtureCollections.erase(id); }

    void redo() override { Engine::Instance().Show().fixtureCollections.insert_or_assign(id, std::make_shared<FixtureCollection>(id, name)); }

private:
    Hash id;
    std::string name;
};

class CommandFixtureCollectionRemove : public ICommand {
public:
    CommandFixtureCollectionRemove(Hash id, std::string name) : id(id), name(std::move(name)) {}

    void execute() override { redo(); }

    void undo() override { Engine::Instance().Show().fixtureCollections.erase(id); }

    void redo() override { Engine::Instance().Show().fixtureCollections.insert_or_assign(id, std::make_shared<FixtureCollection>(id, name)); }

private:
    Hash id;
    std::string name;
};

class CommandFixtureCollectionAssignFixtures : public ICommand {
public:
    CommandFixtureCollectionAssignFixtures(Hash id, const IDSet &fixtures) : id(id), fixtureIds(fixtures) {}

    void execute() override { redo(); }

    void undo() override {
        auto collection = Engine::Instance().Show().fixtureCollections.at(id);
        for (const auto& val : fixtureIds) {
            collection->assignedFixtures.erase(val);
        }
    }

    void redo() override {
        auto collection = Engine::Instance().Show().fixtureCollections.at(id);
        for (auto val : fixtureIds) {
            collection->assignedFixtures.insert(val);
        }
    }

private:
    Hash id;
    IDSet fixtureIds;
};
