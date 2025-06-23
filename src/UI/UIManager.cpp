#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include "UIManager.h"
#include "DevConsole.h"

using namespace Organik;

namespace Organik 
{
    UIManager* g_UIManager = nullptr;
    RValue* g_monoFonto = nullptr;
    UIElement *g_devConsole = nullptr;

    UIManager* UIManager::GetInstance()
    {
        if (!g_UIManager)
        {
            g_UIManager = new UIManager();
        }
        return g_UIManager;
    }
    UIElement* UIManager::GetDevConsole()
    {
        if (!g_devConsole)
        {
            g_devConsole = new DevConsole(15, 30, 800, 600);
            g_UIManager->Add(g_devConsole);
        }
        return g_devConsole;
    }
    RValue *UIManager::GetFont()
    {
        if (!g_monoFonto)
        {
            RValue fontTemp = g_ModuleInterface->CallBuiltin("font_add", {RValue("Inconsolata.ttf")});
            g_monoFonto = &fontTemp;
        }
        return g_monoFonto;
    }
    
    bool UIManager::HasDevConsole()
    {
        return g_devConsole != nullptr;
    }

    UIManager::~UIManager()
    {
        for (auto* element : _elements)
        {
            delete element;
        }
        _elements.clear();
    }

    void UIManager::DrawEvent()
    {
        //Organik::GetLogger()->LogFormatted("DISPLAY THYME");
        for (UIElement* element : _elements) 
        {
            if (element && element->IsVisible()) 
            {
                element->Draw();
            }
        }
    }

    void UIManager::StepEvent()
    {

        CInstance* globalInstance = nullptr;

        g_ModuleInterface->GetGlobalInstance(&globalInstance);

        if (!globalInstance) {
            Organik::GetLogger()->LogFormatted("TextArea::Step - Failed to get global instance");
            return;
        }
        
        RValue lastKey = RValue(0);
        g_ModuleInterface->GetBuiltin("keyboard_lastkey", globalInstance, NULL_INDEX, lastKey);
        bool f12Pressed = (lastKey.ToInt32() == 123); // 123 = F12
        if (f12Pressed)
        {
            f12Pressed = f12Pressed && g_ModuleInterface->CallBuiltin("keyboard_check_pressed", {lastKey}).ToBoolean();
        }
        if (f12Pressed) 
        {
            g_ModuleInterface->CallBuiltin("keyboard_clear", {RValue(123)}); // Clear keyboard input
            Organik::GetLogger()->LogFormatted("EFF TWELVED");
            if (!g_devConsole)
            {
                GetDevConsole();
            }
            else
            {
                Organik::GetLogger()->LogFormatted("MAKE IT SEEN");
                GetDevConsole()->SetVisible(!GetDevConsole()->IsVisible());
            }
        }
        for (UIElement* element : _elements) 
        {
            if (element && element->IsVisible()) 
            {
                element->Step();
            }
        }
    }

    RValue UIManager::Add(UIElement* target)
    {
        _elements.push_back(target);
        return RValue(static_cast<int>(_elements.size() - 1));
    }

    UIElement* UIManager::Find(bool(*pred)(const UIElement&))
    {
        int i = 0;
        for (UIElement* element : _elements) 
        {
            if (element && pred(*element)) 
            {
                return element;
            }
        }
        return nullptr;
    }
    int UIManager::FindIndex(bool(*pred)(const UIElement&))
    {
        int i = 0;
        for (UIElement* element : _elements) 
        {
            if (element && pred(*element)) 
            {
                return i;
            }
            i = i + 1;
        }
        return -1;
    }

    std::vector<UIElement*> UIManager::FindAll(bool(*pred)(const UIElement&))
    {
        std::vector<UIElement*> results;
        for (UIElement* element : _elements) 
        {
            if (element && pred(*element)) 
            {
                results.push_back(element);
            }
        }
        return results;
    }

    bool UIManager::Remove(int index)
    {
        if (index < 0 || index >= static_cast<int>(_elements.size())) 
        {
            return false;
        }
        
        delete _elements[index];
        _elements.erase(_elements.begin() + index);
        return true;
    }
    bool UIManager::Remove(bool(*pred)(const UIElement&))
    {
        int index = -1;
        index = this->FindIndex(pred);
        if (index < 0 || index >= static_cast<int>(_elements.size())) 
        {
            return false;
        }
        
        delete _elements[index];
        _elements.erase(_elements.begin() + index);
        return true;
    }

    void UIManager::Initialize() 
    {
    }

    void UIManager::Shutdown() 
    {
        delete g_UIManager;
        if (g_monoFonto != nullptr)
        {
            g_ModuleInterface->CallBuiltin("font_delete", {*g_monoFonto});
            delete g_monoFonto;
            g_monoFonto = nullptr;
        }
        g_UIManager = nullptr;
    }
}