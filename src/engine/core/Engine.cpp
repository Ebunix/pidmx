#include "Engine.h"
#include "imgui.h"
#include "ShowData.h"
#include <js/js.h>
#include "engine/ui/blackboard/Panel.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Engine* engineInstance;

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
    engineInstance = this;
    NewShow();

    char varName[32], varValue[32];
    int argIndex = 1;
    dpiScale = 1.0f;
    while (eatParam(argc, argv, &argIndex, varName, sizeof(varName), varValue, sizeof(varValue))) {
        if (strcmp(varName, "dpi") == 0) {
            dpiScale = std::stof(varValue);
        }
        else {
            LogMessage(LogMessageType_Warn, "Unrecognized command line parameter '%s'", varName);
        }
    }

#ifdef PIDMX_ENABLE_JAVASCRIPT
    //js::ObjectTemplate fixtures;
    //v8::Local<v8::ObjectTemplate> fixturesTemplate = fixtures.Begin()
    //    ->Function("patch", Fixture::Patch)
    //    ->Build();

    //js::ObjectTemplate engine;
    //engine.Begin()
    //    ->Object("fixtures", fixturesTemplate)
    //    ->BuildGlobal("engine");
#endif

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
    currentShow = new ShowData();
    currentShow->Register();
}

void Engine::LoadShow(const std::string &path) {
    delete currentShow;
    currentShow = new ShowData();
    currentShow->Register();
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

void Engine::LoadFixturePresetsFromDirectory(const std::string &dir) {
    for (const auto & entry : fs::directory_iterator(dir)) {
        auto root = nbt::LoadFromFile(entry.path().string());
        auto vec = nbt::Deserialize(*root, "fixturePresets", Set<FixturePresetInstance>());
        fixturePresets.insert(vec.begin(), vec.end());
    }

    nbt::tag_compound root;
    root.insert("fixturePresets", nbt::Serialize(fixturePresets.begin(), fixturePresets.end()));
    nbt::SaveToFile(root, "fixturePresets.bundle", true);
}

void Engine::SetAction(EngineAction action) {
    this->action = action;
}

void Engine::Clear() {
    if (action != EngineAction_None) {
        SetAction(EngineAction_None);
    }
    else if (programmer.IsTrackingAnything()) {
        programmer.Clear();
    }
    else {
        bool isAnyOutputDiverged = false;
        for (const auto& fix : currentShow->fixtures) {
            if (fix.second->IsAnyOutputDiverged()) {
                isAnyOutputDiverged = true;
                break;
            }
        }
        if (isAnyOutputDiverged) {
            for (const auto &fix: currentShow->fixtures) {
                fix.second->ResetOutputToCalled();
            }
            activePreset = INVALID_HASH;
        }
        else {
            activePreset = INVALID_HASH;
            activeFixtures.clear();
            activeGroups.clear();
        }
    }
}

void Engine::SetActiveFixtures(const IDSet &fixtures) {
    activeFixtures = fixtures;
    UpdateAvailableParameters();
}

void Engine::UpdateAvailableParameters() {
    availableParametersOnFixtures.clear();
    for (const auto& fixtureId : activeFixtures) {
        const auto& fixture = currentShow->fixtures.at(fixtureId);
        if (fixture->data.presetId == INVALID_HASH) {
            continue;
        }
        const auto& preset = currentShow->fixturePresets.at(fixture->data.presetId);
        for (const auto& param : preset->parameters) {
            availableParametersOnFixtures.insert(param->type);
        }
    }

}

void Engine::AddActiveGroup(Hash id) {
    const auto& group = currentShow->groups.at(id);
    activeGroups.insert(group.id);
    activeFixtures.insert(group.fixtures.begin(), group.fixtures.end());
    UpdateAvailableParameters();
}

void Engine::ActivatePreset(Hash presetId) {
    for (const auto& pres : currentShow->presets.at(presetId).callables) {
        for (const auto& val : pres.values) {
            FixtureInstance &fix = currentShow->fixtures.at(pres.fixtureId);
            fix->SetParamValue(val.first, val.second);
            fix->SetParamValue(val.first, val.second, activePreset != presetId);
        }
    }
    activePreset = presetId;
}


