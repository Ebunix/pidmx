//
// Created by ebu on 31.01.23.
//

#include <cmath>
#include "BlackboardItem.h"
#include "BlackboardPanel.h"
#include "engine/core/Show.h"
#include "engine/command/CommandBlackboard.h"
#include "BlackboardItemCollection.h"
#include "ImGuiExt.h"


UI::BlackboardItem::BlackboardItem(std::string name, BlackboardItemType type) : ISerializable(), type(type) {
    this->name = std::move(name);
    id = currentShow->blackboardItems.GetAvailableID();
}

void UI::BlackboardItem::Render(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight) {
    float itemWidth = (bottomRight.x - topLeft.x) / width;
    float itemHeight = (bottomRight.y - topLeft.y) / height;

    if (std::isnan(itemWidth) || std::isnan(itemHeight)) {
        return;
    }
    ImGui::PushID((int) id);

    ImColor borderColor = ColorPresets[ColorPresetType_PanelItemMain].regular;
    ImColor frameBgColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    ImVec2 windowPos = ImGui::GetWindowPos();

    UI::OutlinedPanel(list, 0, frameBgColor, topLeft, bottomRight);

    for (int itemY = 0; itemY < height; itemY++) {
        for (int itemX = 0; itemX < width; itemX++) {
            ImVec2 itemTL = ImVec2(topLeft.x + itemWidth * itemX, topLeft.y + itemHeight * itemY);
            ImVec2 itemBR = ImVec2(itemTL.x + itemWidth, itemTL.y + itemHeight);
            //list->PushClipRect(itemTL, itemBR);
            if (itemY == 0 && itemX == 0) {
                BlackboardItemInstance editing = parent->EditingItem();
                bool clicked = UI::OutlinedButton(list, 0, ColorPresets[ColorPresetType_PanelItemMain], itemTL, itemBR);
                if (clicked) {
                    ImGui::OpenPopup("Options");
                }

                ImGui::PushFont(ImGui::fontSmallRegular);
                UI::CenterTextWrap(name.c_str(), itemWidth, itemHeight);
                ImGui::PopFont();

            } else {
                ImVec2 itemTLLocal = ImVec2(topLeft.x + itemWidth * itemX - windowPos.x, topLeft.y + itemHeight * itemY - windowPos.y);
                ImGui::SetCursorPos(itemTLLocal);
                Draw(list, itemTL, itemBR, itemX + itemY * width - 1);
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

nbt::tag_compound UI::BlackboardItem::save() {
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

void UI::BlackboardItem::load(const nbt::tag_compound &pack) {
    ISerializable::load(pack);
    Hash blackboard = pack.at("blackboard").as<nbt::tag_long>().get();
    x = pack.at("pos").as<nbt::tag_compound>().at("x").as<nbt::tag_int>().get();
    y = pack.at("pos").as<nbt::tag_compound>().at("y").as<nbt::tag_int>().get();
    width = pack.at("pos").as<nbt::tag_compound>().at("w").as<nbt::tag_int>().get();
    height = pack.at("pos").as<nbt::tag_compound>().at("h").as<nbt::tag_int>().get();
    parent = currentShow->panelBlackboard;
}

void UI::BlackboardItem::afterLoad() {
    parent->PlaceInstance(currentShow->blackboardItems.Get(id), x, y, width, height, true);
}

void UI::BlackboardItem::Move(int newX, int newY) {
    parent->FreeInstanceArea(id);
    x = newX;
    y = newY;
    parent->OccupyInstanceArea(id);
    OnMove(x, y);
}

void UI::BlackboardItem::Resize(int newW, int newH) {
    parent->FreeInstanceArea(id);
    width = newW;
    height = newH;
    parent->OccupyInstanceArea(id);
    OnResize(width, height);
}

void UI::TestBlackboardItem::Draw(ImDrawList *list, ImVec2 topLeft, ImVec2 bottomRight, int itemIndex) {
    static char format[10];
    snprintf(format, sizeof(format), "%i", itemIndex);
    UI::CenterTextWrap(format, bottomRight.x - topLeft.x);
}

UI::BlackboardItemInstance UI::CreateBlackboardItem(int type) {
    switch (type) {
        case BlackboardItemType_Test:
            return std::make_shared<UI::TestBlackboardItem>();
        case BlackboardItemType_Collections:
            return std::make_shared<UI::CollectionsBlackboardItem>();
        case BlackboardItemType_None:
        default:
            LogMessage(LogMessageType_Error, "Unknown type %i in %s", type, __FUNCTION__);
            return {};
    }
}
