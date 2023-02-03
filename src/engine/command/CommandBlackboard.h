#pragma once

#include "ICommand.h"
#include "engine/core/Fixture.h"
#include "engine/ui/BlackboardItem.h"
#include "engine/ui/BlackboardPanel.h"
#include "engine/core/Show.h"

class CommandBlackboardAddItem : public ICommand {
public:
    static CommandInstance New(UI::BlackboardItemType type, UI::BlackboardPanel *parent, int x, int y, int w, int h) {
        return std::make_shared<CommandBlackboardAddItem>(type, parent, x, y, w, h);
    }

    explicit CommandBlackboardAddItem(UI::BlackboardItemType type, UI::BlackboardPanel *parent, int x, int y, int w, int h) :
            type(type), parent(parent), x(x), y(y), w(w), h(h) {}

    void execute() override { redo(); }

    void undo() override {
        parent->FreeInstanceArea(id);
        currentShow->blackboardItems.Remove(id);
    }

    void redo() override {
        auto item = UI::CreateBlackboardItem(type);
        id = item->id;
        parent->PlaceInstance(item, x, y, w, h);
    }

private:
    Hash id = INVALID_HASH;
    int x, y, w, h;
    UI::BlackboardPanel *parent;
    UI::BlackboardItemType type;
};

class CommandBlackboardRemoveItem : public ICommand {
public:
    static CommandInstance New(Hash id) {
        return std::make_shared<CommandBlackboardRemoveItem>(id);
    }

    explicit CommandBlackboardRemoveItem(Hash id) : id(id) {}

    void execute() override { redo(); }

    void undo() override {
        auto item = UI::CreateBlackboardItem(type);
        item->load(data);
        parent->PlaceInstance(item, item->x, item->y, item->width, item->height);
    }

    void redo() override {
        auto item = currentShow->blackboardItems.Get(id);
        data = item->save();
        type = item->type;
        parent = item->parent;
        parent->FreeInstanceArea(id);
        currentShow->blackboardItems.Remove(id);
    }

private:
    Hash id;
    nbt::tag_compound data;

    std::shared_ptr<UI::BlackboardPanel> parent;
    UI::BlackboardItemType type = UI::BlackboardItemType_None;
};

class CommandBlackboardMoveItem : public ICommand {
public:
    static CommandInstance New(Hash id, int x, int y) {
        return std::make_shared<CommandBlackboardMoveItem>(id, x, y);
    }

    explicit CommandBlackboardMoveItem(Hash id, int x, int y) : id(id), x(x), y(y) {
        auto item = currentShow->blackboardItems.Get(id);
        oldX = item->x;
        oldY = item->y;
    }

    void execute() override { redo(); }

    void undo() override {
        auto item = currentShow->blackboardItems.Get(id);
        item->Move(oldX, oldY);
    }

    void redo() override {
        auto item = currentShow->blackboardItems.Get(id);
        item->Move(x, y);
    }

private:
    Hash id;
    int x = 0, y = 0;
    int oldX = 0, oldY = 0;
};

