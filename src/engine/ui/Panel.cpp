//
// Created by ebu on 31.01.23.
//
#include "Panel.h"
#include "engine/ui/blackboard/Panel.h"
#include "PanelPatchFixtures.h"
#include "engine/core/Engine.h"

UI::Panel::Panel(std::string title, PanelType type, ImGuiWindowFlags flags): flags(flags), type(type) {
    name = std::move(title);
    id = INVALID_HASH;
}

UI::PanelInstance UI::CreatePanel(int type) {
    switch (type) {
        case PanelType_Blackboard: return std::make_shared<Blackboard::Panel>();
        case PanelType_FixturePatch: return std::make_shared<PanelPatchFixtures>();
        case PanelType_None:
        default:
            LogMessage(LogMessageType_Error, "Unknown type %i in %s", type, __FUNCTION__);
            return {};
    }
}
