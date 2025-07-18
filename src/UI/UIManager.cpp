#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include "UIManager.h"
#include "UIElement.h"
#include "RoomViewer.h"
#include "BugReportWindow.h"
#include "VariableViewer.h"
#include "BuiltinCommands.h"
#include "MainMenu.h"
#include "OrganikConsole.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "../Utils.h"

using namespace Organik;

namespace Organik 
{
    UIManager* g_UIManager = nullptr;
    RValue* g_monoFonto = nullptr;
    
    static int frameCounter = 0;
    std::map<std::string, std::tuple<std::string, std::function<Organik::UIElement*()>, std::function<Organik::UIElement*()>>> UIManager::windowTypes;

    UIManager* UIManager::GetInstance()
    {
        if (!g_UIManager)
        {
            g_UIManager = new UIManager();
            UIManager::windowTypes.insert({
                WINDOW_TYPE(RoomViewer, "Ctrl+F10"),
                WINDOW_TYPE(BugReportWindow, "Ctrl+Alt+C"),
                WINDOW_TYPE(InstanceVariableViewer, "Ctrl+F11"),
                WINDOW_TYPE(OrganikConsole, "Ctrl+Alt+T"),
            });
            g_UIManager->AddElement<MainMenu>("Main Menu");
        }
        return g_UIManager;
    }

    void UIManager::DrawEvent()
    {
        bool mousedOver = false;
        std::vector<size_t> toDelete;
        
        for (const auto& [hash, element] : elements)
        {
            if (element)
            {
                bool wantsToStayOpen = true;
                element->Draw(mousedOver, &wantsToStayOpen, element->name);
                if (!wantsToStayOpen)
                {
                    toDelete.push_back(hash);
                }
            }
        }
        if (mousedOver)
        {
            m_wasItemHoveredLastFrame = true;
        }
        else
        {
            m_wasItemHoveredLastFrame = false;
        }
        
        for (size_t elementHash : toDelete)
        {
            delete elements[elementHash];
            elements.erase(elementHash);
        }
    }

    bool UIManager::isAnyItemHovered()
    {
        return m_wasItemHoveredLastFrame;
    }

    void UIManager::Initialize() 
    {
        g_UIManager = GetInstance();
    }

    UIManager::~UIManager()
    {
        for (const auto& [hash, element] : elements)
        {
            delete element;
        }
        elements.clear();
    }

    void UIManager::StepEvent()
    {
        
    }
    
    template OrganikConsole* UIManager::GetElement<OrganikConsole>(bool, const std::string&);
    template OrganikConsole* UIManager::AddElement<OrganikConsole>(const std::string&);
}