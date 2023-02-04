//
// Created by ebu on 30.01.23.
//

#include "Panel.h"
#include "engine/core/Show.h"
#include "engine/ui/ImGuiExt.h"
#include "engine/command/CommandBlackboard.h"
#include "engine/ui/Tools.h"
#include "engine/core/Engine.h"

#define PLACEMENT_HASH(x, y) (((int64_t)x << 32) | (int64_t)y)
using namespace UI;

void Blackboard::Panel::Draw() {
    if (BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            ImGui::Checkbox("Exact fit", &exactFit);
            ImGui::EndMenu();
        }
        EndMenuBar();
    }

    static char idBuffer[16];

    ImGui::PushID((int) id);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 startPos = ImGui::GetCursorStartPos();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();

    float elementSpacing = cellSize * globalEngine->dpiScale;
    int elementsHorizontal = (int) (avail.x / elementSpacing);
    float elementWidth = !exactFit ? elementSpacing : avail.x / elementsHorizontal;
    int elementsVertical = (int) (avail.y / elementSpacing);
    float elementHeight = !exactFit ? elementSpacing : avail.y / elementsVertical;

    ImVec2 topLeft(startPos.x, startPos.y);

    ImVec2 clipStart(startPos.x + windowPos.x, startPos.y + windowPos.y);
    ImVec2 clipEnd(clipStart.x + avail.x, clipStart.y + avail.y);
    //dl->PushClipRect(clipStart, clipEnd);
    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();

    ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_FrameBg]);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_FrameBgHovered]);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_FrameBgActive]);

    int targetX = -1;
    int targetY = -1;
    int targetWidth = -1;
    int targetHeight = -1;
    bool isIntersecting = false;
    bool isEditing = currentPlacingSelectSize || editingItem != nullptr;

    ImDrawList* draw = ImGui::GetWindowDrawList();

    if (isEditing) {
        if (ImGui::IsKeyDown(ImGuiKey_Escape)) {
            currentPlacingSelectSize = false;
            currentPlacingItem = ItemType_None;
            if (editingItem) {
                OccupyInstanceArea(editingItem);
            }
            EditItem(nullptr, BlackboardItemEditType_None);
            isEditing = false;
        }
        for (int y = 0; y < elementsVertical; y++) {
            for (int x = 0; x < elementsHorizontal; x++) {
                if (IsOccupied(x, y)) {
                    continue;
                }

                float xOff = x * elementWidth;
                float yOff = y * elementHeight;
                ImVec2 tl = ImVec2(xOff, yOff) + windowPos + topLeft;
                ImVec2 br = tl + ImVec2(elementWidth, elementHeight);

                if (ImGui::IsMouseHoveringRect(tl, br)) {
                    switch (editingType) {
                        case BlackboardItemEditType_Move:
                            targetX = x;
                            targetY = y;
                            targetWidth = editingItem->width - 1;
                            targetHeight = editingItem->height - 1;
                            break;
                        case BlackboardItemEditType_None:
                        default:
                            targetX = fmin(currentPlacingItemX, x);
                            targetY = fmin(currentPlacingItemY, y);
                            targetWidth = fmax(x, currentPlacingItemX) - targetX;
                            targetHeight = fmax(y, currentPlacingItemY) - targetY;
                            break;
                    }
                }
            }
        }

        for (int y = targetY; y <= targetY + targetHeight; y++) {
            for (int x = targetX; x <= targetX + targetWidth; x++) {
                if (IsOccupied(x, y)) {
                    isIntersecting = true;
                    goto foundIntersect;
                }
            }
        }
    }
    foundIntersect:

    int id = 0;
    for (int y = 0; y <= elementsVertical; y++) {
        for (int x = 0; x <= elementsHorizontal; x++) {

            float xOff = x * elementWidth;
            float yOff = y * elementHeight;
            ImVec2 tl = ImVec2(xOff, yOff) + windowPos + topLeft;
            ImVec2 br = tl + ImVec2(elementWidth, elementHeight);

            if (x == 0 || y == 0) {
                draw->AddCircleFilled(tl, 4 * globalEngine->dpiScale, ImColor(0x0bffffff), 8);
                draw->AddCircleFilled(tl, 2 * globalEngine->dpiScale, ImColor(0x10ffffff), 8);
            }
            draw->AddCircleFilled(br, 4 * globalEngine->dpiScale, ImColor(0x0bffffff), 8);
            draw->AddCircleFilled(br, 2 * globalEngine->dpiScale, ImColor(0x10ffffff), 8);

            if (IsOccupied(x, y) || x >= elementsHorizontal || y >= elementsVertical) {
                continue;
            }

            ImGui::SetCursorPos(tl + ItemOuterPadding - windowPos);
            ImGui::PushID(id++);
            snprintf(idBuffer, sizeof(idBuffer), "%i:%i", x, y);

            if (isEditing) {
                if (IsOccupied(x, y)) {
                    continue;
                }

                bool insideArea = targetX >= 0 &&
                                  x >= targetX && y >= targetY &&
                                  x <= targetX + targetWidth && y <= targetY + targetHeight;

                if (insideArea) {
                    if (isIntersecting) {
                        ImGui::PushStyleColor(ImGuiCol_Button, ColorPresets[ColorPresetType_ButtonRed].active);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ColorPresets[ColorPresetType_ButtonRed].hovered);
                    }
                    else {
                        ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
                    }
                }


                if (ImGui::Button("##v", br - tl - ItemOuterPadding * 2) && !isIntersecting) {
                    if (currentPlacingSelectSize) {
                        currentPlacingSelectSize = false;
                        currentShow->commandHistory.Push("Add blackboard item",
                                                         CommandBlackboardAddItem::New(currentPlacingItem, this, targetX, targetY, targetWidth + 1, targetHeight + 1));
                        currentPlacingItem = ItemType_None;
                    } else if (editingType == BlackboardItemEditType_Move) {
                        currentShow->commandHistory.Push("Move blackboard item",
                                                         CommandBlackboardMoveItem::New(editingItem->id, targetX, targetY));
                        EditItem(nullptr, BlackboardItemEditType_None);
                    }
                }

                if (insideArea) {
                    ImGui::PopStyleColor(2);
                }


            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, 0);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.07f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.07f));
                if (ImGui::Button("##v", br - tl - ItemOuterPadding * 2) && !isIntersecting) {
                    ImGui::OpenPopup(idBuffer);
                }
                ImGui::PopStyleColor(3);
            }

            if (ImGui::BeginPopup(idBuffer)) {
                ImGui::Text("Create panel");
                if (ImGui::Button("Collections")) { currentPlacingItem = ItemType_Collections; }
                if (ImGui::Button("Fixture Groups")) { currentPlacingItem = ItemType_Groups; }
                if (currentPlacingItem != ItemType_None) {
                    ImGui::CloseCurrentPopup();
                    currentPlacingItemX = x;
                    currentPlacingItemY = y;
                    currentPlacingSelectSize = true;
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
    }

    ImGui::PopStyleColor(3);

    for (const auto &item: currentShow->blackboardItems.items) {
        float xOff = item->x * elementWidth;
        float yOff = item->y * elementHeight;
        ImVec2 tlLocalSpace(topLeft.x + xOff, topLeft.y + yOff);
        ImVec2 tlScreenSpace(topLeft.x + xOff + windowPos.x, topLeft.y + yOff + windowPos.y);
        ImVec2 brScreenSpace(tlScreenSpace.x + elementWidth * item->width, tlScreenSpace.y + elementHeight * item->height);
        //dl->PushClipRect(tlScreenSpace, brScreenSpace, true);
        ImGui::SetCursorPos(tlLocalSpace);
        item->Render(dl, tlScreenSpace, brScreenSpace, elementWidth, elementHeight);
        //dl->PopClipRect();
    }
    //dl->PopClipRect();

    ImGui::PopID();

}

void Blackboard::Panel::PlaceInstance(const Blackboard::ItemInstance &instance, int x, int y, int width, int height, bool skipAddToShow) {
    instance->parent = currentShow->panelBlackboard;
    if (!skipAddToShow) {
        currentShow->blackboardItems.Add(instance);
    }
    instance->Move(x, y);
    instance->Resize(width, height);
}

void Blackboard::Panel::OccupyInstanceArea(Hash id) {
    OccupyInstanceArea(currentShow->blackboardItems.Get(id));
}

void Blackboard::Panel::OccupyInstanceArea(ItemInstance instance) {
    if (!instance) {
        return;
    }
    for (int64_t idX = instance->x; idX < instance->x + instance->width; idX++) {
        for (int64_t idY = instance->y; idY < instance->y + instance->height; idY++) {
            placedInstances.insert_or_assign(PLACEMENT_HASH(idX, idY), instance);
        }
    }
}

void Blackboard::Panel::FreeInstanceArea(Hash id) {
    const auto &instance = currentShow->blackboardItems.Get(id);
    if (!instance) {
        return;
    }
    for (int64_t idX = instance->x; idX < instance->x + instance->width; idX++) {
        for (int64_t idY = instance->y; idY < instance->y + instance->height; idY++) {
            placedInstances.erase(PLACEMENT_HASH(idX, idY));
        }
    }
}

bool Blackboard::Panel::IsOccupied(int64_t x, int64_t y) {
    return placedInstances.find(PLACEMENT_HASH(x, y)) != placedInstances.end();
}

void Blackboard::Panel::EditItem(const ItemInstance &item, ItemEditType editType) {
    editingItem = item;
    editingType = editType;
    if (editType == BlackboardItemEditType_Move) {
        FreeInstanceArea(item->id);
    }
}


