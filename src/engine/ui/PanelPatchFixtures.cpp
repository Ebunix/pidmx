#include "PanelPatchFixtures.h"
#include "ImGuiExt.h"
#include "engine/core/Utils.h"
#include <engine/command/CommandFixtureCollection.h>
#include <engine/command/CommandFixture.h>
#include <engine/core/ShowData.h>
#include <set>

void UI::PanelPatchFixtures::DrawAddFixtureModal(bool openAddFixturesModal) {
    static char fixtureNameBuffer[100];
    static int fixtureId;
    static int fixtureQuantity;
    static char fixturePatchBuffer[8];
    static FixturePresetInstance selectedPreset;

    if (openAddFixturesModal) {
        strcpy(fixturePatchBuffer, "1.001");
    }

    DialogOption result = DialogOption_None;
    int universe, channel;
    bool errors = false;
    bool openImportPresetModal = false;
    ImGuiStyle &style = ImGui::GetStyle();
    Engine &engine = Engine::Instance();
    ShowData &show = engine.Show();

    ImVec2 availableArea = ImGui::GetContentRegionAvail();
    ImVec2 halfSize(availableArea.x / 2 - style.FramePadding.x, availableArea.y - style.FramePadding.y - 35 * engine.dpiScale);

    if (ImGui::BeginChildFrame(ImGui::GetID("left"), halfSize, ImGuiWindowFlags_NoBackground)) {
        if (openAddFixturesModal) {
            fixtureNameBuffer[0] = 0;
        }

        if (ImGui::InputText("Fixture name", fixtureNameBuffer, sizeof(fixtureNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            result = DialogOption_OK;
        }
        ImGui::InputInt("Fixture ID", &fixtureId, 1, 1);
        ImGui::InputInt("Quantity", &fixtureQuantity, 1, 1);
        if (ImGui::InputText("Patch", fixturePatchBuffer, sizeof(fixturePatchBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            result = DialogOption_OK;
        }

        fixtureId = fmin(999999, fmax(1, fixtureId));
        fixtureQuantity = fmin(999999, fmax(1, fixtureQuantity));

        if (!FixturePatchStringToUniverseChannel(fixturePatchBuffer, sizeof(fixturePatchBuffer), &universe, &channel)) {
            ImGui::TextWrapped("Patch: <%s> is not a valid patch", fixturePatchBuffer);
            errors = true;
        }
        else {
            ImGui::TextWrapped("Target: %d.%03d is not a valid patch", universe, channel);
        }

    }
    ImGui::EndChildFrame();

    ImGui::SameLine();

    if (ImGui::BeginChildFrame(ImGui::GetID("right"), halfSize)) {
        ImGui::Text("Imported presets");
        if (ImGui::BeginTable("preset list", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable)) {
            ImGui::TableSetupColumn("Fixture");
            ImGui::TableSetupColumn("Manufacturer");
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            openImportPresetModal = ImGui::Selectable("Import fixture preset", false, ImGuiSelectableFlags_SpanAllColumns);

            for (const auto &preset: show.fixturePresets) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Selectable(preset.second->name.c_str(), selectedPreset && selectedPreset->id == preset.second->id, ImGuiSelectableFlags_SpanAllColumns)) {
                    selectedPreset = preset.second;
                    strncpy(fixtureNameBuffer, selectedPreset->name.c_str(), sizeof(fixtureNameBuffer));
                    strncat(fixtureNameBuffer, " 1", sizeof(fixtureNameBuffer) - strlen(fixtureNameBuffer) - 1);
                }
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", preset.second->manufacturer.c_str());
            }

            ImGui::EndTable();
        }
    }
    ImGui::EndChildFrame();

    ImVec2 halfVp = ImGui::GetMainViewport()->WorkSize / 2;
    ImGui::SetNextWindowSize(halfVp);
    ImGui::SetNextWindowPos(halfVp / 2, ImGuiCond_Appearing);
    if (UI::BeginPopupDialog(openImportPresetModal, "Import preset")) {
        static FixturePresetInstance importSelectedPreset;
        if (openImportPresetModal) {
            importSelectedPreset = nullptr;
        }

        ImVec2 contentArea = ImGui::GetContentRegionAvail();
        ImVec2 halfHeight(contentArea.x, (contentArea.y - style.FramePadding.y - 40 * engine.dpiScale) / 2);
        if (UI::BeginPresetList(halfHeight)) {
            for (const auto &preset: engine.fixturePresets) {
                if (!preset) {
                    continue;
                }
                if (UI::DrawPresetTableEntry(preset, importSelectedPreset && importSelectedPreset->id == preset->id)) {
                    importSelectedPreset = preset;
                }
            }
            UI::EndPresetList();
            if (ImGui::BeginChildFrame(ImGui::GetID("description"), halfHeight)) {

            }
            ImGui::EndChildFrame();
        }

        if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK, result, importSelectedPreset ? DialogOption_None : DialogOption_OK) == DialogOption_OK) {
            show.fixturePresets.insert_or_assign(importSelectedPreset->id, importSelectedPreset);
        }
    }

    if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK, result, errors ? DialogOption_OK : DialogOption_None) == DialogOption_OK && !errors) {
        show.commandHistory.BeginEntry("Patch %d fixtures", fixtureQuantity);
        for (int i = 0; i < fixtureQuantity; i++) {
            Hash presetId = selectedPreset ? selectedPreset->id : INVALID_HASH;
            int targetChannel = channel + (selectedPreset ? selectedPreset->footprint * i : 0);
            show.commandHistory.Push(std::make_shared<CommandFixtureAdd>(FixtureData(fixtureNameBuffer, fixtureId + i, presetId, universe, targetChannel)));
        }
        show.commandHistory.EndEntry();
    }


    /*
    static char fixtureNameBuffer[100];
    static char fixtureIdBuffer[6];
    static char fixtureQuantityBuffer[6];
    static char fixtureDmxChannelsBuffer[3];
    static char fixturePatchBuffer[8];
    if (openAddFixturesModal) {
        fixtureNameBuffer[0] = 0;
    }
    DialogOption result = DialogOption_None;

    if (ImGui::BeginCombo("##v", "Custom...")) {
        // TODO preset selection
        ImGui::EndCombo();
    }
    if (ImGui::InputText("Fixture name", fixtureNameBuffer, sizeof(fixtureNameBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        result = DialogOption_OK;
    }
    if (ImGui::InputText("Fixture ID", fixtureIdBuffer, sizeof(fixtureIdBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        result = DialogOption_OK;
    }
    if (ImGui::InputText("Quantity", fixtureQuantityBuffer, sizeof(fixtureQuantityBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        result = DialogOption_OK;
    }
    if (ImGui::InputText("DMX channels", fixtureDmxChannelsBuffer, sizeof(fixtureDmxChannelsBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        result = DialogOption_OK;
    }
    if (ImGui::InputText("Patch", fixturePatchBuffer, sizeof(fixturePatchBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        result = DialogOption_OK;
    }

    if (!VerifyTextNumeric(fixtureIdBuffer)) {
        ImGui::TextWrapped("Fixture ID: %s is not a valid number", fixtureIdBuffer);
    }
    if (!VerifyTextNumeric(fixtureQuantityBuffer)) {
        ImGui::TextWrapped("Quantity: %s is not a valid number", fixtureQuantityBuffer);
    }
    if (!VerifyTextNumeric(fixtureDmxChannelsBuffer)) {
        ImGui::TextWrapped("DMX Channels: %s is not a valid number", fixtureDmxChannelsBuffer);
    }
    int universe, channel;
    if (!FixturePatchStringToUniverseChannel(fixturePatchBuffer, sizeof(fixturePatchBuffer), &universe,
                                             &channel)) {
        ImGui::TextWrapped("Patch: %s is not a valid patch", fixturePatchBuffer);
    }

    if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK, result) == DialogOption_OK) {
        Hash id = std::stoi(fixtureIdBuffer);
        int quantity = std::stoi(fixtureQuantityBuffer);
        int footprint = std::stoi(fixtureDmxChannelsBuffer);

        ShowData& show = Engine::Instance().Show();

        show.commandHistory.BeginEntry("Patch %d fixtures", quantity);
        for (int i = 0; i < quantity; i++) {
            show.commandHistory.Push(std::make_shared<CommandFixtureAdd>(FixtureData(fixtureNameBuffer, id + i, 0, universe, channel + footprint * i)));
        }
        show.commandHistory.EndEntry();
    }
     */
}

void UI::PanelPatchFixtures::DrawAddFixturesToCollectionModal(bool open) {
    static Hash selectedCollection = INVALID_HASH;
    ShowData &show = Engine::Instance().Show();

    FixtureCollectionInstance instance = selectedCollection != INVALID_HASH ? show.fixtureCollections.at(selectedCollection) : nullptr;
    if (ImGui::BeginCombo("Collection", instance ? instance->name.c_str() : "Select a collection")) {
        for (const auto &c: show.fixtureCollections) {
            if (ImGui::Selectable(c.second->name.c_str())) {
                selectedCollection = c.second->id;
            }
        }
        ImGui::EndCombo();
    }

    if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK) == DialogOption_OK &&
        selectedCollection != INVALID_HASH) {
        show.commandHistory.BeginEntry("Add %d to collection '%s'", selectedFixtureIds.size(), instance->name.c_str());
        show.commandHistory.Push(std::make_shared<CommandFixtureCollectionAssignFixtures>(selectedCollection, selectedFixtureIds));
        show.commandHistory.EndEntry();
    }
}

void UI::PanelPatchFixtures::DrawCollectionTable() {
    static char buffer[16];

    ImGui::TableSetupColumn("ID");
    ImGui::TableSetupColumn("Collection");
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::PushFont(ImGui::fontMonospace);
    if (ImGui::Selectable("-", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap)) {
        selectedCollectionIds.clear();
        FilterForCurrentCollection();
    }
    ImGui::PopFont();
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("All fixtures");

    ShowData &show = Engine::Instance().Show();

    for (const auto &collection: show.fixtureCollections) {
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::PushFont(ImGui::fontMonospace);
        snprintf(buffer, sizeof(buffer), "%li", collection.second->id);

        bool selected = selectedCollectionIds.find(collection.second->id) != selectedCollectionIds.end();
        if (ImGui::Selectable(buffer, selected,
                              ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap)) {
            bool ctrl = ImGui::GetIO().KeyCtrl;
            bool shift = ImGui::GetIO().KeyShift;
            if (!ctrl && !shift) {
                selectedCollectionIds.clear();
            }
            if (shift && lastCollectionSelection != INVALID_HASH) {
                selectedCollectionIds.clear();
                bool greater = collection.second->id > lastCollectionSelection;
                for (const auto &f: show.fixtureCollections) {
                    if ((greater && f.second->id >= lastCollectionSelection && f.second->id <= collection.second->id) ||
                        (!greater && f.second->id <= lastCollectionSelection && f.second->id >= collection.second->id)) {
                        selectedCollectionIds.insert(f.second->id);
                    }
                }
            } else {
                selectedCollectionIds.insert(collection.second->id);
                lastCollectionSelection = collection.second->id;
            }
            FilterForCurrentCollection();
        }
        ImGui::PopFont();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", collection.second->name.c_str());
    }

    ImGui::PopStyleVar();

    ImGui::EndTable();
}


void UI::PanelPatchFixtures::DrawFixtureTable() {
    ImGui::TableSetupColumn("ID");
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Patch");
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    static char buffer[16];

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    ShowData &show = Engine::Instance().Show();

    const auto &fixtureSrc = selectedCollectionIds.empty() ? show.fixtures : filteredFixtures;
    for (const auto &fixture: fixtureSrc) {
        bool selected = selectedFixtureIds.find(fixture.second->id) != selectedFixtureIds.end();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        snprintf(buffer, sizeof(buffer), "%li", fixture.second->data.fixtureId);
        ImGui::PushFont(ImGui::fontMonospace);
        if (ImGui::Selectable(buffer, selected,
                              ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap)) {
            bool ctrl = ImGui::GetIO().KeyCtrl;
            bool shift = ImGui::GetIO().KeyShift;
            if (!ctrl && !shift) {
                selectedFixtureIds.clear();
            }
            if (shift && lastFixtureSelection != INVALID_HASH) {
                selectedFixtureIds.clear();
                bool greater = fixture.second->id > lastFixtureSelection;
                for (const auto &f: fixtureSrc) {
                    if ((greater && f.second->id >= lastFixtureSelection && f.second->id <= fixture.second->id) ||
                        (!greater && f.second->id <= lastFixtureSelection && f.second->id >= fixture.second->id)) {
                        selectedFixtureIds.insert(f.second->id);
                    }
                }
            } else {
                selectedFixtureIds.insert(fixture.second->id);
                lastFixtureSelection = fixture.second->id;
            }
        }
        ImGui::PopFont();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", fixture.second->data.name.c_str());
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("Unknown fixture type");
        ImGui::TableSetColumnIndex(3);
        ImGui::PushFont(ImGui::fontMonospace);
        if (fixture.second->data.universe == 0 && fixture.second->data.channel == 0) {
            ImGui::Text("-.---");
        } else if (fixture.second->data.universe != 0 && fixture.second->data.channel != 0) {
            ImGui::Text("%i.%03i", fixture.second->data.universe, fixture.second->data.channel);
        } else if (fixture.second->data.universe != 0) {
            ImGui::Text("%i.---", fixture.second->data.universe);
        } else {
            ImGui::Text("-.%03i", fixture.second->data.channel);
        }
        ImGui::PopFont();
    }

    ImGui::PopStyleVar();

    ImGui::EndTable();
}

void UI::PanelPatchFixtures::Draw() {
    bool openAddCollectionModal = false;
    bool openAddFixturesModal = false;
    bool openAssignFixturesModal = false;
    bool openUnimplementedMessage = false;

    ShowData &show = Engine::Instance().Show();

    if (UI::BeginMenuBar()) {
        if (ImGui::BeginMenu("Collections")) {
            openAddCollectionModal = ImGui::MenuItem("Add collection");
            ImGui::MenuItem("Remove selected");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Fixtures")) {
            openAddFixturesModal = ImGui::MenuItem("Add fixtures");
            if (ImGui::MenuItem("Remove selected")) {
                show.commandHistory.BeginEntry("Remove %d fixtures", selectedFixtureIds.size());
                for (const auto &fix: selectedFixtureIds) {
                    show.commandHistory.Push(std::make_shared<CommandFixtureRemove>(fix));
                }
                show.commandHistory.EndEntry();
            }
            ImGui::Separator();
            openAssignFixturesModal = ImGui::MenuItem("Assign fixtures to collection");
            ImGui::EndMenu();
        }
        UI::EndMenuBar();
    }

    UI::ShowMessagePopup(openUnimplementedMessage, DialogOption_OK, "Feature is not implemented");

    if (UI::BeginPopupDialog(openAddCollectionModal, "Add collection")) {
        static char collectionNameBuffer[100];
        if (openAddCollectionModal) {
            collectionNameBuffer[0] = 0;
        }

        ShowData &show = Engine::Instance().Show();

        DialogOption result = DialogOption_None;
        ImGui::PushID(0);
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputTextWithHint("##v", "Collection name...", collectionNameBuffer, sizeof(collectionNameBuffer),
                                     ImGuiInputTextFlags_EnterReturnsTrue)) {
            result = DialogOption_OK;
        }
        ImGui::PopID();
        if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK, result) == DialogOption_OK) {
            show.commandHistory.BeginEntry("Add collection '%s'", collectionNameBuffer);
            show.commandHistory.Push(std::make_shared<CommandFixtureCollectionAdd>((Hash) show.fixtureCollections.size() + 1, collectionNameBuffer));
            show.commandHistory.EndEntry();
        }
    }

    ImVec2 halfVp = ImGui::GetMainViewport()->WorkSize / 2;
    ImGui::SetNextWindowSize(halfVp);
    ImGui::SetNextWindowPos(halfVp / 2, ImGuiCond_Appearing);
    if (UI::BeginPopupDialog(openAddFixturesModal, "Add fixtures")) {
        DrawAddFixtureModal(openAddFixturesModal);
    }

    if (UI::BeginPopupDialog(openAssignFixturesModal, "Assign to collections")) {
        DrawAddFixturesToCollectionModal(openAssignFixturesModal);
    }

    if (ImGui::BeginTable("table0", 2, ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableSetupColumn("Collections", ImGuiTableColumnFlags_None, 0.35f);
        ImGui::TableSetupColumn("Fixtures");
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("table1", 2,
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit |
                              ImGuiTableFlags_ScrollY)) {
            DrawCollectionTable();
        }

        ImGui::TableNextColumn();
        if (ImGui::BeginTable("table2", 4,
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit |
                              ImGuiTableFlags_ScrollY)) {
            DrawFixtureTable();
        }

        ImGui::EndTable();
    }
}

void UI::PanelPatchFixtures::OnShow() {
    FilterForCurrentCollection();
}

void UI::PanelPatchFixtures::FilterForCurrentCollection() {
    ShowData &show = Engine::Instance().Show();
    filteredFixtures.clear();
    std::set<Hash> availableInCollections;
    for (Hash id: selectedCollectionIds) {
        auto collection = show.fixtureCollections.at(id);
        availableInCollections.insert(collection->assignedFixtures.begin(), collection->assignedFixtures.end());
    }
    Filter(show.fixtures, filteredFixtures, [&availableInCollections](const auto &param) {
        return availableInCollections.find(param.second->id) != availableInCollections.end();
    });
}
