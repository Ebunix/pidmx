//
// Created by ebu on 31.01.23.
//
#include "Panel.h"
#include "engine/ui/blackboard/Panel.h"
#include "PatchFixturesPanel.h"
#include "engine/core/Engine.h"

UI::Panel::Panel(std::string title, PanelType type, ImGuiWindowFlags flags): flags(flags), type(type) {
    name = std::move(title);
    id = INVALID_HASH;
}

void UI::Panel::Load(const nbt::tag_compound &comp) {
    open = nbt::Load(comp, "open", nullptr, false);

    // Handled by ImGui saving ini data to the show file instead
    // of manually restoring. Works better with any sort of docked
    // layout than manually trying to figure this out..

    //ImRect location = nbt::Load(comp, "location", ImRect(ImVec2(100, 100) * Engine::Instance().dpiScale, ImVec2(600, 400) * Engine::Instance().dpiScale));
    //position = location.Min;
    //size = location.Max - location.Min;
    //wasLoaded = true;
}

nbt::tag_compound UI::Panel::Save() {
    auto comp = ISerializable::Save();
    nbt::Save(comp, "open", open);
    //nbt::Save(comp, "location", ImRect(position, position + size));
    return comp;
}

UI::PanelInstance UI::CreatePanel(int type) {
    switch (type) {
        case PanelType_Blackboard: return std::make_shared<Blackboard::Panel>();
        case PanelType_FixturePatch: return std::make_shared<PatchFixturesPanel>();
        case PanelType_None:
        default:
            LogMessage(LogMessageType_Error, "Unknown type %i in %s", type, __FUNCTION__);
            return {};
    }
}
