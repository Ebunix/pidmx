#pragma once

#include "BlackboardItem.h"
#include "engine/core/FixtureCollection.h"

namespace UI {

    class CollectionsBlackboardItem : public BlackboardItem {
    public:
        explicit CollectionsBlackboardItem() : BlackboardItem("Collections", BlackboardItemType_Collections) {}

        void Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) override;

        void OnResize(int w, int h) override {
            if (w * h > assignedCollections.size()) {
                assignedCollections.resize(w * h);
            }
        }

    private:
        std::vector<FixtureCollectionInstance> assignedCollections;
        int choosingCollectionIndex = -1;
        bool startChoosingCollectionIndex = false;

        void DrawButtonUnassigned(const ImVec2 &tl, const ImVec2 &br, int index);
    };
}