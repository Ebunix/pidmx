//
// Created by ebu on 08.02.23.
//

#include "ItemPresets.h"
#include "engine/core/Engine.h"
#include "engine/ui/ImGuiExt.h"

namespace Blackboard {
    const char* PresetDataTargetNames[] = {
            "Selection",
            "Assigned fixtures"
    };

    void ItemPresets::OnClick(int itemId) {
        Engine &engine = Engine::Instance();
        ShowData &show = engine.Show();
        switch (engine.action) {
            case EngineAction_Store: {
                const auto &selection = show.panelPatchFixtures->GetSelection();
                PresetData store;
                Hash id = store.id;
                store.name = "New preset";

                for (const auto &val : engine.programmer.GetTracking()) {
                    CallablePreset callable;
                    callable.fixtureId = val.first;
                    const auto &fix = show.fixtures.at(val.first);
                    for (const auto &param : val.second) {
                        float value;
                        if (fix->GetParamValue(param, &value, true)) {
                            callable.values.insert_or_assign(param, value);
                        }
                    }
                    store.callables.insert(std::move(callable));
                }

                Hash assigned = GetAt(itemId);
                if (assigned != INVALID_HASH) {
                    show.presets.erase(assigned);
                }

                show.presets.insert_or_assign(id, std::move(store));
                AssignAt(itemId, id);
                engine.SetAction(EngineAction_None);
                BeginEditing(itemId);
                break;
            }
            case EngineAction_Delete: {
                Hash assigned = GetAt(itemId);
                AssignAt(itemId, INVALID_HASH);
                if (assigned != INVALID_HASH) {
                    show.presets.erase(assigned);
                }
                engine.SetAction(EngineAction_None);
                break;
            }
            case EngineAction_Edit: {
                BeginEditing(itemId);
                engine.SetAction(EngineAction_None);
                break;
            }
            default: {
                if (!HasItemAt(itemId)) {
                    break;
                }

                Hash assigned = GetAt(itemId);
                if (assigned == INVALID_HASH) {
                    break;
                }

                engine.ActivatePreset(assigned);
                break;
            }
        }
    }

    void ItemPresets::Draw(ImDrawList *list, ImVec2 tl, ImVec2 br, int itemIndex) {
        Engine &engine = Engine::Instance();
        ShowData &show = engine.Show();

        static ImVec2 size = br - tl;
        static ImVec2 countTextPos = (ItemInnerPadding + ImVec2(0, 14)) * engine.dpiScale;
        static ImVec2 modeTextPos = ImVec2(size.x / 2, ItemInnerPadding.y * engine.dpiScale);
        ItemCollection::Draw(list, tl, br, itemIndex);

        Hash presetId = GetAt(itemIndex);
        if (presetId == INVALID_HASH) {
            return;
        }

        const auto &item = show.presets.at(GetAt(itemIndex));

        UI::DrawCollectionActiveButton(tl, br, item.name.c_str(), engine.IsPresetActive(presetId) ? UI::ColorGroupIndicatorGreen : UI::ColorGroupIndicatorInactive);

        ImVec2 cursorPos = tl - ImGui::GetWindowPos();
        ImGui::PushFont(ImGui::fontPixel);
        ImGui::SetCursorPos(cursorPos + countTextPos);
        ImGui::Text("%lu", item.callables.size());
        ImGui::SetCursorPos(cursorPos + modeTextPos);
        ImGui::Text(item.target == PresetDataTarget_Selection ? "S" : "A");
        ImGui::PopFont();
    }

    void ItemPresets::DrawEdit(bool justOpened) {
        Engine &engine = Engine::Instance();
        ShowData &show = engine.Show();
        auto &item = show.presets.at(GetAt(editTargetIndex));

        static PresetDataTarget selectedTarget;
        static char editNameBuffer[100];

        if (justOpened) {
            selectedTarget = item.target;
            strncpy(editNameBuffer, item.name.c_str(), sizeof(editNameBuffer));
        }

        ImGui::InputText("Name", editNameBuffer, sizeof(editNameBuffer));
        if (ImGui::BeginCombo("Target", PresetDataTargetNames[selectedTarget])) {
            for (int i = 0; i < PresetDataTarget_Count_; i++) {
                if (ImGui::Selectable(PresetDataTargetNames[i])) {
                    selectedTarget = (PresetDataTarget)i;
                }
            }
            ImGui::EndCombo();
        }
        DialogOption result = UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK);
        if (result != DialogOption_None) {
            bool apply = result == DialogOption_OK;
            if (apply) {
                item.target = selectedTarget;
                item.name = editNameBuffer;
            }
            FinishEditing();
        }
    }
} // Blackboard