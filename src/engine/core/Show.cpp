#include "Show.h"
#include "ISerializable.h"
#include <fstream>
#include "engine/ui/blackboard/Panel.h"
#include "engine/ui/PatchFixturesPanel.h"
#include <js/js.h>

const int32_t SHOW_VERSION = VERSION_CODE(0, 1, 0);
Show* currentShow = nullptr;

template<typename T>
void SerializeCollection(nbt::tag_compound& container, const std::string& tag, ShowCollection<T>& collection) {
    LogMessage(LogMessageType_Debug, "Serialize %s", tag.c_str());
    nbt::tag_list list;
    for (const auto& item : collection.items) {
        list.push_back(item->Save());
    }
    container.insert(tag, std::move(list));
}
template<typename T>
void SerializeCollectionRaw(nbt::tag_compound& container, const std::string& tag, ShowCollection<T>& collection) {
    LogMessage(LogMessageType_Debug, "Serialize raw %s", tag.c_str());
    nbt::tag_list list;
    for (const auto& item : collection.items) {
        nbt::tag_compound data;
        nbt::Save(data, "", item);
        list.push_back(std::move(data));
    }
    container.insert(tag, std::move(list));
}
template<typename T>
void SerializeCollectionTyped(nbt::tag_compound& container, const std::string& tag, ShowCollection<T>& collection) {
    LogMessage(LogMessageType_Debug, "Serialize typed %s", tag.c_str());
    nbt::tag_list list;
    for (const auto& item : collection.items) {
        auto comp = item->Save();
        comp.insert("TYPE", item->type);
        list.push_back(std::move(comp));
    }
    container.insert(tag, std::move(list));
}
template<typename T>
void DeserializeCollection(nbt::tag_compound& container, const std::string& tag, ShowCollection<std::shared_ptr<T>>& collection) {
    LogMessage(LogMessageType_Debug, "Deserialize %s", tag.c_str());
    nbt::tag_list list = container.at(tag).as<nbt::tag_list>();
    for (auto& item : list) {
        std::shared_ptr<T> instance = std::make_shared<T>();
        instance->Load(item.as<nbt::tag_compound>());
        collection.Add(instance);
    }
    for (auto& item : collection.items) {
        item->afterLoad();
    }
}
template<typename T>
void DeserializeCollectionRaw(nbt::tag_compound& container, const std::string& tag, ShowCollection<std::shared_ptr<T>>& collection) {
    LogMessage(LogMessageType_Debug, "Deserialize raw %s", tag.c_str());
    nbt::tag_list list = container.at(tag).as<nbt::tag_list>();
    for (auto& item : list) {
        auto comp = item.as<nbt::tag_compound>();
        std::shared_ptr<T> value = nbt::Load(comp, "", std::shared_ptr<T>());
        collection.Add(value);
    }
}
template<typename T, typename V>
void DeserializeCollectionTyped(nbt::tag_compound& container, const std::string& tag, ShowCollection<std::shared_ptr<T>>& collection, std::shared_ptr<T> (*creator)(V type)) {
    LogMessage(LogMessageType_Debug, "Deserialize typed %s", tag.c_str());
    nbt::tag_list list = container.at(tag).as<nbt::tag_list>();
    for (auto& item : list) {
        auto comp = item.as<nbt::tag_compound>();
        std::shared_ptr<T> instance = creator((V)comp.at("TYPE").as<nbt::tag_int>().get());
        instance->Load(comp);
        collection.Add(instance);
    }
    for (auto& item : collection.items) {
        item->afterLoad();
    }
}


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

    SerializeCollection(rootCompound, "fixtures", fixtures);
    SerializeCollection(rootCompound, "fixtureCollections", fixtureCollections);
    SerializeCollectionRaw(rootCompound, "groups", groups);
    SerializeCollectionTyped(rootCompound, "blackboardItems", blackboardItems);
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

    DeserializeCollection(*rootCompound, "fixtures", fixtures);
    DeserializeCollection(*rootCompound, "fixtureCollections", fixtureCollections);
    DeserializeCollectionRaw(*rootCompound, "groups", groups);
    DeserializeCollectionTyped(*rootCompound, "blackboardItems", blackboardItems, Blackboard::CreateItem);
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

