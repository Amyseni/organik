#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include <vector>
#include "Command.h"
#include <map>
#include "VariableViewer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

using namespace YYTK;
using namespace Aurie;

namespace Organik
{
    
    class OrganikConsole
    {
    public:
        std::string                 InputBuf; 
        std::string                 ObjInputBuf;
        ImVector<std::string*>       Items;
        ImVector<std::string*>       History;
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        void AddCommandHandler(Command* cmd);
        std::map<std::string, Command*>& Commands();

        OrganikConsole();
        ~OrganikConsole();

        void    ClearLog();

        void AddLog(const std::string& message);
        void AddLog(const char *fmt, ...) IM_FMTARGS(2);

        void    Draw(const std::string &title, bool* p_open);

        void    ExecCommand(const std::string &command_line);
        
        // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        static void ShowOrganikConsole(bool* p_open);
        
        int     TextEditCallback(ImGuiInputTextCallbackData* data);

        std::map<std::string, Command*> _commands;
    };
    struct UIElement
    {
        virtual void Step() = 0;
        virtual void Draw() = 0;
        virtual bool IsCollidingWithMouse() = 0;
        virtual ~UIElement() = default;
        
        inline const std::string& GetName() const { return _name; }
        inline bool IsVisible() const { return _visible; }
        inline void SetVisible(bool visible) { _visible = visible; }
        
    protected:
        std::string _name = "UIElement";
        std::vector<UIElement*> _children = {};
        vector2 _position = {0, 0};
        bbox _bbox = {0, 0, 0, 0};
        RValue _index;
        bool _visible = true;
    };

    
    class UIManager
    {
    public:
        static UIManager* GetInstance();
        static OrganikConsole* GetConsole();
        static RValue* GetFont();
        static int GetMenuState();
        
        void DrawEvent();
        void StepEvent();
        void ToggleConsole();
        RValue Add(UIElement* target);
        UIElement* Find(bool(*pred)(const UIElement&));
        int FindIndex(bool(*pred)(const UIElement&));
        std::vector<UIElement*> FindAll(bool(*pred)(const UIElement&));
        bool Remove(int index);
        bool Remove(bool(*pred)(const UIElement&));
        
        static void Initialize();
        static void Shutdown();
        bool showConsole = true;
        bool showVariableViewer = false;
        bool showPropertyEditor = true;
        
    private:
        std::vector<UIElement*> _elements = {};
        UIManager() = default;
        ~UIManager();
    };
    extern int g_DevMenuState;
    extern UIManager* g_UIManager;
    extern OrganikConsole* console;
    extern RValue* g_monoFonto;
}