#pragma once
#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "Command.h"
#include "UIElement.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

using namespace Organik;

namespace Organik
{
    struct OrganikConsole : public UIElement
    {
    public:
        std::string InputBuf; 
        std::string ObjInputBuf;
        ImVector<std::string*> Items;
        ImVector<std::string*> History;
        int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter Filter;
        bool AutoScroll;
        bool ScrollToBottom;
        void AddCommandHandler(Command* cmd);
        std::map<std::string, Command*>& Commands();

        OrganikConsole(const std::string& name = "Organik Console");
        ~OrganikConsole();

        void ClearLog();

        void AddLog(const std::string& message);
        void AddLog(const char *fmt, ...) IM_FMTARGS(2);

        void Draw(bool& out_mousedOver, bool* p_open, const std::string& title);

        void ExecCommand(const std::string &command_line);
        
        // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        static void ShowOrganikConsole(bool* p_open);
        
        int TextEditCallback(ImGuiInputTextCallbackData* data);

        std::map<std::string, Command*> _commands;
    };
    
    
}
