#pragma once

#include "Panel.h"

namespace UI {
    class PanelConsole: public Panel {
    public:
        PanelConsole() : Panel("Console", PanelType_Console) {}
        void Draw() override;
    };
}