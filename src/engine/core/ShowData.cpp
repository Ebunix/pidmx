#include "ShowData.h"
#include "NbtSerializer.h"
#include <fstream>
#include "engine/ui/blackboard/Panel.h"
#include "engine/ui/PanelPatchFixtures.h"

const int32_t SHOW_VERSION = VERSION_CODE(0, 1, 0);
ShowData *currentShow = nullptr;

void ShowData::Register() {
    panelBlackboard = RegisterUIPanel(std::make_shared<Blackboard::Panel>());
    panelPatchFixtures = RegisterUIPanel(std::make_shared<UI::PanelPatchFixtures>());
    panelFixturePresetEditor = RegisterUIPanel(std::make_shared<UI::PanelFixturePresetEditor>());
    panelConsole = RegisterUIPanel(std::make_shared<UI::PanelConsole>());
    panelOperators = RegisterUIPanel(std::make_shared<UI::PanelOperators>());
}

void ShowData::Save(const std::string &path) {
    nbt::tag_compound rootCompound;
    rootCompound.insert("VERSION", SHOW_VERSION);

    rootCompound.insert("importedFixturePresets", nbt::Serialize(fixturePresets.begin(), fixturePresets.end()));
    rootCompound.insert("fixtures", nbt::Serialize(fixtures.begin(), fixtures.end()));

    rootCompound.insert("groups", nbt::Serialize(groups.begin(), groups.end()));
    rootCompound.insert("presets", nbt::Serialize(presets.begin(), presets.end()));

    rootCompound.insert("fixtureCollections", nbt::Serialize(fixtureCollections.begin(), fixtureCollections.end()));
    rootCompound.insert("blackboardItems", nbt::Serialize(blackboardItems.begin(), blackboardItems.end()));
    nbt::tag_compound panelsRoot;
    for (const auto &panel: registeredPanels) {
        nbt::tag_compound p = panel->Save();
        panelsRoot.insert(panel->name, std::move(p));
    }
    rootCompound.insert("panels", std::move(panelsRoot));
    size_t iniSize;
    const signed char *iniData = (const signed char *) ImGui::SaveIniSettingsToMemory(&iniSize);
    rootCompound.insert("panelLayout", nbt::Serialize(iniData, iniSize));

    // TODO: Figure out why cleaning up
    //  our mess here sometimes ends
    //  up crashing...
    //ImGui::MemFree(iniData);

    nbt::SaveToFile(rootCompound, path, true);
}

void ShowData::Load(const std::string &path) {
    auto rootCompound = nbt::LoadFromFile(path);

    auto a = nbt::Deserialize(*rootCompound, "", FixtureInstance());

    fixturePresets = nbt::Deserialize(*rootCompound, "importedFixturePresets", Map<Hash, FixturePresetInstance>());
    fixtures = nbt::Deserialize(*rootCompound, "fixtures", Map<Hash, FixtureInstance>());

    groups = nbt::Deserialize(*rootCompound, "groups", Map<Hash, Blackboard::GroupData>());
    presets = nbt::Deserialize(*rootCompound, "presets", Map<Hash, Blackboard::PresetData>());

    fixtureCollections = nbt::Deserialize(*rootCompound, "fixtureCollections", Map<Hash, FixtureCollectionInstance>());
    blackboardItems = nbt::Deserialize(*rootCompound, "blackboardItems", Map<Hash, Blackboard::ItemInstance>());
    if (rootCompound->has_key("panels")) {
        const nbt::tag_compound &panelsRoot = rootCompound->at("panels").as<nbt::tag_compound>();
        for (const auto &panel: registeredPanels) {
            if (panelsRoot.has_key(panel->name)) {
                panel->Load(panelsRoot.at(panel->name).as<nbt::tag_compound>());
            }
        }
    }
    size_t iniSize;
    const signed char *iniData = nbt::Deserialize(*rootCompound, "panelLayout", nullptr, &iniSize);
    if (iniData) {
        ImGui::LoadIniSettingsFromMemory((const char *) iniData, iniSize);
        delete[] iniData;
    }

    for (const auto &panel: blackboardItems) {
        if (panel.second) {
            panel.second->AfterLoad();
        }
        else {
            LogMessage(LogMessageType_Warn, "Blackboard panel failed to load");
        }
    }
}

void ShowData::RenderPanels() {
    for (const auto &panel: registeredPanels) {
        if (panel) {
            panel->Render();
        }
    }
}

void ShowData::RenderWindowMenu() {
    for (const auto &panel: registeredPanels) {
        if (panel) {
            ImGui::MenuItem(panel->name.c_str(), nullptr, &panel->open);
        }
    }
}

