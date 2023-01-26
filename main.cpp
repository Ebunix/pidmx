#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <engine/Engine.h>
#include <js.h>

#include <PanelChannelMonitor.h>
#include <PanelPatch.h>
#include <PanelChannelEditor.h>
#include <PanelConsole.h>
#include <rewind.h>

GLFWwindow* glfwWindow = nullptr;

ImGuiWindowFlags consoleWindowFlags =
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;

void setImGuiStyle(float highDPIscaleFactor)
{
    ImGuiStyle &style = ImGui::GetStyle();

    // https://github.com/ocornut/imgui/issues/707#issuecomment-415097227
    style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_Header]                 = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
    style.Colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

    style.WindowPadding            = ImVec2(5, 5);
    style.WindowRounding           = 5.0f;
    style.FramePadding             = ImVec2(14, 8);
    style.CellPadding              = ImVec2(8, 8);
    //style.FrameRounding            = 5.0f;
    style.ItemSpacing              = ImVec2(5, 5);
    // style.ItemInnerSpacing         = ImVec2(1, 1);
    // style.TouchExtraPadding        = ImVec2(0, 0);
    // style.IndentSpacing            = 6.0f;
    // style.ScrollbarSize            = 12.0f;
    // style.ScrollbarRounding        = 16.0f;
    // style.GrabMinSize              = 20.0f;
    style.GrabRounding             = 2.0f;
    // style.WindowTitleAlign.x = 0.50f;
    // style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::fontRegular = io.Fonts->AddFontFromFileTTF(
        "resources/Roboto-Regular.ttf",
        16.0f * engine->displayScale,
        NULL,
        NULL
    );
    ImGui::fontMonospace = io.Fonts->AddFontFromFileTTF(
        "resources/JetBrainsMono-Regular.ttf",
        16.0f * engine->displayScale,
        NULL,
        NULL
    );
    ImGui::fontBackgroundMassive = io.Fonts->AddFontFromFileTTF(
        "resources/JetBrainsMono-Regular.ttf",
        96.0f * engine->displayScale,
        NULL,
        NULL
    );

    style.ScaleAllSizes(highDPIscaleFactor);
}

static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool initGLFW(const char* title, bool fullscreen) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LogMessage(ConsoleMessageType_Error, "Couldn't initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER , 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    std::string glsl_version = "#version 150";

    float scale = 1;
    int displayWidth, displayHeight;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(monitor, &scale, &scale);
    glfwGetMonitorWorkarea(monitor, NULL, NULL, &displayWidth, &displayHeight);

    if (fullscreen) {
        glfwWindow = glfwCreateWindow(displayWidth, displayHeight, title, monitor, NULL);
    }
    else {
        glfwWindow = glfwCreateWindow(displayWidth / 2, displayHeight / 2, title, NULL, NULL);
    }
    if (!glfwWindow) {
        LogMessage(ConsoleMessageType_Error, "Couldn't create a GLFW window");
        return false;
    }

    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);

    LogMessage(ConsoleMessageType_Info, "OpenGL from GLFW %i.%i",
        glfwGetWindowAttrib(glfwWindow, GLFW_CONTEXT_VERSION_MAJOR),
        glfwGetWindowAttrib(glfwWindow, GLFW_CONTEXT_VERSION_MINOR));

    return true;
}

bool initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LogMessage(ConsoleMessageType_Error, "Couldn't initialize GLAD");
        return false;
    }

    LogMessage(ConsoleMessageType_Info, "OpenGL renderer: %s", glGetString(GL_RENDERER));
    LogMessage(ConsoleMessageType_Info, "OpenGL from glad %i.%i", GLVersion.major, GLVersion.minor);

    return true;
}

bool initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true)) {
        LogMessage(ConsoleMessageType_Error, "Couldn't initialize ImGui for GLFW");
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init()) {
        LogMessage(ConsoleMessageType_Error, "Couldn't initialize ImGui for OpenGL3");
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (!initGLFW("PiDMX", true)) {
        return -1;
    }
    if (!initGLAD()) {
        return -1;
    }
    if (!initImGui()) {
        return -1;
    }
    if (!jsInitRuntime("snapshot_blob.bin")) {
        return -1;
    }

    v8::HandleScope scope(js::global::isolate);

    engine = new Engine();
    engine->displayScale = 2.5f;
    setImGuiStyle(engine->displayScale);

    js::execFile("resources/extern/init.js");

    bool open = true;

    std::vector<Panel*> panels;
    panels.push_back(new PanelChannelMonitor());
    panels.push_back(new PanelChannelEditor());
    panels.push_back(new PanelPatch());
    panels.push_back(new PanelConsole());

    FixturePreset* currentPreset = nullptr;
    
    std::vector<Modulator*> modulators;
    for (int i = 0; i < 20; i++) {
        Modulator* mod = engine->modulators[0]->Clone();
        mod->SetParameter("phase", (1.0f / 20) * i);
        modulators.push_back(mod);
    }

    while (!glfwWindowShouldClose(glfwWindow)) {

        engine->Update();

        glClearColor(0.025, 0.025, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

        if (open) {
            ImGui::ShowDemoWindow(&open);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    engine->SaveShowFile("showfile.xml");
                }
                if (ImGui::MenuItem("Load")) {
                    engine->LoadShowFile("showfile.xml");
                }
                if (ImGui::MenuItem("Quit")) {
                    exit(0);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Panels")) {
                for (auto& panel : panels) {
                    ImGui::MenuItem(panel->title.c_str(), "", &panel->open);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();    
        }
        ImGui::PopStyleVar(1);

        for (const auto& panel : panels) {
            panel->Draw();
        }

        if (ImGui::Begin("Debug Tools")) {
            if (ImGui::Button("Assign sine modulators to slots 1-20")) {
                for (int i = 0; i < modulators.size(); i++) {
                    engine->universes[0]->SetSlotModulationSource(i, modulators[i]);
                }
            }
            if (ImGui::BeginCombo("Presets", currentPreset ? currentPreset->name.c_str() : nullptr)) {
                for (const auto& preset : engine->fixturePresets) {
                    if (ImGui::Selectable(preset->name.c_str())) {
                        currentPreset = preset;
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Undo")) {
                Rewind::Undo(1);
            }
            ImGui::SameLine();
            if (ImGui::Button("Redo")) {
                Rewind::Redo(1);
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    return 0;
}
