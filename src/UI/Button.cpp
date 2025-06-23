#include "Button.h"
#include "UIManager.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "../ModuleMain.h"

using namespace YYTK;

namespace Organik
{
    Button::Button(const char* label, int x, int y, int width, int height) 
        : _label(label), _x(x), _y(y), _width(width), _height(height)
    {
        char _nameBuf[32];
        unsigned int _nameIndex = x;
        _nameIndex = (_nameIndex ^ y) << 2;
        _nameIndex = (_nameIndex ^ width) << 2;
        _nameIndex = (_nameIndex ^ height);
        _itoa_s(_nameIndex, _nameBuf, 10);
        _name = std::string("Button_");
        _name.append(_nameBuf);
        Organik::GetLogger()->LogFormatted("Initializing TextArea with name: %s", _name.c_str());
        _bbox = {x, y, 
                width, height};
    }

    void Button::Step() 
    {
        if (!g_ModuleInterface) return;

        CInstance *globalInstance = nullptr;
        g_ModuleInterface->GetGlobalInstance(&globalInstance);
        if (!globalInstance) return;

        RValue lastMouseButton = RValue(0);
        g_ModuleInterface->GetBuiltin("mouse_lastbutton", globalInstance, NULL_INDEX, lastMouseButton);
        RValue isNewButtonPress = g_ModuleInterface->CallBuiltin("mouse_check_button_pressed", {lastMouseButton});
        if (!(isNewButtonPress.ToBoolean()))
            {return;}
        
        // Get mouse position and button states
        RValue mouseX = RValue(0);
        RValue mouseY = RValue(0);
        RValue mb_side1 = RValue(5);
        RValue mb_side2 = RValue(6);
        RValue mb_any = RValue(0);
        
        g_ModuleInterface->GetBuiltin("mouse_x", globalInstance, NULL_INDEX, mouseX);
        g_ModuleInterface->GetBuiltin("mouse_y", globalInstance, NULL_INDEX, mouseY);
        
        // g_ModuleInterface->GetBuiltin("mb_side1", globalInstance, NULL_INDEX, mb_side1); -- doesn't work
        // g_ModuleInterface->GetBuiltin("mb_side2", globalInstance, NULL_INDEX, mb_side2); -- doesn't work
        g_ModuleInterface->GetBuiltin("mb_any", globalInstance, NULL_INDEX, mb_any);
        
        const int m_frwrd = mb_side1.ToInt32();
        const int m_back = mb_side2.ToInt32();

        // Check if mouse is over the button
        bool isHovered = (mouseX.ToInt32() >= _x && mouseX.ToInt32() <= _x + _width &&
                         mouseY.ToInt32() >= _y && mouseY.ToInt32() <= _y + _height);

        // Handle hover state change
        if (isHovered != _wasHovered) {
            _wasHovered = isHovered;
            if (_callbacks[HOVERSTATECHANGE]) {
                (_callbacks[HOVERSTATECHANGE])(this);
            }
        }
        
        // Only check button presses if mouse is over the button
        if (isHovered && isNewButtonPress.ToBoolean()) {
//            GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
            switch(lastMouseButton.ToInt32()) {
                case 1:
                case 2:
                case 3:
                 // Left, right, middle
//                    GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
                    if (_callbacks[lastMouseButton.ToInt32() - 1]) {
                        (_callbacks[lastMouseButton.ToInt32() - 1])(this);
                    }
                    break;
                case 5:
//                    GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
                    (_callbacks[FORWARD])(this);
                    break;
                case 6:
//                    GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
                    (_callbacks[BACK])(this);
                    break;
                default:
//                    GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
                    g_ModuleInterface->CallBuiltin("mouse_clear", {mb_any});
                    break;
            }    
        }
//        GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
            
            
        if (g_ModuleInterface->CallBuiltin("mouse_check_button", {RValue(5)}).ToBoolean()) {
            if (_callbacks[FORWARD]) {
                (_callbacks[FORWARD])(this);
            }
        }
//        GetLogger()->LogFormatted("FN %s: LINE %d", __FUNCTION__, __LINE__);
        
        if (g_ModuleInterface->CallBuiltin("mouse_check_button", {RValue(6)}).ToBoolean()) {
            if (_callbacks[BACK]) {
                (_callbacks[BACK])(this);
            }
        }
    }

    void Button::Draw() 
    {
        if (!g_ModuleInterface) return;

        // Save current drawing state
        RValue alpha_old = g_ModuleInterface->CallBuiltin("draw_get_alpha", {});
        RValue color_old = g_ModuleInterface->CallBuiltin("draw_get_colour", {});
        
        // Set drawing properties
        g_ModuleInterface->CallBuiltin("draw_set_alpha", {RValue(1.0)});
        
        // Change color based on hover state
        RValue color1 = _wasHovered ? RValue(0x00ff00) : RValue(0xff0000);
        RValue color2 = _wasHovered ? RValue(0x0000ff) : RValue(0x00ff00);
        
        // Draw button rectangle
        g_ModuleInterface->CallBuiltin("draw_roundrect_colour", {
            RValue(_x), 
            RValue(_y),
            RValue(_x + _width), 
            RValue(_y + _height),
            color1, 
            color2,
            RValue(false)
        });
        
        // Draw button text (if label exists)
        if (_label) {
            g_ModuleInterface->CallBuiltin("draw_set_colour", {RValue(0xFFFFFF)});
            g_ModuleInterface->CallBuiltin("draw_text", {
                RValue(_x + _width/2.0),
                RValue(_y + _height/2.0),
                RValue(_label)
            });
        }
        
        // Restore drawing state
        g_ModuleInterface->CallBuiltin("draw_set_alpha", {alpha_old});
        g_ModuleInterface->CallBuiltin("draw_set_colour", {color_old});
    }

    bool Button::IsCollidingWithMouse() 
    {
        if (!g_ModuleInterface) return false;
        
        RValue mouseX = g_ModuleInterface->CallBuiltin("mouse_x", {});
        RValue mouseY = g_ModuleInterface->CallBuiltin("mouse_y", {});
        
        return (mouseX.ToInt32() >= _x && mouseX.ToInt32() <= _x + _width &&
                mouseY.ToInt32() >= _y && mouseY.ToInt32() <= _y + _height);
    }
}