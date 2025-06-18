#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "UIManager.h"

using namespace YYTK;

namespace Organik
{
    enum ClickType 
    {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
        HOVERSTATECHANGE = 3,
        FORWARD = 4,
        BACK = 5
    };

    class Button : public UIElement
    {
    public:
        typedef void(*ButtonClickEvent)(Button* self);
        
        Button(const char* label, int x, int y, int width, int height);
        
        Button(const char* label, int x, int y, int width, int height, 
               std::map<ClickType, ButtonClickEvent> callbacks)
            : _label(label), _x(x), _y(y), _width(width), _height(height)
        {
            for(const auto& pair : callbacks) {
                if (pair.first >= 0 && pair.first < 6) {
                    _callbacks[pair.first] = pair.second;
                }
            }
        }
        
        void Step() override;
        void Draw() override;
        bool IsCollidingWithMouse() override;
        
        const char* GetLabel() const { return _label; }

        template <ClickType T>
        void SetCallback(ButtonClickEvent callback) { _callbacks[T] = callback; }

        // Helper method to set callbacks with runtime ClickType
        void SetCallback(ClickType type, ButtonClickEvent callback) 
        { 
            if (type >= 0 && type < 6) {
                _callbacks[type] = callback; 
            }
        }

    private:
        const char* _label;
        ButtonClickEvent _callbacks[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        int _x, _y, _width, _height;
        bool _wasHovered = false;
    };
}