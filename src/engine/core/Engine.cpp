#include "Engine.h"
#include "Fixture.h"
#include "imgui.h"
#include "Show.h"
#include <js/js.h>
#include "engine/ui/BlackboardPanel.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace v8;

Engine* globalEngine;

bool eatParam(int argc, char* argv[], int* index, char* name, size_t nameLen, char* value, size_t valueLen) {
    if (*index >= argc) {
        return false;
    }
    if (argv[*index][0] != '-' && argv[*index][1] != '-') {
        return false;
    }
    strncpy(name, argv[*index] + 2, nameLen);
    *index = *index + 1;
    if (*index >= argc || argv[*index][0] == '-') {
        value[0] = 0;
        return true;
    }
    strncpy(value, argv[*index], valueLen);
    *index = *index + 1;
    return true;
}

Engine::Engine(int argc, char* argv[])
{
    char varName[32], varValue[32];
    int argIndex = 1;
    while (eatParam(argc, argv, &argIndex, varName, sizeof(varName), varValue, sizeof(varValue))) {
        if (strcmp(varName, "dpi") == 0) {
            dpiScale = std::stof(varValue);
        }
        else {
            LogMessage(LogMessageType_Warn, "Unrecognized command line parameter '%s'", varName);
        }
    }
    globalEngine = this;

    js::ObjectTemplate fixtures;
    Local<ObjectTemplate> fixturesTemplate = fixtures.Begin()
        ->Function("patch", Fixture::Patch)
        ->Build();

    js::ObjectTemplate engine;
    engine.Begin()
        ->Object("fixtures", fixturesTemplate)
        ->BuildGlobal("engine");

    LoadFixturePresetsFromDirectory("resources/fixtures");
}

void Engine::ProcessHotkeys() {
    ImGuiIO &io = ImGui::GetIO();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z)) {
        currentShow->commandHistory.Undo();
    }
    else if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y)) {
        currentShow->commandHistory.Redo();
    }
    else if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N)) {
       NewShow();
    }
    else if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O)) {
        LoadShow("show.pidmxs");
    }
    else if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
        SaveShow("show.pidmxs");
    }

}

void Engine::NewShow() {
    delete currentShow;
    currentShow = new Show();
}

void Engine::LoadShow(const std::string &path) {
    delete currentShow;
    currentShow = new Show();
    currentShow->Load(path);
}

void Engine::SaveShow(const std::string &path) {
    currentShow->Save(path);
}

void Engine::Undo() {
    currentShow->commandHistory.Undo();
}

void Engine::Redo() {
    currentShow->commandHistory.Redo();
}

void Engine::DrawPanels() {
    currentShow->panelBlackboard->Render();
    currentShow->panelPatchFixtures->Render();
}

void Engine::LoadFixturePresetsFromDirectory(const std::string &dir) {
    for (const auto & entry : fs::directory_iterator(dir)) {
        auto root = nbt::LoadFromFile(entry.path());
        auto vec = nbt::Load(*root, "fixturePresets", std::vector<FixturePreset>{});
        fixturePresets.insert(fixturePresets.end(), vec.begin(), vec.end());
    }

    FixturePreset pres;
    pres.footprint = (int)fixturePresets.size();
    pres.manufacturer = "Auto-generated";
    pres.parameters.push_back(FixtureParameter{});
    fixturePresets.push_back(pres);

    nbt::tag_compound root;
    nbt::Save(root, "fixturePresets", fixturePresets);
    nbt::SaveToFile(root, "resources/fixtures/fixtures.bundle1");
}
