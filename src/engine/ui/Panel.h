#pragma once
#include <string>
#include <utility>
#include <imgui.h>
#include "engine/core/ISerializable.h"
//
// Created by ebu on 30.01.23.
//

namespace UI {
    enum PanelType {
        PanelType_None,
        PanelType_FixturePatch,
        PanelType_Console,
        PanelType_Blackboard
    };

    class Panel: public ISerializable {
    public:
        bool open = true;
        bool drawWindowFrame = true;
        PanelType type = PanelType_None;

        explicit Panel(std::string title, PanelType type, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

        virtual void Draw() = 0;
        virtual void OnHide() {}
        virtual void OnShow() {}
        void Render() {
            if (!drawWindowFrame) {
                Draw();
                return;
            }
            if (open) {
                if (ImGui::Begin(name.c_str(), &open, flags)) {
                    Draw();
                }
                ImGui::End();
            }
            if (open != wasOpen) {
                if (open) {
                    OnShow();
                }
                else {
                    OnHide();
                }
                wasOpen = open;
            }
        }
    private:
        bool wasOpen = false;
        ImGuiWindowFlags flags;
    };
    typedef std::shared_ptr<Panel> PanelInstance;

    PanelInstance CreatePanel(int type);
}