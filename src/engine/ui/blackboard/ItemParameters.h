#pragma once

#include "Item.h"
#include "engine/core/FixturePreset.h"

namespace Blackboard {

    class ItemParameters: public Item {
    public:
        ItemParameters(): Item("Parameters", ItemType_Parameters) {}

        void Draw(ImDrawList *list, ImVec2 screenTL, ImVec2 screenBR, int itemIndex) override;

    private:
        bool DrawParameter(FixtureParameterType type, float* input);

        Map<FixtureParameterType, float> storedValues;

    };

}