#pragma once

#include <imgui.h>
#include <string>
#include "engine/core/ISerializable.h"

namespace UI {
    class BlackboardPanel;

    const ImVec2 blackboardItemPadding(1.5f, 1.5f);

    enum BlackboardItemType {
        BlackboardItemType_None,
        BlackboardItemType_Test,
        BlackboardItemType_Collections,
    };

    class BlackboardItem : public ISerializable {
    public:
        int x = 0, y = 0;
        int width = 1, height = 1;
        BlackboardItemType type = BlackboardItemType_None;
        std::shared_ptr<BlackboardPanel> parent;

        BlackboardItem(std::string name, BlackboardItemType type);

        void load(const nbt::tag_compound& pack) override;
        nbt::tag_compound save() override;
        void afterLoad() override;

        void Render(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight);

        void Move(int x, int y);
        void Resize(int w, int h);

        virtual void Draw(ImDrawList *list, ImVec2 screenTL, ImVec2 screenBR, int itemIndex) {}
        virtual void OnResize(int newW, int newH) {}
        virtual void OnMove(int newX, int newY) {}
    };
    typedef std::shared_ptr<BlackboardItem> BlackboardItemInstance;

    class TestBlackboardItem : public BlackboardItem {
    public:
        explicit TestBlackboardItem() : BlackboardItem("Test item", BlackboardItemType_Test) {}

        void Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) override;
    };

    BlackboardItemInstance CreateBlackboardItem(int type);
}
