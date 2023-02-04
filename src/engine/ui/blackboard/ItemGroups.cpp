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

    static ImVec2 fixtureCountTextPos = (ItemInnerPadding + ImVec2(0, 14)) * globalEngine->dpiScale;

    const auto& item = collection[itemIndex];

    ImVec2 size = br - tl;
    float halfHeight = size.y / 2;

    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImVec2 cursorLeftHalf = cursorPos + ImVec2(0, halfHeight);

    ImVec2 rightHalf(br.x, tl.y + halfHeight);
    ImVec2 leftHalf(tl.x, tl.y + halfHeight);

    list->AddRectFilled(tl, rightHalf, 0xff000000, PanelButtonRounding * globalEngine->dpiScale);
    list->AddLine(leftHalf, rightHalf, 0xff888888, 4.0f * globalEngine->dpiScale);

    ImGui::SetCursorPos(cursorLeftHalf);
    UI::CenterTextWrap(item->name.c_str(), br.x - tl.x, halfHeight);

    ImGui::PushFont(ImGui::fontMonospaceSmall);
    ImGui::SetCursorPos(cursorPos + fixtureCountTextPos);
    ImGui::Text("%lu", item->fixtures.size());
    ImGui::PopFont();

    UI::OutlinedPanelBorder(list, 0xff666666, tl, br, 1.0f);
}

Blackboard::ItemGroups::ItemGroups(): ItemCollection<ItemGroupsStorage>("Fixture Groups", ItemType_Groups) {
    collection.push_back(std::make_shared<ItemGroupsStorage>(ItemGroupsStorage { "Fixture 1", std::vector<FixtureInstance>{ currentShow->fixtures.items[0], currentShow->fixtures.items[1], currentShow->fixtures.items[2] }}));
    AssignAt(7, std::make_shared<ItemGroupsStorage>(ItemGroupsStorage { "Fixture group 2", std::vector<FixtureInstance>{ currentShow->fixtures.items[0], currentShow->fixtures.items[1], currentShow->fixtures.items[2] }}));
}