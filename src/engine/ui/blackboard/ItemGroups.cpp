//
// Created by ebu on 04.02.23.
//

#include "ItemGroups.h"
#include "engine/core/Engine.h"
#include "engine/core/Show.h"
#include "../ImGuiExt.h"

void Blackboard::ItemGroups::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
    ItemCollection::Draw(list, tl, br, itemIndex);
    if (!HasItemAt(itemIndex)) {
        return;
    }

    Engine& engine = Engine::Instance();

    static ImVec2 fixtureCountTextPos = (ItemInnerPadding + ImVec2(0, 14)) * Engine::Instance().dpiScale;

    const auto &item = currentShow->groups.Get(collection[itemIndex]);

    ImVec2 size = br - tl;
    float halfHeight = size.y / 2;

    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImVec2 cursorLeftHalf = cursorPos + ImVec2(0, halfHeight);

    ImVec2 rightHalf(br.x, tl.y + halfHeight);
    ImVec2 leftHalf(tl.x, tl.y + halfHeight);

    bool highlight = engine.activeGroups.find(itemIndex) != engine.activeGroups.end();
    bool solo = highlight && engine.activeGroups.size() == 1;
    ImU32 indicatorColor = solo ? ColorGroupIndicatorSolo : highlight ? ColorGroupIndicatorMulti : ColorGroupIndicatorInactive;

    list->AddRectFilled(tl, rightHalf, 0xff000000, PanelButtonRounding * Engine::Instance().dpiScale);
    list->AddLine(leftHalf, rightHalf, indicatorColor, 4.0f * Engine::Instance().dpiScale);

    ImGui::SetCursorPos(cursorLeftHalf);
    UI::CenterTextWrap(item->name.c_str(), br.x - tl.x, halfHeight);

    ImGui::PushFont(ImGui::fontPixel);
    ImGui::SetCursorPos(cursorPos + fixtureCountTextPos);
    ImGui::Text("%lu", item->fixtures.size());
    ImGui::PopFont();

    UI::OutlinedPanelBorder(list, 0xff666666, tl, br, 1.0f);
}

Blackboard::ItemGroups::ItemGroups() : Item("Fixture Groups", ItemType_Groups) {
}

void Blackboard::ItemGroups::OnClick(int itemIndex) {
    Engine& engine = Engine::Instance();
    switch (engine.action) {
        case EngineAction_Store: {
            const auto &selection = currentShow->panelPatchFixtures->GetSelection();
            GroupData store;
            store.id = (Hash)currentShow->groups.items.size() + 1;
            store.name = "New group";
            store.fixtures = selection;
            currentShow->groups.Add(std::make_shared<GroupData>(store));
            AssignAt(itemIndex + 1, store.id);
            engine.SetAction(EngineAction_None);
            break;
        }
        case EngineAction_Delete: {
            AssignAt(itemIndex + 1, INVALID_HASH);
            engine.SetAction(EngineAction_None);
            break;
        }
        default:
            if (HasItemAt(itemIndex)) {
                engine.activeGroups.insert(itemIndex);
            }
            break;
    }
}

nbt::tag_compound Blackboard::ItemGroups::Save() {
    return Item::Save();
}

void Blackboard::ItemGroups::Load(const nbt::tag_compound &comp) {
    Item::Load(comp);
}

