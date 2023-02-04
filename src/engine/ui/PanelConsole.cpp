#include "PanelConsole.h"
#include "ImGuiExt.h"
#include <js/js.h>
#include <imgui.h>
#include <engine/core/Engine.h>
#include <math.h>
#include <vector>

char commandBuffer[5000];
std::vector<std::string> commandHistory;
int historyPos = 0;

static int HandleHistoryCallback(ImGuiInputTextCallbackData* data) {

	switch (data->EventFlag) {
		case ImGuiInputTextFlags_CallbackHistory: { 
			size_t size = commandHistory.size();
			if (data->EventKey == ImGuiKey_UpArrow) {
				historyPos = fmax(0, historyPos - 1);
				if (historyPos < size) {
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, commandHistory[historyPos].c_str());
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow) {
				historyPos = fmin(commandHistory.size(), historyPos + 1);
				if (historyPos < size) {
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, commandHistory[historyPos].c_str());
				}
				else {
					data->DeleteChars(0, data->BufTextLen);
				}
			}
			break;
		}
	}
	return 0;
}

void UI::DrawConsolePanel() {
	static bool lastActive = false;
	static size_t lastLogSize = 0;
	static bool commandSent = false;
    static float commandLineHeight = 0;

    if (ImGui::Begin("Console")) {
        ImGui::PushFont(ImGui::fontMonospace);
        ImVec2 size = ImGui::GetContentRegionAvail();
        size.y -= commandLineHeight;
        ImGui::BeginChildFrame(ImGui::GetID("frame"), size);
        for (const auto &str: consolePanelLogBuffer) {
            switch (str.type) {
                case LogMessageType_Debug:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.8f, 1.0f));
                    break;
                case LogMessageType_Warn:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.3f, 1.0f));
                    break;
                case LogMessageType_Error:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
                    break;
                default:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
                    break;
            }
            ImGui::TextWrapped("%s", str.message);
            ImGui::PopStyleColor();
        }
        if (lastLogSize != consolePanelLogBuffer.size()) {
            ImGui::SetScrollHereY(1.0f);
            lastLogSize = consolePanelLogBuffer.size();
        }

        ImGui::EndChild();

        ImGui::PushID(1);
        ImGui::SetNextItemWidth(-1);
        if (commandSent) {
            commandSent = false;
            ImGui::SetKeyboardFocusHere();
        }
        float startPos = ImGui::GetCursorPosY();
#ifdef PIDMX_ENABLE_JAVASCRIPT
        ImGui::InputTextWithHint("##v", "Command...", commandBuffer, sizeof(commandBuffer),
                                 ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackHistory,
                                 HandleHistoryCallback);
        bool active = ImGui::IsItemActive();
        if (lastActive && ImGui::IsKeyDown(ImGuiKey_Enter)) {
            commandSent = true;
            if (strlen(commandBuffer) > 0) {
                commandHistory.push_back(commandBuffer);
                historyPos = commandHistory.size();
                LogMessage(LogMessageType_Info, "%s", commandBuffer);
                v8::Local<v8::Context> ctx = js::global::context.Get(js::global::isolate);
                v8::Local<v8::Value> result = js::exec(commandBuffer);
                if (result->IsString()) {
                    LogMessage(LogMessageType_Info, "=> '%s'", V8CStr(ctx, result).c_str());
                } else {
                    LogMessage(LogMessageType_Info, "=> %s", V8CStr(ctx, result).c_str());
                }
                commandBuffer[0] = 0;
            }
        }
        lastActive = active;
#endif
        commandLineHeight = ImGui::GetCursorPosY() - startPos;
        ImGui::PopID();

        ImGui::PopFont();
    }
    ImGui::End();
}