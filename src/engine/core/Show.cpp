#include "Show.h"
#include "ISerializable.h"
#include <fstream>
#include "engine/ui/blackboard/Panel.h"
#include "engine/ui/PatchFixturesPanel.h"
#include <js/js.h>
#include "engine/ui/blackboard/ItemSaveLoad.h"

const int32_t SHOW_VERSION = VERSION_CODE(0, 1, 0);
Show* currentShow = nullptr;

Show::Show() {
    panelBlackboard = RegisterUIPanel(std::make_shared<Blackboard::Panel>());
    panelPatchFixtures = RegisterUIPanel(std::make_shared<UI::PatchFixturesPanel>());
    panelFixturePresetEditor = RegisterUIPanel(std::make_shared<UI::PanelFixturePresetEditor>());
    panelConsole = RegisterUIPanel(std::make_shared<UI::PanelConsole>());
    panelOperators = RegisterUIPanel(std::make_shared<UI::PanelOperators>());
}

void Show::Save(const std::string &path)
{
	nbt::tag_compound rootCompound;
	rootCompound.insert("VERSION", SHOW_VERSION);

    nbt::Save(rootCompound, "fixtures", fixtures);
    nbt::Save(rootCompound, "fixtureCollections", fixtureCollections);
    nbt::Save(rootCompound, "groups", groups);
    nbt::Save(rootCompound, "blackboardItems", blackboardItems);
    nbt::tag_compound panelsRoot;
    for (const auto& panel : registeredPanels) {
        panelsRoot.insert(panel->name, panel->Save());
    }
    rootCompound.insert("panels", std::move(panelsRoot));
    size_t iniSize;
    void* iniData = (void*)ImGui::SaveIniSettingsToMemory(&iniSize);
    nbt::Save(rootCompound, "panelLayout", iniData, iniSize);

    // TODO: Figure out why cleaning up
    //  our mess here sometimes ends
    //  up crashing...
    //ImGui::MemFree(iniData);

    nbt::SaveToFile(rootCompound, path, true);
}

void Show::Load(const std::string &path)
{
    auto rootCompound = nbt::LoadFromFile(path);

    nbt::Load(*rootCompound, "fixtures", nullptr, fixtures);
    nbt::Load(*rootCompound, "fixtureCollections", nullptr, fixtureCollections);
    nbt::Load(*rootCompound, "groups", nullptr, groups);
    nbt::Load(*rootCompound, "blackboardItems", nullptr, blackboardItems);
    const nbt::tag_compound& panelsRoot = rootCompound->at("panels").as<nbt::tag_compound>();
    for (const auto& panel : registeredPanels) {
        if (panelsRoot.has_key(panel->name)) {
            const nbt::tag_compound& panelData = panelsRoot.at(panel->name).as<nbt::tag_compound>();
            panel->Load(panelData);
        }
    }
    size_t iniSize;
    void* iniData = nbt::Load(*rootCompound, "panelLayout", nullptr, &iniSize);
    ImGui::LoadIniSettingsFromMemory((const char*)iniData, iniSize);

    // TODO: as long as ImGui::MemFree()
    //  crashes, I'm not returning this
    //  chunk either >:[
    //free(iniData);
}

void Show::RenderPanels() {
    for (const auto& panel : registeredPanels) {
        panel->Render();
    }
}

void Show::RenderWindowMenu() {
    for (const auto& panel : registeredPanels) {
        ImGui::MenuItem(panel->name.c_str(), nullptr, &panel->open);
    }
}

