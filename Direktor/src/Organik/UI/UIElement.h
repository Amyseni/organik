#pragma once
#include <string>

namespace Organik
{
    struct UIElement
    {
        const std::string name;

        UIElement(const std::string& name) : name(name) {};
        UIElement(const char* name) : name(std::string(name)) {};
        virtual ~UIElement() = default;
        virtual void Draw(bool& out_mousedOver, bool* p_open = nullptr, const std::string &title = "") {};
    };
}