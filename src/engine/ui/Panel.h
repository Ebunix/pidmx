#pragma once
#include <string>
#include <utility>
#include <imgui.h>
#include "engine/core/NbtFileIO.h"
#include "engine/core/IIdentifiable.h"
//
// Created by ebu on 30.01.23.
//

namespace UI {
    enum PanelType {
        PanelType_None,
        PanelType_FixturePatch,
        PanelType_Console,
        PanelType_Blackboard,
        PanelType_PresetEditor,
        PanelType_Operators,
    };

    class Panel: public IIdentifiable {
    public:
        std::string name;
        bool open = false;
        bool drawWindowFrame = true;
        PanelType type = PanelType_None;

        explicit Panel(std::string title, PanelType type, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

        nbt::tag_compound Save() const override {
            nbt::tag_compound c = IIdentifiable::Save();
            c.insert("name", nbt::Serialize(name));
            c.insert("open", nbt::Serialize(open));
            c.insert("type", nbt::Serialize(type));
            return c;
        }
        void Load(const nbt::tag_compound &c) override {
            IIdentifiable::Load(c);
            name = nbt::Deserialize(c, "name", std::string("Panel"));
            open = nbt::Deserialize(c, "open", false);
            type = (PanelType)nbt::Deserialize(c, "type", PanelType_None);
        }


        virtual void Draw() = 0;
        virtual void OnHide() {}
        virtual void OnShow() {}
        void Render() {
            if (!drawWindowFrame) {
                Draw();
                return;
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
            if (open) {
                if (wasLoaded) {
                    wasLoaded = false;
                    ImGui::SetNextWindowPos(position);
                    ImGui::SetNextWindowSize(size);
                }
                if (ImGui::Begin(name.c_str(), &open, flags)) {
                    position = ImGui::GetWindowPos();
                    size = ImGui::GetWindowSize();
                    Draw();
                }
                ImGui::End();
            }
        }
    private:
        bool wasOpen = false;
        ImGuiWindowFlags flags;
        bool wasLoaded = false;
        ImVec2 position;
        ImVec2 size;
    };
    typedef std::shared_ptr<Panel> PanelInstance;

    PanelInstance CreatePanel(int type);
}
