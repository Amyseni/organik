#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "UIManager.h"
#include <string>
#include <vector>
#define vk_enter RValue(13)


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
        ~TextArea() override;
        void Step() override;
        void Draw() override;
        bool IsCollidingWithMouse() override;

        // Getters/Setters
        std::string GetText() { return _text; }
        void SetText(const std::string& text);
        void AppendText(const std::string& text);
        void ClearText();
        
        // Configuration
        void SetEditable(bool editable) { _editable = editable; }
        bool IsEditable() const { return _editable; }
        void SetMultiline(bool multiline) { _isMultiline = multiline; }
        bool IsMultiline() const { return _isMultiline; }
        void SetWordWrap(bool wordWrap) { _wordWrap = wordWrap; }
        bool HasWordWrap() const { return _wordWrap; }
        
        // For editable fields - gets current keyboard input
        void ClearKeyboardInput();
        
    private:
        void WrapText();
        void UpdateTextLines();
        
        bool _hasOutline;
        bool _editable;
        bool _isMultiline;
        bool _wordWrap;
        RValue _font;
        RValue _fontSize;
        RValue _textColor;
        RValue _fillColor;
        RValue _borderColor;
        RValue _alpha;
        
        int _width;
        int _height;
        int _x;
        int _y;
        
        std::string _text = std::string("");
        std::string _lastText = std::string("");
        std::vector<std::string> _lines = std::vector<std::string>();
        int _scrollOffset;
        unsigned int _maxVisibleLines;
    };
}
