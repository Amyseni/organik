#pragma once
#include "Synthetik.h"
#include "zhl_internal.h"
#include <vector>
#include <unordered_map>
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include <functional>
#include <typeinfo>
#include <string>
#include <map>
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
static int g_chatOpenCloseDelay = 0;
namespace Organik
{
    struct UIElement;
    class UIManager;

    extern UIManager* g_UIManager;
    extern RValue* g_monoFonto;
    
    class UIManager
    {
    private:
        ImFont* consoleFont = nullptr;
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
        static bool isImGuiInitialized(bool set = false)
        {
            static bool isInitialized = false;
            if (!isInitialized)
                if (set)
                    isInitialized = true;
            
            return isInitialized;
        };
        static bool isFullyInitialized(bool set = false)
        {
            static bool firstFrameStarted = false;

            if (!firstFrameStarted)
                if (set)
                    firstFrameStarted = true;
            
            return firstFrameStarted;
        };
        static HWND g_hWnd;
        
        std::unordered_map<size_t, UIElement*> elements;
        std::vector<UIElement*> elementsToRemove;
        
        static std::map<std::string, std::tuple<std::string, std::function<Organik::UIElement*()>, std::function<Organik::UIElement*()>>> windowTypes;
        
        static RValue* GetFont();
        void DrawEvent();
        void StepEvent();
        bool isAnyItemHovered();
        void setHovered()
        {
            m_wasItemHoveredLastFrame = true;
        }
        static void Initialize();
        static void Shutdown();
        static size_t GetHash(const std::type_info& typeinfo)
        {
            return typeinfo.hash_code();
        }
        UIElement* GetElement(bool create = true, const std::string& name = "");
        ImFont* GetConsoleFont()
        {
            return consoleFont;
        }
        void SetConsoleFont(ImFont* font)
        {
            if (font)
            {
                consoleFont = font;
            }
        }
        
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