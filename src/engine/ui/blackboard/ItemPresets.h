#pragma once

#include "ItemCollection.h"
#include "engine/core/CallablePreset.h"

namespace Blackboard {
    enum PresetDataTarget {
        PresetDataTarget_Selection,
        PresetDataTarget_AssignedFixtures,
        PresetDataTarget_Count_
    };
    extern const char* PresetDataTargetNames[];
    struct PresetData: IIdentifiable {
        std::string name;
        Set<CallablePreset> callables;
        PresetDataTarget target = PresetDataTarget_AssignedFixtures;

        nbt::tag_compound Save() const override {
            auto c = IIdentifiable::Save();
            c.insert("name", nbt::Serialize(name));
            c.insert("callables", nbt::Serialize(callables.begin(), callables.end()));
            c.insert("target", nbt::Serialize(target));
            return c;
        }

        void Load(const nbt::tag_compound &data) override {
            IIdentifiable::Load(data);
            name = nbt::Deserialize(data, "name", std::string(""));
            callables = nbt::Deserialize(data, "callables", Set<CallablePreset>());
            target = (PresetDataTarget)nbt::Deserialize(data, "target", PresetDataTarget_AssignedFixtures);
        }
    };

    class ItemPresets: public ItemCollection {
    public:
        ItemPresets(): ItemCollection() {
            name = "Presets";
            type = ItemType_Presets;
        }

        void Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) override;

        void OnClick(int itemId) override;
        void DrawEdit(bool justOpened) override;

    private:
    };

} // Blackboard
