#include "Show.h"
#include <io/ozlibstream.h>
#include <io/izlibstream.h>
#include <nbt_tags.h>
#include <fstream>
#include "engine/ui/BlackboardPanel.h"
#include "engine/ui/PatchFixturesPanel.h"
#include "v8ObjectInterface.h"

const int32_t SHOW_VERSION = VERSION(0, 1, 0);
Show* currentShow = nullptr;

template<typename T>
void SerializeCollection(nbt::tag_compound& container, const std::string& tag, ShowCollection<T>& collection) {
    LogMessage(LogMessageType_Debug, "Serialize %s", tag.c_str());
    nbt::tag_list list;
    for (const auto& item : collection.items) {
        list.push_back(item->save());
    }
    container.insert(tag, std::move(list));
}
template<typename T>
void SerializeCollectionTyped(nbt::tag_compound& container, const std::string& tag, ShowCollection<T>& collection) {
    LogMessage(LogMessageType_Debug, "Serialize typed %s", tag.c_str());
    nbt::tag_list list;
    for (const auto& item : collection.items) {
        auto comp = item->save();
        comp.insert("TYPE", item->type);
        list.push_back(std::move(comp));
    }
    container.insert(tag, std::move(list));
}
template<typename T>
void DeserializeCollection(nbt::tag_compound& container, const std::string& tag, ShowCollection<std::shared_ptr<T>>& collection, std::shared_ptr<T> (*creator)()) {
    LogMessage(LogMessageType_Debug, "Deserialize %s", tag.c_str());
    nbt::tag_list list = container.at(tag).as<nbt::tag_list>();
    for (auto& item : list) {
        std::shared_ptr<T> instance = creator();
        instance->load(item.as<nbt::tag_compound>());
        collection.Add(instance);
    }
    for (auto& item : collection.items) {
        item->afterLoad();
    }
}
template<typename T>
void DeserializeCollectionTyped(nbt::tag_compound& container, const std::string& tag, ShowCollection<std::shared_ptr<T>>& collection, std::shared_ptr<T> (*creator)(int type)) {
    LogMessage(LogMessageType_Debug, "Deserialize typed %s", tag.c_str());
    nbt::tag_list list = container.at(tag).as<nbt::tag_list>();
    for (auto& item : list) {
        auto comp = item.as<nbt::tag_compound>();
        std::shared_ptr<T> instance = creator(comp.at("TYPE").as<nbt::tag_int>().get());
        instance->load(comp);
        collection.Add(instance);
    }
    for (auto& item : collection.items) {
        item->afterLoad();
    }
}


Show::Show() {
    panelBlackboard = std::make_shared<UI::BlackboardPanel>();
    panelPatchFixtures = std::make_shared<UI::PatchFixturesPanel>();
}

void Show::Save(const std::string &path)
{
    LogMessage(LogMessageType_Info, "Saving show file %s", path.c_str());
	std::ofstream out(path);
	zlib::ozlibstream ogzs(out, -1, true);
	nbt::tag_compound rootCompound;
	rootCompound.insert("VERSION", SHOW_VERSION);

    SerializeCollection(rootCompound, "fixtures", fixtures);
    SerializeCollection(rootCompound, "fixtureCollections", fixtureCollections);
    SerializeCollectionTyped(rootCompound, "blackboardItems", blackboardItems);
    rootCompound.insert("blackboard", panelBlackboard->save());

	nbt::io::write_tag("", rootCompound, ogzs);
	ogzs.close();
}

void Show::Load(const std::string &path)
{
    LogMessage(LogMessageType_Info, "Loading show file %s", path.c_str());
	std::ifstream in(path);
	zlib::izlibstream igzs(in);
	auto rootCompound = nbt::io::read_compound(igzs).second;

    DeserializeCollection(*rootCompound, "fixtures", fixtures, std::make_shared<Fixture>);
    DeserializeCollection(*rootCompound, "fixtureCollections", fixtureCollections, std::make_shared<FixtureCollection>);
    DeserializeCollectionTyped(*rootCompound, "blackboardItems", blackboardItems, UI::CreateBlackboardItem);
    panelBlackboard->load(rootCompound->at("blackboard").as<nbt::tag_compound>());
}

