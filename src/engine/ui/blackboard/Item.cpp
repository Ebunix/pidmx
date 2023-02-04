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

using namespace UI;

Blackboard::Item::Item(std::string name, ItemType type) : ISerializable(), type(type) {
    this->name = std::move(name);
    id = currentShow->blackboardItems.GetAvailableID();
}

void Blackboard::Item::Render(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, float cellWidth, float cellHeight) {
    ImGui::PushID((int) id);

    ImColor borderColor = ColorPresets[ColorPresetType_PanelItemMain].regular;
    ImColor frameBgColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    ImVec2 windowPos = ImGui::GetWindowPos();

    OutlinedPanel(list, 0, frameBgColor, topLeft, bottomRight);

    float totalWidth = cellWidth * width;
    float totalHeight = cellHeight * height;
    ImVec2 padding = ItemOuterPadding * globalEngine->dpiScale;
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
                ImGui::PushFont(ImGui::fontMonospaceSmall);
                ImGui::SetCursorPos(itemTLLocal + ItemInnerPadding * globalEngine->dpiScale);
                ImGui::PushStyleColor(ImGuiCol_Text, ColorTextTransparentLight);
                ImGui::Text("%d", index + 1);
                ImGui::PopStyleColor();
                ImGui::PopFont();
            }
            //list->PopClipRect();
        }
    }

    UI::OutlinedPanelBorder(list, borderColor, topLeft, bottomRight);

    if (ImGui::BeginPopup("Options")) {
        if (ImGui::MenuItem("Move")) {
            parent->EditItem(currentShow->blackboardItems.Get(id), BlackboardItemEditType_Move);
        }
        if (ImGui::MenuItem("Delete")) {
            currentShow->commandHistory.Push("Delete blackboard item", CommandBlackboardRemoveItem::New(id));
        }
        ImGui::EndPopup();
    }
    ImGui::PopID();

}

nbt::tag_compound Blackboard::Item::save() {
    nbt::tag_compound cmp = ISerializable::save();
    cmp.insert("pos", nbt::tag_compound{
            {"x", x},
            {"y", y},
            {"w", width},
            {"h", height}
    });
    cmp.insert("blackboard", parent->id);
    return cmp;
}

void Blackboard::Item::load(const nbt::tag_compound &pack) {
    ISerializable::load(pack);
    Hash blackboard = pack.at("blackboard").as<nbt::tag_long>().get();
    x = pack.at("pos").as<nbt::tag_compound>().at("x").as<nbt::tag_int>().get();
    y = pack.at("pos").as<nbt::tag_compound>().at("y").as<nbt::tag_int>().get();
    width = pack.at("pos").as<nbt::tag_compound>().at("w").as<nbt::tag_int>().get();
    height = pack.at("pos").as<nbt::tag_compound>().at("h").as<nbt::tag_int>().get();
    parent = currentShow->panelBlackboard;
}

void Blackboard::Item::afterLoad() {
    parent->PlaceInstance(currentShow->blackboardItems.Get(id), x, y, width, height, true);
}

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
            return std::make_shared<ItemCollection<ISerializable>>();
        case ItemType_Groups:
            return std::make_shared<ItemGroups>();
        case ItemType_None:
        default:
            LogMessage(LogMessageType_Error, "Unknown type %i in %s", type, __FUNCTION__);
            return {};
    }
}
