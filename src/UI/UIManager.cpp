#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include "UIManager.h"

using namespace Organik;

namespace Organik 
{
    UIManager* g_UIManager = nullptr;

    UIManager* UIManager::GetInstance()
    {
        if (!g_UIManager)
        {
            g_UIManager = new UIManager();
        }
        return g_UIManager;
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
        // Initialization code for the UI manager
    }

    void UIManager::Shutdown() 
    {
        delete g_UIManager;
        g_UIManager = nullptr;
    }
}