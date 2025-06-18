#include "TextArea.h"

namespace Organik
{
    TextArea::TextArea(int x, int y, int width, int height, 
                      RValue alpha, RValue fillColor, RValue borderColor, 
                      RValue textColor, RValue fontSize)
        : _x(x), _y(y), _width(width), _height(height),
          _alpha(alpha), _fillColor(fillColor), _borderColor(borderColor),
          _textColor(textColor), _fontSize(fontSize)
    {
        _hasOutline = (fillColor.ToInt32() != borderColor.ToInt32());
        _editable = false;
        _isMultiline = (_height > _fontSize.ToInt32() * 2);
        _cursorPosition = {0, 0};
        
        // Initialize text buffer
        memset(textBuffer, 0, sizeof(textBuffer));
        
        // Set bounding box
        _bbox = {x, y, 
                width, height};
    }

    void TextArea::Step() 
    {
        // TODO: Implement input handling logic
    }

    void TextArea::Draw() 
    {
        // TODO: Implement drawing logic
        // This would typically involve:
        // 1. Drawing the background rectangle
        // 2. Drawing the border if hasOutline
        // 3. Drawing the text
    }

    bool TextArea::IsCollidingWithMouse() 
    {
        // TODO: Implement collision detection
        return false;
    }
}