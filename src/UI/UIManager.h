#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"
#include <vector>

using namespace YYTK;
using namespace Aurie;

namespace Organik
{
    struct UIElement
    {
        virtual void Step() = 0;
        virtual void Draw() = 0;
        virtual bool IsCollidingWithMouse() = 0;
        virtual ~UIElement() = default;
        
        inline bool IsVisible() const { return _visible; }
        inline void SetVisible(bool visible) { _visible = visible; }
        
    protected:
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
        
        void DrawEvent();
        void StepEvent();
        RValue Add(UIElement* target);
        UIElement* Find(bool(*pred)(const UIElement&));
        int FindIndex(bool(*pred)(const UIElement&));
        std::vector<UIElement*> FindAll(bool(*pred)(const UIElement&));
        bool Remove(int index);
        bool Remove(bool(*pred)(const UIElement&));
        
        static void Initialize();
        static void Shutdown();
        
    private:
        std::vector<UIElement*> _elements = {};
        UIManager() = default;
        ~UIManager();
    };

    extern UIManager* g_UIManager;
}