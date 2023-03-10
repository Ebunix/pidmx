#pragma once
#include "../command/CommandBuffer.h"
#include "Fixture.h"
#include "FixturePreset.h"
#include "FixtureCollection.h"
#include "../Console.h"
#include "engine/ui/BlackboardItem.h"
#include "engine/ui/Panel.h"
#include "engine/ui/PatchFixturesPanel.h"
#include <pidmx_utils.h>
#include <map>
#include <type_traits>

template<class T>
struct ShowCollection {
	std::vector<T> items;

	bool Add(T instance);
	T Remove(Hash id);
	T Get(Hash id);
    Hash GetAvailableID();
};

template<class T>
Hash ShowCollection<T>::GetAvailableID() {
    Hash id = 1;
    while (Get(id)) {
        id++;
    }
    return id;
}

class Show {
public:
	CommandBuffer commandHistory;

    // Show data structure
	ShowCollection<FixtureInstance> fixtures;
	ShowCollection<FixturePresetInstance> fixturePresets;
	ShowCollection<FixtureCollectionInstance> fixtureCollections;

    // UI elements
    ShowCollection<UI::BlackboardItemInstance> blackboardItems;

    std::shared_ptr<UI::BlackboardPanel> panelBlackboard;
    std::shared_ptr<UI::PatchFixturesPanel> panelPatchFixtures;

    Show();

	void Save(const std::string& path);
	void Load(const std::string& path);
};

extern Show* currentShow;

template <class T>
bool CollectionSortFunc(const T &a, const T &b) { return a->id < b->id; }

template <class T>
inline bool ShowCollection<T>::Add(T instance)
{
    if (instance->id == INVALID_HASH) {
        LogMessage(LogMessageType_Error, "Failed to add instance to collection, ID was 0 (INVALID_HASH)");
        return false;
    }
    if (Get(instance->id)) {
		LogMessage(LogMessageType_Error, "Failed to add ID %lu to collection, already taken", instance->id);
		return false;
	}
	items.push_back(instance);
    std::sort(items.begin(), items.end(), CollectionSortFunc<T>);
	return true;
}

template <class T>
inline T ShowCollection<T>::Remove(Hash id)
{
    if (id == INVALID_HASH) {
        LogMessage(LogMessageType_Error, "Trying to remove item ID 0 from collection (INVALID_HASH)");
    }
	for (auto iter = items.begin(); iter != items.end(); iter++) {
        T result = *iter;
        if (result->id == id) {
            items.erase(iter);
            std::sort(items.begin(), items.end(), CollectionSortFunc<T>);
            return result;
        }
	}

	LogMessage(LogMessageType_Error, "Failed to remove ID %lu from collection, not found", id);
	return T();
}

template <class T>
inline T ShowCollection<T>::Get(Hash id)
{
    for (const auto& item : items) {
        if (item->id == id) {
            return item;
        }
    }
    return T();
}
