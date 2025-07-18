#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include <vector>
#include <unordered_map>
#include <functional>     // For std::function and std::hash
#include <typeinfo>
#include <string>
#include <map>           // Add this for std::map
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

using namespace YYTK;
using namespace Aurie;

#define WINDOW_TYPE(name, shortcut) \
    std::pair<std::string, std::tuple<std::string, std::function<Organik::UIElement*()>,std::function<Organik::UIElement*()>>> { #name, \
        std::make_tuple( \
            #shortcut, \
            []() -> Organik::UIElement* { \
                return UIManager::GetInstance()->AddElement<name>(); \
            }, \
            []() -> Organik::UIElement* { \
                return UIManager::GetInstance()->GetElement<name>(false); \
            } \
        ) \
    }

namespace Organik
{
    struct UIElement;
    class UIManager;

    extern UIManager* g_UIManager;
    extern RValue* g_monoFonto;
    
    class UIManager
    {
    public:
        template<typename T>
        requires std::is_base_of_v<UIElement, T>
        T* AddElement(const std::string& name = "")
        {
            UIManager* instance = GetInstance();
            size_t hash = GetHash(typeid(T));
            auto it = instance->elements.find(hash);
            if (it != instance->elements.end())
                return reinterpret_cast<T*>(it->second);

            T* element = new T();
            instance->elements[hash] = element;
            return element;
        }

        static UIManager* GetInstance();

        std::unordered_map<size_t, UIElement*> elements;
        std::vector<UIElement*> elementsToRemove;
        
        static std::map<std::string, std::tuple<std::string, std::function<Organik::UIElement*()>, std::function<Organik::UIElement*()>>> windowTypes;
        
        static RValue* GetFont();
        static int GetMenuState();
        void DrawEvent();
        void StepEvent();
        bool isAnyItemHovered();
        static void Initialize();
        static void Shutdown();
        static size_t GetHash(const std::type_info& typeinfo)
        {
            return typeinfo.hash_code();
        }
        UIElement* GetElement(bool create = true, const std::string& name = "");
        
        template<typename T>
        requires std::is_base_of_v<UIElement, T>
        T* GetElement(bool create = true, const std::string& name = "")
        {
            UIManager* instance = GetInstance();
            if (!create)
            {
                if (name.empty())
                {
                    const std::type_info& t_info = typeid(T);
                    auto it = instance->elements.find(GetHash(t_info));
                    if (it != instance->elements.end())
                        return reinterpret_cast<T*>(it->second);
                }
                return nullptr;
            }
            size_t hash = name.empty() ? GetHash(typeid(T)) : std::hash<std::string>()(name);
            auto it = instance->elements.find(hash);
            if (it != instance->elements.end())
                return reinterpret_cast<T*>(it->second);
            return AddElement<T>(name);
        }
        
    private:
        bool m_wasItemHoveredLastFrame = false;
        UIManager() = default;
        ~UIManager();
    };
}