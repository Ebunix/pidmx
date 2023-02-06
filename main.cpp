#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <engine/ui/ImGuiExt.h>

#include <js/js.h>
#include <engine/core/ShowData.h>
#include <engine/command/CommandFixture.h>
#include <engine/ui/MainPanel.h>
#include "engine/core/Engine.h"
#include "engine/ui/Panel.h"
#include "engine/ui/PatchFixturesPanel.h"
#include "engine/ui/blackboard/Panel.h"

GLFWwindow *glfwWindow = nullptr;

ImGuiWindowFlags consoleWindowFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;

void setImGuiStyle(float highDPIscaleFactor) {
    ImGuiStyle &style = ImGui::GetStyle();

    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.33f, 0.34f, 0.34f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.27f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.27f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.27f);
    colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.27f);
    colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
    colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.27f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.22f, 0.55f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.56f, 0.94f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.18f, 0.40f, 0.79f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.22f, 0.55f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.40f, 0.79f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.18f, 0.40f, 0.79f, 0.35f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


    style.WindowPadding = ImVec2(8, 8);
    style.WindowRounding = 5.0f;
    style.FramePadding = ImVec2(14, 8);
    style.CellPadding = ImVec2(6, 2);
    style.FrameRounding = 5.0f;
    style.ItemSpacing = ImVec2(5, 5);
    // style.ItemInnerSpacing         = ImVec2(1, 1);
    // style.TouchExtraPadding        = ImVec2(0, 0);
    // style.IndentSpacing            = 6.0f;
    // style.ScrollbarSize            = 12.0f;
    // style.ScrollbarRounding        = 16.0f;
    // style.GrabMinSize              = 20.0f;
    style.GrabRounding = 2.0f;
    // style.WindowTitleAlign.x = 0.50f;
    // style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = Engine::Instance().dpiScale;

    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::fontRegular = io.Fonts->AddFontFromFileTTF(
            "resources/Roboto-Regular.ttf",
            17.0f * Engine::Instance().dpiScale /* engine->displayScale */,
            NULL,
            NULL
    );
    ImGui::fontRegularSmall = io.Fonts->AddFontFromFileTTF(
            "resources/Roboto-Regular.ttf",
            14.0f * Engine::Instance().dpiScale /* engine->displayScale */,
            NULL,
            NULL
    );
    ImGui::fontMonospace = io.Fonts->AddFontFromFileTTF(
            "resources/JetBrainsMono-Regular.ttf",
            17.0f * Engine::Instance().dpiScale,
            NULL,
            NULL
    );
    ImGui::fontPixel = io.Fonts->AddFontFromFileTTF(
            "resources/ponderosa.regular.ttf",
            10.0f * Engine::Instance().dpiScale,
            NULL,
            NULL
    );
    ImGui::fontPixelGigantic = io.Fonts->AddFontFromFileTTF(
            "resources/ponderosa.regular.ttf",
            180.0f * Engine::Instance().dpiScale,
            NULL,
            NULL
    );
    /*
    ImGui::fontBackgroundMassive = io.Fonts->AddFontFromFileTTF(
        "resources/JetBrainsMono-Regular.ttf",
        96.0f * engine->displayScale,
        NULL,
        NULL
    );*/

    style.ScaleAllSizes(highDPIscaleFactor);
}

static void glfw_error_callback(int error, const char *description) {
    std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool initGLFW(const char *title, bool fullscreen) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LogMessage(LogMessageType_Error, "Couldn't initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    std::string glsl_version = "#version 150";

    float scale = 1;
    int displayWidth, displayHeight;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(monitor, &scale, &scale);
    glfwGetMonitorWorkarea(monitor, NULL, NULL, &displayWidth, &displayHeight);

    if (fullscreen) {
        glfwWindow = glfwCreateWindow(displayWidth, displayHeight, title, monitor, NULL);
    } else {
        glfwWindow = glfwCreateWindow(displayWidth / 2, displayHeight / 2, title, NULL, NULL);
    }
    if (!glfwWindow) {
        LogMessage(LogMessageType_Error, "Couldn't create a GLFW window");
        return false;
    }

    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);

    LogMessage(LogMessageType_Info, "OpenGL from GLFW %i.%i",
               glfwGetWindowAttrib(glfwWindow, GLFW_CONTEXT_VERSION_MAJOR),
               glfwGetWindowAttrib(glfwWindow, GLFW_CONTEXT_VERSION_MINOR));

    return true;
}

bool initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LogMessage(LogMessageType_Error, "Couldn't initialize GLAD");
        return false;
    }

    LogMessage(LogMessageType_Info, "OpenGL renderer: %s", glGetString(GL_RENDERER));
    LogMessage(LogMessageType_Info, "OpenGL from glad %i.%i", GLVersion.major, GLVersion.minor);

    return true;
}

bool initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.IniFilename = nullptr;

    if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true)) {
        LogMessage(LogMessageType_Error, "Couldn't initialize ImGui for GLFW");
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init()) {
        LogMessage(LogMessageType_Error, "Couldn't initialize ImGui for OpenGL3");
        return false;
    }
    return true;
}


int main(int argc, char *argv[]) {
    if (!initGLFW("PiDMX", false)) {
        return -1;
    }
    if (!initGLAD()) {
        return -1;
    }
    if (!initImGui()) {
        return -1;
    }
#ifdef PIDMX_ENABLE_JAVASCRIPT
    if (!jsInitRuntime("snapshot_blob.bin")) {
        return -1;
    }
    v8::HandleScope scope(js::global::isolate);
#endif
    bool openDemoWindow = false;

    Engine engine(argc, argv);
    engine.NewShow();

    setImGuiStyle(Engine::Instance().dpiScale);
#ifdef PIDMX_ENABLE_JAVASCRIPT
    js::exec("let fixtures = [];"
             "for (let i = 0; i < 30; i++) fixtures.push({name: 'Fixture A', fixtureId: 201 + i, channel: 0, universe: 0});"
             "for (let i = 0; i < 30; i++) fixtures.push({name: 'Fixture B', fixtureId: 101 + i, channel: 0, universe: 0});"
             "engine.fixtures.patch(fixtures);");
#else
    LogMessage(LogMessageType_Warn, "This build does not include Javascript support");
#endif

    char versionInfo[20];
    FORMAT_VERSION(versionInfo, sizeof(versionInfo), Engine::Instance().VERSION);
    while (!glfwWindowShouldClose(glfwWindow)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;

        static ImVec2 bgTextSize = ImGui::fontPixelGigantic->CalcTextSizeA(ImGui::fontPixelGigantic->FontSize, FLT_MAX, 0.0f, "PiDMX");
        static ImVec2 verTextSize = ImGui::fontPixelGigantic->CalcTextSizeA(ImGui::fontPixelGigantic->FontSize / 8, FLT_MAX, 0.0f, versionInfo);
        ImVec2 bgTextPos = (workSize - bgTextSize) / 2;
        ImVec2 verTextPos = (workSize - verTextSize) / 2;
        verTextPos.y += ImGui::fontPixelGigantic->FontSize / 2;
        ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
        bgDrawList->AddText(ImGui::fontPixelGigantic, ImGui::fontPixelGigantic->FontSize, bgTextPos, (ImColor)UI::ColorOutlineHint, "PiDMX");
        bgDrawList->AddText(ImGui::fontPixelGigantic, ImGui::fontPixelGigantic->FontSize / 8, verTextPos, (ImColor)UI::ColorOutlineHint, versionInfo);

        if (openDemoWindow) {
            ImGui::ShowDemoWindow(&openDemoWindow);
        }

        UI::DrawMainPanel();
        engine.Show().RenderPanels();

        /*
        if (ImGui::Begin("Debug")) {
            for (const auto& item : currentShow->commandHistory.buffer) {
                ImGui::Text("%s", item.name.c_str());
            }
        }
        ImGui::End();
*/

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
        engine.Show().commandHistory.Transfer();
        Engine::Instance().ProcessHotkeys();
    }

    return 0;
}
