//
// Created by ebu on 04.02.23.
//

#include "ItemGroups.h"
#include "engine/core/Engine.h"
#include "engine/core/ShowData.h"
#include "../ImGuiExt.h"

nbt::tag_compound Blackboard::GroupData::Save() const {
    nbt::tag_compound c = IIdentifiable::Save();
    c.insert("name", nbt::Serialize(name));
    c.insert("fixtures", nbt::Serialize(fixtures.begin(), fixtures.end()));
    return c;
}

void Blackboard::GroupData::Load(const nbt::tag_compound &c) {
    IIdentifiable::Load(c);
    name = nbt::Deserialize(c, "name", std::string(""));
    fixtures = nbt::Deserialize(c, "fixtures", IDSet());
}


void Blackboard::ItemGroups::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
    static ImVec2 countTextPos = (ItemInnerPadding + ImVec2(0, 14)) * Engine::Instance().dpiScale;
    ItemCollection::Draw(list, tl, br, itemIndex);

    if (!HasItemAt(itemIndex)) {
        return;
    }

    Engine &engine = Engine::Instance();
    const auto &item = engine.Show().groups.at(GetAt(itemIndex));
    bool highlight = engine.activeGroups.find(item.id) != engine.activeGroups.end();
    bool solo = highlight && engine.activeGroups.size() == 1;

    ImU32 indicatorColor = solo ? UI::ColorGroupIndicatorGreen : highlight ? UI::ColorGroupIndicatorYellow : UI::ColorGroupIndicatorInactive;
    UI::DrawCollectionActiveButton(tl, br, item.name.c_str(), indicatorColor);

    ImGui::PushFont(ImGui::fontPixel);
    ImGui::SetCursorPos(ImGui::GetCursorPos() + countTextPos);
    ImGui::Text("%lu", item.fixtures.size());
    ImGui::PopFont();
}

Blackboard::ItemGroups::ItemGroups() : Item("Fixture Groups", ItemType_Groups) {
}

void Blackboard::ItemGroups::OnClick(int itemId) {
    Engine &engine = Engine::Instance();
    ShowData &show = engine.Show();
    switch (engine.action) {
        case EngineAction_Store: {
            const auto &selection = show.panelPatchFixtures->GetSelection();
            GroupData store;
            Hash id = store.id;
            store.name = "New group";
            store.fixtures = selection;

            Hash assigned = GetAt(itemId);
            if (assigned != INVALID_HASH) {
                show.groups.erase(assigned);
            }

            show.groups.insert_or_assign(id, std::move(store));
            AssignAt(itemId, id);
            engine.SetAction(EngineAction_None);
            BeginEditing(itemId);
            break;
        }
        case EngineAction_Delete: {
            Hash assigned = GetAt(itemId);
            AssignAt(itemId, INVALID_HASH);
            if (assigned != INVALID_HASH) {
                show.groups.erase(assigned);
            }
            engine.SetAction(EngineAction_None);
            break;
        }
        case EngineAction_Edit: {
            BeginEditing(itemId);
            engine.SetAction(EngineAction_None);
        }
        default: {
            if (HasItemAt(itemId)) {
                Hash groupId = GetAt(itemId);

                if (engine.activeGroups.contains(groupId)) {
                    const GroupData &data = show.groups.at(groupId);
                    for (const auto &fixId : data.fixtures) {
                        engine.programmer.TrackAllParameters(fixId);
                    }
                }
                else {
                    engine.AddActiveGroup(groupId);
                }
            }
            break;
        }
    }
}

void Blackboard::ItemGroups::DrawEdit(bool justOpened) {
    Engine &engine = Engine::Instance();
    ShowData &show = engine.Show();
    auto &item = show.groups.at(GetAt(editTargetIndex));

    static char editNameBuffer[100];

    if (justOpened) {
        strncpy(editNameBuffer, item.name.c_str(), sizeof(editNameBuffer));
    }

    ImGui::InputText("Name", editNameBuffer, sizeof(editNameBuffer));

    DialogOption result = UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK);
    if (result != DialogOption_None) {
        bool apply = result == DialogOption_OK;
        if (apply) {
            item.name = editNameBuffer;
        }
        FinishEditing();
    }
}

