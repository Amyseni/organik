#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "UIElement.h"
#include "../Utils.h"
#include "BugReportWindow.h"
#include "YYToolkit/YYTK_Shared.hpp"

void BugReportWindow::Draw(bool& out_mousedOver, bool* p_open, const std::string &title)
{
    //mousedOver is passed by reference to all UI elements, so we never set it to false. only true if we are hovered.
    if (p_open == nullptr)
        return;
    if (!(*p_open))
        return;
    
    if (!ImGui::Begin("Bug Report", p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
        return;
    }
    
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
        out_mousedOver = true;
    }

    ImGui::Text("Please describe the bug you encountered:");
    ImGui::InputTextMultiline("##bug_report", &BugReportBuf, ImVec2(400, 200), ImGuiInputTextFlags_AllowTabInput);
    if (ImGui::Button("Submit"))
    {
        Utils::bugWebhook(BugReportBuf);
        BugReportBuf.clear();
        *p_open = false;
    }
    if (ImGui::Button("Cancel"))
    {
        *p_open = false;
    }
    ImGui::End();
}