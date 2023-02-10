#pragma once
#include "../command/CommandBuffer.h"
#include "Fixture.h"
#include "FixturePreset.h"
#include "FixtureCollection.h"
#include "../Console.h"
#include "engine/ui/blackboard/Item.h"
#include "engine/ui/Panel.h"
#include "engine/ui/PanelPatchFixtures.h"
#include "engine/ui/PanelFixturePresetEditor.h"
#include "engine/ui/PanelConsole.h"
#include "engine/ui/PanelOperators.h"
#include "engine/ui/blackboard/ItemGroups.h"
#include "Collections.h"
#include "engine/ui/blackboard/ItemPresets.h"
#include <pidmx_utils.h>
#include <map>
#include <type_traits>
#include <algorithm>

class ShowData {
public:
	CommandBuffer commandHistory;

    // Show data structure
	Map<Hash, FixtureInstance> fixtures;
	Map<Hash, FixturePresetInstance> fixturePresets;
	Map<Hash, FixtureCollectionInstance> fixtureCollections;
    Map<Hash, Blackboard::GroupData> groups;
    Map<Hash, Blackboard::PresetData> presets;

    // UI elements
    Map<Hash, Blackboard::ItemInstance> blackboardItems;

    std::shared_ptr<Blackboard::Panel> panelBlackboard;
    std::shared_ptr<UI::PanelConsole> panelConsole;
    std::shared_ptr<UI::PanelFixturePresetEditor> panelFixturePresetEditor;
    std::shared_ptr<UI::PanelPatchFixtures> panelPatchFixtures;
    std::shared_ptr<UI::PanelOperators> panelOperators;

    void Register();

	void Save(const std::string& path);
	void Load(const std::string& path);

    void RenderPanels();

    void RenderWindowMenu();

    Hash GetID() {
        Hash id = RandomHash();
        int iter = 0;
        if (globalIds.size() > 0) {
            while (globalIds.contains(id)) {
                id = RandomHash();
                if (iter++ > 20) {
                    LogMessage(LogMessageType_Error, "Failed to find a free ID after 20 attempts, random function broken?");
                    break;
                }
            }
        }
        globalIds.insert(id);
        return id;
    }

private:
    IDSet globalIds;
    std::vector<std::shared_ptr<UI::Panel>> registeredPanels;
    template<typename T>
    inline std::shared_ptr<T> RegisterUIPanel(std::shared_ptr<T> panel) {
        registeredPanels.push_back(panel);
        return panel;
    }
};
