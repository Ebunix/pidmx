#pragma once
#include "Panel.h"

namespace UI {

    class PanelFixturePresetEditor: public Panel {
    public:
        PanelFixturePresetEditor(): Panel("Preset editor", PanelType_PresetEditor) {}

        void Draw() override;

    private:
        Hash selectedPreset = INVALID_HASH;
        void DrawPresetTable();
        void DrawPresetDetails();
    };

}