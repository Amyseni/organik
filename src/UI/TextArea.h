#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "UIManager.h"
#include <string>
#include <vector>

using namespace YYTK;

namespace Organik
{
    class TextArea : public UIElement
    {
    public:
        TextArea(int x, int y, int width, int height, 
                RValue alpha = RValue(0.75),
                RValue fillColor = RValue(0x202020), 
                RValue borderColor = RValue(0xAAAAAA), 
                RValue textColor = RValue(0xa4a4a4), 
                RValue fontSize = RValue(12.0));

        void Step() override;
        void Draw() override;
        bool IsCollidingWithMouse() override;

        // Getters/Setters
        const std::string& GetText() const { return _text; }
        void SetText(const std::string& text) { _text = text; }
        
    private:
        bool _hasOutline;
        bool _editable;
        bool _isMultiline;
        RValue _fontSize;
        RValue _textColor;
        RValue _fillColor;
        RValue _borderColor;
        RValue _alpha;
        
        int _width;
        int _height;
        int _x;
        int _y;
        
        std::string _text;
        char textBuffer[1024];
        std::vector<std::string> _lines;
        vector2 _cursorPosition;
    };
}
