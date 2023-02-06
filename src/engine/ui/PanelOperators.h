#pragma once

#include "Panel.h"
#include "engine/core/EngineAction.h"

namespace UI {

    class PanelOperators: public Panel {
    public:
        PanelOperators(): Panel("Operators", PanelType_Operators) {}

        void Draw() override;

    private:
        bool ActionButton(const char* text, EngineAction action);
    };

}