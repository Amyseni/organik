#include "TextArea.h"
#include "../ModuleMain.h"

namespace Organik
{
    TextArea::TextArea(int x, int y, int width, int height, 
                      RValue alpha, RValue fillColor, RValue borderColor, 
                      RValue textColor, RValue fontSize)
        : _x(x), _y(y), _width(width), _height(height),
          _alpha(alpha), _fillColor(fillColor), _borderColor(borderColor),
          _textColor(textColor), _fontSize(fontSize), _scrollOffset(0)
    {
        char _nameBuf[32];
        unsigned int _nameIndex = x;
        _nameIndex = (_nameIndex ^ y) << 2;
        _nameIndex = (_nameIndex ^ width) << 2;
        _nameIndex = (_nameIndex ^ height);
        _itoa_s(_nameIndex, _nameBuf, 10);
        _name = std::string("TextArea_");
        _name.append(_nameBuf);
        Organik::GetLogger()->LogFormatted("Initializing TextArea with name: %s", _name.c_str());
        Organik::GetLogger()->LogFormatted("About to initialize TextArea at (%d, %d) size (%d, %d)", x, y, width, height);
        
        _hasOutline = (fillColor.ToInt32() != borderColor.ToInt32());
        _editable = false;
        _isMultiline = (height > fontSize.ToInt32() * 2);
        _wordWrap = _isMultiline; // Enable word wrap for multiline by default
        _font = g_ModuleInterface->CallBuiltin("font_add", {RValue("Inconsolata.ttf"), fontSize, RValue(false), RValue(false), RValue(32), RValue(128)});
        // Calculate max visible lines
        _maxVisibleLines = height / fontSize.ToInt32();
        if (_maxVisibleLines < 1) {
            _maxVisibleLines = 1;
        }
        
        // Set bounding box
        _bbox = {(x), (y), 
                (width), (height)};
        
        Organik::GetLogger()->LogFormatted("TextArea initialized successfully");
    }

    void TextArea::Step() 
    {
        if (!g_ModuleInterface) {
            Organik::GetLogger()->LogFormatted("TextArea::Step - g_ModuleInterface is null");
            return;
        }

        if (_editable) {
            // Organik::GetLogger()->LogFormatted("[EDITABLE FIELD STEP] %s @ %s : %d ", __FUNCTION__, __FILE__, __LINE__);
            // Handle keyboard input for editable fields
            CInstance* globalInstance = nullptr;
            g_ModuleInterface->GetGlobalInstance(&globalInstance);
            if (!globalInstance) {
                Organik::GetLogger()->LogFormatted("TextArea::Step - Failed to get global instance");
                return;
            }

            RValue lastKey = RValue(0);
            g_ModuleInterface->GetBuiltin("keyboard_lastkey", globalInstance, NULL_INDEX, lastKey);
            bool backspace = (lastKey.ToInt32() == vk_backspace.ToInt32()); 
            bool shiftPressed = g_ModuleInterface->CallBuiltin("keyboard_check", {vk_shift}).ToBoolean();
            bool asciiPressed = (lastKey.ToInt32() >= 32);
            if (asciiPressed || backspace)
            {
                bool keyWasPressed = g_ModuleInterface->CallBuiltin("keyboard_check_pressed", {lastKey}).ToBoolean();
                asciiPressed = asciiPressed && keyWasPressed;
                backspace = backspace && keyWasPressed;
            }
            if (backspace)
            {
                g_ModuleInterface->CallBuiltin("keyboard_clear", {lastKey}); // Clear keyboard input
                if (!_text.empty())
                    _text.pop_back();
            }
            else if (asciiPressed)
            {
                RValue lastChar = RValue(0);
                g_ModuleInterface->GetBuiltin("keyboard_lastchar", globalInstance, NULL_INDEX, lastChar);
                
                // it shouldn't be possible for this to exceed 255, but just in case
                Organik::GetLogger()->LogFormatted("Key pressed: %s", lastChar.ToCString()); 
                _text.push_back(lastChar.ToCString()[0]);
                g_ModuleInterface->CallBuiltin("keyboard_clear", {lastKey}); // Clear keyboard input
            }
        }
        else
        {
            // Organik::GetLogger()->LogFormatted("[NONEDITABLE FIELD STEP] %s @ %s : %d ", __FUNCTION__, __FILE__, __LINE__);
        }
    }

    void TextArea::Draw() 
    {
        if (!g_ModuleInterface) {
            Organik::GetLogger()->LogFormatted("TextArea::Draw - g_ModuleInterface is null");
            return;
        }
        CInstance* globalInstance = nullptr;
        g_ModuleInterface->GetGlobalInstance(&globalInstance);
        // Save current drawing state
        RValue alpha_old = g_ModuleInterface->CallBuiltin("draw_get_alpha", {});
        RValue color_old = g_ModuleInterface->CallBuiltin("draw_get_colour", {});
        RValue font_old = g_ModuleInterface->CallBuiltin("draw_get_font", {});
        RValue view_current;
        g_ModuleInterface->GetBuiltin("view_current", globalInstance, NULL_INDEX, view_current);
        
        int window_x = g_ModuleInterface->CallBuiltin("window_get_x", {}).ToInt32();
        int window_y = g_ModuleInterface->CallBuiltin("window_get_y", {}).ToInt32();
        int view_x = g_ModuleInterface->CallBuiltin("view_get_xport", {view_current}).ToInt32();
        int view_y = g_ModuleInterface->CallBuiltin("view_get_yport", {view_current}).ToInt32();
        // Draw background
        g_ModuleInterface->CallBuiltin("draw_set_alpha", {RValue(1.0)});
        g_ModuleInterface->CallBuiltin("draw_set_font", {_font});
        RValue fill = _fillColor;
        RValue border = _hasOutline ? _borderColor : _fillColor;
        
        g_ModuleInterface->CallBuiltin("draw_rectangle_colour", {
            RValue((window_x + view_x + _x)), 
            RValue((window_y + view_y + _y)),
            RValue((window_x + view_x + _x + _width)), 
            RValue((window_y + view_y + _y + _height)),
            _fillColor,
            _fillColor,
            _fillColor,
            _fillColor,
            RValue(false) // not outlined
        });
        
        // Draw border if needed
        if (_hasOutline) {
            g_ModuleInterface->CallBuiltin("draw_rectangle_colour", {
                RValue((window_x + view_x + _x)), 
                RValue((window_y + view_y + _y)),
                RValue((window_x + view_x + _x + _width)), 
                RValue((window_y + view_y + _y + _height)),
                _borderColor,
                _borderColor,
                _borderColor,
                _borderColor,
                RValue(true) // not outlined
            });
        }
        
        // Draw text
        g_ModuleInterface->CallBuiltin("draw_set_colour", {_textColor});
        g_ModuleInterface->CallBuiltin("draw_set_alpha", {RValue(1.0)});
        
        if (_isMultiline) {
            // Draw multiple lines with scrolling
            int lineY = window_y + view_y + _y + 5; // Small padding
            int startLine = _scrollOffset;
            int endLine = min(startLine + _maxVisibleLines, _lines.size());
            
            for (int i = startLine; i < endLine; i++) {
                if (!_lines[i].empty()) {
                    g_ModuleInterface->CallBuiltin("draw_text", {
                        RValue((window_x + view_x + _x + 5)), // Small left padding
                        RValue(lineY),
                        RValue(_lines[i].c_str())
                    });
                }
                lineY += _fontSize.ToInt32();
            }
        } else {
            // Single line - draw text or current keyboard input if editable
            
            if (!_text.empty()) {
                g_ModuleInterface->CallBuiltin("draw_text", {
                    RValue((window_x + view_x + _x + 5)), // Small left padding
                    RValue((window_y + view_y + _y + _height/2)),
                    RValue(_text)
                });
            }
        }
        
        // Restore drawing state
        g_ModuleInterface->CallBuiltin("draw_set_font", {font_old});
        g_ModuleInterface->CallBuiltin("draw_set_alpha", {alpha_old});
        g_ModuleInterface->CallBuiltin("draw_set_colour", {color_old});
    }

    bool TextArea::IsCollidingWithMouse() 
    {
        return false;
    }

    void TextArea::SetText(const std::string& text) 
    {
        _text = std::string(text);
        UpdateTextLines();
    }

    void TextArea::AppendText(const std::string& text) 
    {
        Organik::GetLogger()->LogFormatted("About to append to TextArea (length: %zu)", text.length());
        if (_editable)
        {
            Organik::GetLogger()->LogFormatted("[CALLED APPENDTEXT ON EDITABLE FIELD] %s @ %s : %d ", __FUNCTION__, __FILE__, __LINE__);
            return;
        }
        _text.append(text);

        UpdateTextLines();
        
        // Auto-scroll to bottom for multiline
        if (_isMultiline && (_lines.size() > _maxVisibleLines)) {
            _scrollOffset = _lines.size() - _maxVisibleLines;
        }
    }

    void TextArea::ClearText() 
    {
        _text = std::string("");
        _lines.clear();
        _scrollOffset = 0;
    }

    TextArea::~TextArea()
    {
        
        Organik::GetLogger()->LogFormatted("DevConsole destructor called");
        if(!g_ModuleInterface) {
            Organik::GetLogger()->LogFormatted("TextArea::~TextArea - g_ModuleInterface is null");
            return;
        }
        if (_font.ToInt32() != 0)
        {
            Organik::GetLogger()->LogFormatted("TextArea::~TextArea - Freeing font");
            g_ModuleInterface->CallBuiltin("font_delete", {_font});
        }
        ClearKeyboardInput();
    }

    void TextArea::ClearKeyboardInput() 
    {
        Organik::GetLogger()->LogFormatted("CLEARKEYBOARDINPUT CALLED CLEARKEYBOARDINPUT CALLED CLEARKEYBOARDINPUT CALLED");
        if (!g_ModuleInterface) {
            Organik::GetLogger()->LogFormatted("TextArea::ClearKeyboardInput - g_ModuleInterface is null");
            return;
        }
        
        CInstance* globalInstance = nullptr;
        g_ModuleInterface->GetGlobalInstance(&globalInstance);
        if (!globalInstance) {
            Organik::GetLogger()->LogFormatted("TextArea::ClearKeyboardInput - Failed to get global instance");
            return;
        }
        
        // Clear keyboard_string by setting it to empty
        RValue keyboardString = RValue("");
        g_ModuleInterface->GetBuiltin("keyboard_string", globalInstance, NULL_INDEX, keyboardString);
        keyboardString = RValue(""); // Set to empty string

    }

    void TextArea::UpdateTextLines() 
    {
        Organik::GetLogger()->LogFormatted("About to update TextArea lines for word wrap");
        
        _lines.clear();
        if (_editable)
        {
            Organik::GetLogger()->LogFormatted("UPDATETEXTLINES CALLED ON EDITABLE TEXTAREA");
        }
        if (!_isMultiline) {
            _lines.push_back(_text);
            return;
        }
        
        if (_wordWrap) {
            WrapText();
        } else {
            // Split by newlines only
            std::stringstream ss(_text);
            std::string line;
            while (std::getline(ss, line)) {
                _lines.push_back(line);
            }
        }
    }

    void TextArea::WrapText() 
    {
        if (!g_ModuleInterface) {
            Organik::GetLogger()->LogFormatted("TextArea::WrapText - g_ModuleInterface is null");
            return;
        }
        
        // Simple word wrapping - split text into lines that fit within width
        // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
        std::stringstream ss(_text);
        std::string line;
        
        while (std::getline(ss, line)) {
            // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
           if (line.empty()) {
                _lines.push_back("");
                continue;
            }
            
            // Get text width - placeholder for actual width calculation
            // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
            int textWidth = line.length() * (_fontSize.ToInt32() / 2); // yay monospaced fonts
            
            if (textWidth <= _width - 10) { // Account for padding
                // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                _lines.push_back(line.c_str());
            } else {
                // Need to wrap this line
                // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                std::string currentLine = "";
                std::istringstream words(line);
                std::string word;
                
                while (words >> word) {
                    // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                    std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
                    int testWidth = line.length() * (_fontSize.ToInt32() / 2);
                    // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                    
                    if (testWidth <= _width - 10) {
                        // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                        currentLine = testLine;
                    } else {
                        // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                        if (!currentLine.empty()) {
                            _lines.push_back(currentLine);
                        }
                        // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                        currentLine = word;
                    }
                }
                
                if (!currentLine.empty()) {
                    // Organik::GetLogger()->LogFormatted("Line %d", __LINE__);
                    _lines.push_back(currentLine);
                }
            }
        }
    }
}