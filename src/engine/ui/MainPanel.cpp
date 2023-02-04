#include "MainPanel.h"
#include "Tools.h"
#include "PatchFixturesPanel.h"
#include "PanelConsole.h"
#include <string.h>
#include <engine/core/Show.h>

enum class MainPanelMode {
	MainMenu,
	FixturesPatch
};

MainPanelMode mainPanelMode = MainPanelMode::MainMenu;
ImVec2 buttonSmall(100, 32);
ImVec2 buttonLarge(230, 48);

void DrawMainMenuPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		
	ImGui::Text("Patch setup");
	if (ImGui::Button("Fixtures & Patch", buttonLarge * globalEngine->dpiScale)) {
		mainPanelMode = MainPanelMode::FixturesPatch;
	}

	ImGui::PopStyleVar(1);
}

void UI::DrawMainPanel(Engine &engine)
{
	if (UI::BeginMainMenuBar()) {
        ImGuiIO &io = ImGui::GetIO();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl+N") || (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N))) {
                engine.NewShow();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S") || (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))) {
				engine.SaveShow("show.pidmxs");
			}
			ImGui::MenuItem("Save as...", "Ctrl+Shift+S");
			if (ImGui::MenuItem("Load", "Ctrl+O") || (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O))) {
                engine.LoadShow("show.pidmxs");
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				exit(0);
			}
			ImGui::EndMenu();
		}
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                engine.Undo();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                engine.Redo();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows")) {

            ImGui::EndMenu();
        }
		UI::EndMainMenuBar();
	}
	
	static bool windowSetup = false;

	ImGuiViewport* vp = ImGui::GetMainViewport();
    ImVec2 workArea = vp->WorkSize;
    ImVec2 workPos = vp->WorkPos;


	float leftPanelWidth = 75 * globalEngine->dpiScale;
	// ImVec2 sideAreaPos = ImVec2(0, vp->WorkPos.y);
    // ImVec2 mainAreaPos = ImVec2(vp->WorkPos.x + leftPanelWidth, vp->WorkPos.y);
    // ImVec2 mainAreaSize = ImVec2(workArea.x - leftPanelWidth, workArea.y);
    ImVec2 mainAreaPos = workPos;
    ImVec2 mainAreaSize = workArea;


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

    /*
	ImGui::SetNextWindowPos(sideAreaPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(leftPanelWidth - globalEngine->dpiScale, workArea.y), ImGuiCond_Always);
	ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::Button("Setup", ImVec2(leftPanelWidth - globalEngine->dpiScale, 48 * globalEngine->dpiScale))) {
		windowSetup = !windowSetup;
	}

	ImGui::End();
     */

    ImGui::SetNextWindowPos(mainAreaPos);
    ImGui::SetNextWindowSize(mainAreaSize);
    ImGui::SetNextWindowViewport(vp->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
    host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    char label[32];
    snprintf(label, sizeof(label), "DockSpaceViewport_%08X", vp->ID);

    ImGui::Begin(label, nullptr, host_window_flags);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode, 0);
    ImGui::End();
 
    ImGui::PopStyleVar(3);

	if (windowSetup) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16 * globalEngine->dpiScale, 16 * globalEngine->dpiScale));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::SetNextWindowPos(mainAreaPos);
		ImGui::SetNextWindowSize(mainAreaSize);
		if (ImGui::Begin("Main menu", &windowSetup, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration)) {
			

			switch (mainPanelMode) {
				default: mainPanelMode = MainPanelMode::MainMenu; break;
				case MainPanelMode::MainMenu: DrawMainMenuPanel(); break;
			}
		}
		
		ImGui::PopStyleVar(3);
		ImGui::End();
	}

    UI::DrawConsolePanel();
}