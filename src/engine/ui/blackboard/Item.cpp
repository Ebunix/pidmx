//
// Created by ebu on 31.01.23.
//

#include <cmath>
#include "Item.h"
#include "Panel.h"
#include "engine/core/Show.h"
#include "engine/core/Engine.h"
#include "engine/command/CommandBlackboard.h"
#include "ItemCollection.h"
#include "engine/ui/ImGuiExt.h"
#include "ItemGroups.h"
#include "ItemFixtureSheet.h"

using namespace UI;

Blackboard::Item::Item(std::string name, ItemType type) : type(type) {
    this->name = std::move(name);
    id = currentShow->blackboardItems.size() + 1;
}

void Blackboard::Item::Render(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight) {
    ImGui::PushID((int) id);

    ImColor frameBgColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];

    OutlinedPanel(list, 0, frameBgColor, topLeft, bottomRight);

    if (UseGridLayout()) {
        RenderGrid(list, topLeft, bottomRight, cellWidth, cellHeight);
    }
    else {
        RenderWindow(list, topLeft, bottomRight);
    }

    ImColor borderColor = ColorPresets[ColorPresetType_PanelItemMain].regular;
    UI::OutlinedPanelBorder(list, borderColor, topLeft, bottomRight);

    if (ImGui::BeginPopup("Options")) {
        if (ImGui::MenuItem("Move")) {
            parent->EditItem(currentShow->blackboardItems.at(id), BlackboardItemEditType_Move);
        }
        if (ImGui::MenuItem("Delete")) {
            currentShow->commandHistory.Push("Delete blackboard item", CommandBlackboardRemoveItem::New(id));
        }
        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void Blackboard::Item::RenderGrid(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight) {
    ImVec2 windowPos = ImGui::GetWindowPos();

    float totalWidth = cellWidth * width;
    float totalHeight = cellHeight * height;
    ImVec2 padding = ItemOuterPadding * Engine::Instance().dpiScale;
    ImVec2 itemSize((totalWidth - ((width + 1) * padding.x)) / width, (totalHeight - ((height + 1) * padding.y)) / height);

    for (int itemY = 0; itemY < height; itemY++) {
        for (int itemX = 0; itemX < width; itemX++) {
            ImVec2 itemTL = ImVec2(topLeft.x + itemSize.x * itemX + ((itemX + 1) * padding.x), topLeft.y + itemSize.y * itemY + ((itemY + 1) * padding.y));
            ImVec2 itemBR = itemTL + itemSize;
           
            ImVec2 itemTLLocal = itemTL - windowPos;
            ImGui::SetCursorPos(itemTLLocal);
            
            //list->PushClipRect(itemTL, itemBR);
            if (itemY == 0 && itemX == 0) {
                ItemInstance editing = parent->EditingItem();
                bool clicked = OutlinedButton(list, 0, ColorPresets[ColorPresetType_PanelItemMain], itemTL, itemBR);
                if (clicked) {
                    ImGui::OpenPopup("Options");
                }

                ImGui::PushFont(ImGui::fontRegularSmall);
                CenterTextWrap(name.c_str(), itemSize.x, itemSize.y);
                ImGui::PopFont();

            } else {
                int index = itemX + itemY * width - 1;
                ImGui::PushFont(ImGui::fontRegularSmall);
                Draw(list, itemTL, itemBR, index);
                ImGui::PopFont();
                ImGui::PushFont(ImGui::fontPixel);
                ImGui::SetCursorPos(itemTLLocal + ItemInnerPadding * Engine::Instance().dpiScale);
                ImGui::PushStyleColor(ImGuiCol_Text, ColorTextTransparentLight);
                ImGui::Text("%d", index + 1);
                ImGui::PopStyleColor();
                ImGui::PopFont();
            }
            //list->PopClipRect();
        }
    }
}

void Blackboard::Item::RenderWindow(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight) {
    ImVec2 windowPos = ImGui::GetWindowPos();
    static ImVec2 innerPadding = ItemInnerPadding * Engine::Instance().dpiScale;
    topLeft += innerPadding;
    bottomRight -= innerPadding;
    ImVec2 topLeftLocal = topLeft - windowPos;
    ImVec2 headerBottomRight(bottomRight.x, topLeft.y + 32.0f * Engine::Instance().dpiScale);
    ImVec2 headerSize = headerBottomRight - topLeft;

    if (OutlinedButton(list, 0, ColorPresets[ColorPresetType_PanelItemMain], topLeft, headerBottomRight)) {
        ImGui::OpenPopup("Options");
    }
    ImGui::SetCursorPos(topLeftLocal);
    CenterTextWrap(name.c_str(), headerSize.x, headerSize.y);
    topLeft.y += headerSize.y + innerPadding.y;
    ImVec2 contentSize = bottomRight - topLeft;
    topLeftLocal = topLeft - windowPos;
    ImGui::SetCursorPos(topLeftLocal);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    ImGui::BeginChildFrame(ImGui::GetID("Content"), contentSize);
    Draw(list, topLeft, bottomRight, 0);
    ImGui::EndChildFrame();

    ImGui::PopStyleVar();
}

//void Blackboard::Item::afterLoad() {
//    parent->PlaceInstance(currentShow->blackboardItems.at(id), x, y, width, height, true);
//}

void Blackboard::Item::Move(int newX, int newY) {
    parent->FreeInstanceArea(id);
    x = newX;
    y = newY;
    parent->OccupyInstanceArea(id);
    OnMove(x, y);
}

void Blackboard::Item::Resize(int newW, int newH) {
    parent->FreeInstanceArea(id);
    width = newW;
    height = newH;
    parent->OccupyInstanceArea(id);
    OnResize(width, height);
}



Blackboard::ItemInstance Blackboard::CreateItem(ItemType type) {
    switch (type) {
        case ItemType_Collections:
            return std::make_shared<ItemCollection>();
        case ItemType_Groups:
            return std::make_shared<ItemGroups>();
        case ItemType_FixtureSheet:
            return std::make_shared<ItemFixtureSheet>();
        case ItemType_None:
        default:
            LogMessage(LogMessageType_Error, "Unknown type %i in %s", type, __FUNCTION__);
            return {};
    }
}
