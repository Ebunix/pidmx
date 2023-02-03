#include "Engine.h"
#include "Fixture.h"
#include "imgui.h"
#include "Show.h"
#include <js/js.h>
#include "engine/ui/BlackboardPanel.h"

using namespace v8;

Engine::Engine()
{
    js::ObjectTemplate fixtures;
    Local<ObjectTemplate> fixturesTemplate = fixtures.Begin()
        ->Function("patch", Fixture::Patch)
        ->Build();

    js::ObjectTemplate engine;
    engine.Begin()
        ->Object("fixtures", fixturesTemplate)
        ->BuildGlobal("engine");


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
