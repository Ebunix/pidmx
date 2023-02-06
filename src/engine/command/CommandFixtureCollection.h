#pragma once

#include "ICommand.h"
#include "engine/core/Show.h"
#include <engine/core/Fixture.h>
#include <utility>

class CommandFixtureCollectionAdd : public ICommand {
public:
    CommandFixtureCollectionAdd(Hash id, std::string name) : id(id), name(std::move(name)) {}

    static CommandInstance New(Hash id, const std::string &name) { return std::make_shared<CommandFixtureCollectionAdd>(id, name); }

    void execute() override { redo(); }

    void undo() override {} // { currentShow->fixtureCollections.Remove(id); }

    void redo() override {} // { currentShow->fixtureCollections.Add(FixtureCollection::New(id, name)); }

private:
    Hash id;
    std::string name;
};

class CommandFixtureCollectionRemove : public ICommand {
public:
    CommandFixtureCollectionRemove(Hash id, std::string name) : id(id), name(std::move(name)) {}

    static CommandInstance New(Hash id, const std::string &name) { return std::make_shared<CommandFixtureCollectionAdd>(id, name); }

    void execute() override { redo(); }

    void undo() override {} // { currentShow->fixtureCollections.Remove(id); }

    void redo() override {} // { currentShow->fixtureCollections.Add(FixtureCollection::New(id, name)); }

private:
    Hash id;
    std::string name;
};

class CommandFixtureCollectionAssignFixtures : public ICommand {
public:
    CommandFixtureCollectionAssignFixtures(Hash id, const std::set<Hash> &fixtures) : id(id), fixtureIds(fixtures) {}

    static CommandInstance New(Hash id, const std::set<Hash> &fixtures) { return std::make_shared<CommandFixtureCollectionAssignFixtures>(id, fixtures); }

    void execute() override { redo(); }

    void undo() override {
        /*
        auto collection = currentShow->fixtureCollections.Get(id);
        std::remove_if(collection->assignedFixtures.begin(), collection->assignedFixtures.end(), [&](Hash val) {
            return fixtureIds.find(val) != fixtureIds.end();
        });*/
    }

    void redo() override {
        /*
        auto collection = currentShow->fixtureCollections.Get(id);
        for (auto val : fixtureIds) {
            collection->assignedFixtures.push_back(val);
        }*/
    }

private:
    Hash id;
    std::set<Hash> fixtureIds;
};
