#include "PatchFixturesPanel.h"
#include "ImGuiExt.h"
#include "engine/core/Utils.h"
#include <engine/command/CommandFixtureCollection.h>
#include <engine/command/CommandFixture.h>
#include <engine/core/Show.h>
#include <set>


void UI::PatchFixturesPanel::DrawAddFixtureModal(bool openAddFixturesModal) {
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
//        Hash id = std::stoi(fixtureIdBuffer);
        int quantity = std::stoi(fixtureQuantityBuffer);
        int footprint = std::stoi(fixtureDmxChannelsBuffer);

        std::vector<FixtureData> fixtures;
        for (int i = 0; i < quantity; i++) {
            fixtures.push_back(FixtureData{fixtureNameBuffer, 0, universe, channel + footprint * i});
        }
        currentShow->commandHistory.Push("Patch fixtures", CommandFixtureAdd::New(fixtures));
    }
}

void UI::PatchFixturesPanel::DrawAddFixturesToCollectionModal(bool open) {
    static Hash selectedCollection = INVALID_HASH;

    FixtureCollectionInstance instance = currentShow->fixtureCollections.at(selectedCollection);
    if (ImGui::BeginCombo("Collection", instance ? instance->name.c_str() : "Select a collection")) {
        for (const auto &c: currentShow->fixtureCollections) {
            if (ImGui::Selectable(c.second->name.c_str())) {
                selectedCollection = c.second->id;
            }
        }
        ImGui::EndCombo();
    }

    if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK) == DialogOption_OK &&
        selectedCollection != INVALID_HASH) {
        currentShow->commandHistory.Push("Assign fixtures to collection",
                                         CommandFixtureCollectionAssignFixtures::New(selectedCollection,
                                                                                     selectedFixtureIds));
    }
}

void UI::PatchFixturesPanel::DrawCollectionTable() {
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


    for (const auto &collection: currentShow->fixtureCollections) {
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
                for (const auto &f: currentShow->fixtureCollections) {
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


void UI::PatchFixturesPanel::DrawFixtureTable() {
    ImGui::TableSetupColumn("ID");
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Patch");
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    static char buffer[16];

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

    const auto &fixtureSrc = selectedCollectionIds.empty() ? currentShow->fixtures : filteredFixtures;
    for (const auto &fixture: fixtureSrc) {
        bool selected = selectedFixtureIds.find(fixture.second->id) != selectedFixtureIds.end();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        snprintf(buffer, sizeof(buffer), "%li", fixture.second->id);
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
        } else if (fixture.second->data.channel != 0) {
            ImGui::Text("-.%03i", fixture.second->data.channel);
        } else if (fixture.second->data.universe != 0) {
            ImGui::Text("%i.---", fixture.second->data.universe);
        } else {
            ImGui::Text("%i.%03i", fixture.second->data.universe, fixture.second->data.channel);
        }
        ImGui::PopFont();
    }

    ImGui::PopStyleVar();

    ImGui::EndTable();
}

void UI::PatchFixturesPanel::Draw() {
    bool openAddCollectionModal = false;
    bool openAddFixturesModal = false;
    bool openAssignFixturesModal = false;
    bool openUnimplementedMessage = false;

    if (UI::BeginMenuBar()) {
        if (ImGui::BeginMenu("Collections")) {
            openAddCollectionModal = ImGui::MenuItem("Add collection");
            ImGui::MenuItem("Remove selected");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Fixtures")) {
            openAddFixturesModal = ImGui::MenuItem("Add fixtures");
            if (ImGui::MenuItem("Remove selected")) {
                currentShow->commandHistory.Push("Remove fixtures", CommandFixtureRemove::New(selectedFixtureIds));
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

        DialogOption result = DialogOption_None;
        ImGui::PushID(0);
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputTextWithHint("##v", "Collection name...", collectionNameBuffer, sizeof(collectionNameBuffer),
                                     ImGuiInputTextFlags_EnterReturnsTrue)) {
            result = DialogOption_OK;
        }
        ImGui::PopID();
        if (UI::EndPopupDialog(DialogOption_Cancel | DialogOption_OK, result) == DialogOption_OK) {
            currentShow->commandHistory.Push("Create collection", CommandFixtureCollectionAdd::New(
                    (Hash) currentShow->fixtureCollections.size() + 1, collectionNameBuffer));
        }
    }

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

void UI::PatchFixturesPanel::OnShow() {
    FilterForCurrentCollection();
}

void UI::PatchFixturesPanel::FilterForCurrentCollection() {
    filteredFixtures.clear();
    std::set<Hash> availableInCollections;
    for (Hash id: selectedCollectionIds) {
        auto collection = currentShow->fixtureCollections.at(id);
        availableInCollections.insert(collection->assignedFixtures.begin(), collection->assignedFixtures.end());
    }
    Filter(currentShow->fixtures, filteredFixtures, [&availableInCollections](const auto &param) {
        return availableInCollections.find(param.second->id) != availableInCollections.end();
    });
}
