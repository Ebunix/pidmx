#pragma once
#include "Tools.h"
#include "Panel.h"
#include "engine/core/Fixture.h"
#include <pidmx_utils.h>
#include <set>
#include <vector>

namespace UI {
    class PatchFixturesPanel : public Panel {
    public:
        PatchFixturesPanel() : Panel("Patch setup", PanelType_FixturePatch, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar) {}
        void Draw() override;
        void OnShow() override;

        inline const IDSet& GetSelection() { return selectedFixtureIds; }
    private:
        IDSet selectedFixtureIds;
        IDSet selectedCollectionIds;

        Map<Hash, FixtureInstance> filteredFixtures;
        Hash lastFixtureSelection = INVALID_HASH;
        Hash lastCollectionSelection = INVALID_HASH;

        static void DrawAddFixtureModal(bool openAddFixturesModal);
        void DrawAddFixturesToCollectionModal(bool open);
        void DrawCollectionTable();
        void DrawFixtureTable();
        void FilterForCurrentCollection();
    };
}