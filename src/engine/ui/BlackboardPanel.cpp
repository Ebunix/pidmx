//
// Created by ebu on 30.01.23.
//

#include "BlackboardPanel.h"
#include "engine/core/Show.h"
#include "ImGuiExt.h"
#include "engine/command/CommandBlackboard.h"
#include "Tools.h"

#define PLACEMENT_HASH(x, y) (((int64_t)x << 32) | (int64_t)y)

void UI::BlackboardPanel::Draw() {
    static char idBuffer[16];

    ImGui::PushID((int) id);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 startPos = ImGui::GetCursorStartPos();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();

    float spacing = 80 * DPI_SCALE;
    int elementsHorizontal = (int) (avail.x / spacing);
    float elementWidth = avail.x / elementsHorizontal;
    int elementsVertical = (int) (avail.y / spacing);
    float elementHeight = avail.y / elementsVertical;

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
    float inset = 3;
    bool isIntersecting = false;
    bool isEditing = currentPlacingSelectSize || editingItem != nullptr;

    if (isEditing) {
        if (ImGui::IsKeyDown(ImGuiKey_Escape)) {
            currentPlacingSelectSize = false;
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
                ImVec2 tl(topLeft.x + xOff + windowPos.x, topLeft.y + yOff + windowPos.y);
                ImVec2 br(tl.x + elementWidth, tl.y + elementHeight);

                ImVec2 tlInset = tl + (ImVec2(inset, inset) * DPI_SCALE);
                ImVec2 brInset = br - (ImVec2(inset, inset) * DPI_SCALE);

                if (ImGui::IsMouseHoveringRect(tlInset, brInset)) {
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
    for (int y = 0; y < elementsVertical; y++) {
        for (int x = 0; x < elementsHorizontal; x++) {
            if (IsOccupied(x, y)) {
                continue;
            }
            ImGui::PushID(id++);
            snprintf(idBuffer, sizeof(idBuffer), "%i:%i", x, y);

            float xOff = x * elementWidth;
            float yOff = y * elementHeight;
            ImVec2 tl(topLeft.x + xOff + windowPos.x, topLeft.y + yOff + windowPos.y);
            ImVec2 br(tl.x + elementWidth, tl.y + elementHeight);

            if (isEditing) {
                if (IsOccupied(x, y)) {
                    continue;
                }

                ImVec2 tlInset = tl + (ImVec2(inset, inset) * DPI_SCALE);
                ImVec2 brInset = br - (ImVec2(inset, inset) * DPI_SCALE);

                bool insideArea = targetX >= 0 &&
                                  x >= targetX && y >= targetY &&
                                  x <= targetX + targetWidth && y <= targetY + targetHeight;

                if (insideArea) {
                    if (isIntersecting) {
                        ImGui::PushStyleColor(ImGuiCol_Button, UI::BlackboardPanelItemColorOccupied);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, UI::BlackboardPanelItemColorOccupied);
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_Button, UI::BlackboardPanelItemColor);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, UI::BlackboardPanelItemColor);
                    }
                }

                if (UI::BlackboardPanelButton("##v", tlInset, brInset) && !isIntersecting) {
                    if (currentPlacingSelectSize) {
                        currentPlacingSelectSize = false;
                        currentShow->commandHistory.Push("Add blackboard item",
                                                         CommandBlackboardAddItem::New(currentPlacingItem, this, targetX, targetY, targetWidth + 1, targetHeight + 1));
                        currentPlacingItem = BlackboardItemType_None;
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
                if (UI::BlackboardPanelButton("##v", tl, br)) {
                    ImGui::OpenPopup(idBuffer);
                }
            }

            if (ImGui::BeginPopup(idBuffer)) {
                ImGui::Text("Create panel");
                if (ImGui::Button("Test panel")) { currentPlacingItem = BlackboardItemType_Test; }
                if (ImGui::Button("Collections")) { currentPlacingItem = BlackboardItemType_Collections; }
                if (currentPlacingItem != BlackboardItemType_None) {
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
        item->Render(dl, tlScreenSpace, brScreenSpace);
        //dl->PopClipRect();
    }
    //dl->PopClipRect();

    ImGui::PopID();
}

void UI::BlackboardPanel::PlaceInstance(const UI::BlackboardItemInstance &instance, int x, int y, int width, int height, bool skipAddToShow) {
    instance->parent = currentShow->panelBlackboard;
    instance->Move(x, y);
    instance->Resize(width, height);
    if (!skipAddToShow) {
        currentShow->blackboardItems.Add(instance);
    }
}

void UI::BlackboardPanel::OccupyInstanceArea(Hash id) {
    OccupyInstanceArea(currentShow->blackboardItems.Get(id));
}

void UI::BlackboardPanel::OccupyInstanceArea(BlackboardItemInstance instance) {
    if (!instance) {
        return;
    }
    for (int64_t idX = instance->x; idX < instance->x + instance->width; idX++) {
        for (int64_t idY = instance->y; idY < instance->y + instance->height; idY++) {
            placedInstances.insert_or_assign(PLACEMENT_HASH(idX, idY), instance);
        }
    }
}

void UI::BlackboardPanel::FreeInstanceArea(Hash id) {
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

bool UI::BlackboardPanel::IsOccupied(int64_t x, int64_t y) {
    return placedInstances.find(PLACEMENT_HASH(x, y)) != placedInstances.end();
}

void UI::BlackboardPanel::EditItem(const UI::BlackboardItemInstance &item, UI::BlackboardItemEditType editType) {
    editingItem = item;
    editingType = editType;
    if (editType == BlackboardItemEditType_Move) {
        FreeInstanceArea(item->id);
    }
}


