#pragma once

#include "ICommand.h"
#include "engine/core/Fixture.h"
#include "engine/ui/blackboard/Item.h"
#include "engine/ui/blackboard/Panel.h"
#include "engine/core/Engine.h"

class CommandBlackboardAddItem : public ICommand {
public:
    explicit CommandBlackboardAddItem(Blackboard::ItemType type, Blackboard::Panel *parent, int x, int y, int w, int h) :
            type(type), parent(parent), x(x), y(y), w(w), h(h) {}

    void execute() override { redo(); }

    void undo() override {
        parent->FreeInstanceArea(id);
        Engine::Instance().Show().blackboardItems.erase(id);
    }

    void redo() override {
        auto item = Blackboard::CreateItem(type);
        id = item->id;
        parent->PlaceInstance(item, x, y, w, h);
    }

private:
    Hash id = INVALID_HASH;
    int x, y, w, h;
    Blackboard::Panel *parent;
    Blackboard::ItemType type;
};

class CommandBlackboardRemoveItem : public ICommand {
public:
    explicit CommandBlackboardRemoveItem(Hash id) : id(id) {}

    void execute() override { redo(); }

    void undo() override {
        auto item = Blackboard::CreateItem(type);
        parent->PlaceInstance(item, item->x, item->y, item->width, item->height);
    }

    void redo() override {
        auto items = Engine::Instance().Show().blackboardItems;
        auto item = items.at(id);
        type = item->type;
        parent = item->parent;
        parent->FreeInstanceArea(id);
        items.erase(id);
    }

private:
    Hash id;
    nbt::tag_compound data;

    std::shared_ptr<Blackboard::Panel> parent;
    Blackboard::ItemType type = Blackboard::ItemType_None;
};

class CommandBlackboardMoveItem : public ICommand {
public:
    explicit CommandBlackboardMoveItem(Hash id, int x, int y) : id(id), x(x), y(y) {
        auto items = Engine::Instance().Show().blackboardItems;
        auto item = items.at(id);
        oldX = item->x;
        oldY = item->y;
    }

    void execute() override { redo(); }

    void undo() override {
        auto items = Engine::Instance().Show().blackboardItems;
        auto item = items.at(id);
        item->Move(oldX, oldY);
    }

    void redo() override {
        auto items = Engine::Instance().Show().blackboardItems;
        auto item = items.at(id);
        item->Move(x, y);
    }

private:
    Hash id;
    int x = 0, y = 0;
    int oldX = 0, oldY = 0;
};

