//
// Created by ebu on 04.02.23.
//

#include "ItemGroups.h"
#include "engine/core/Engine.h"
#include "engine/core/Show.h"
#include "../ImGuiExt.h"

void Blackboard::ItemGroups::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
    ItemCollection::Draw(list, tl, br, itemIndex + 1);

    int groupIndex = itemIndex + 1;
    if (!HasItemAt(groupIndex)) {
        return;
    }

    Engine& engine = Engine::Instance();
    const auto &item = currentShow->groups.at(GetAt(groupIndex));

    static ImVec2 fixtureCountTextPos = (ItemInnerPadding + ImVec2(0, 14)) * Engine::Instance().dpiScale;

    ImVec2 size = br - tl;
    float halfHeight = size.y / 2;

    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImVec2 cursorLeftHalf = cursorPos + ImVec2(0, halfHeight);

    ImVec2 rightHalf(br.x, tl.y + halfHeight);
    ImVec2 leftHalf(tl.x, tl.y + halfHeight);

    bool highlight = engine.activeGroups.find(item->id) != engine.activeGroups.end();
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

void Blackboard::ItemGroups::OnClick(int itemIdOneBased) {
    Engine& engine = Engine::Instance();
    switch (engine.action) {
        case EngineAction_Store: {
            const auto &selection = currentShow->panelPatchFixtures->GetSelection();
            GroupData store;
            store.id = (Hash)currentShow->groups.size() + 1;
            store.name = "New group";
            store.fixtures = selection;
            currentShow->groups.insert_or_assign(store.id, std::make_shared<GroupData>(store));
            AssignAt(itemIdOneBased, store.id);
            engine.SetAction(EngineAction_None);
            break;
        }
        case EngineAction_Delete: {
            AssignAt(itemIdOneBased, INVALID_HASH);
            engine.SetAction(EngineAction_None);
            break;
        }
        default:
            if (HasItemAt(itemIdOneBased)) {
                engine.AddActiveGroup(GetAt(itemIdOneBased));
            }
            break;
    }
}


