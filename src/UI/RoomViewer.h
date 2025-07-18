#pragma once
#include <string>
#include <vector>
#include "UIElement.h"
#include "YYToolkit/YYTK_Shared.hpp"

namespace Organik
{
    struct RoomViewer : public UIElement
    {
    public:
        RoomViewer(const std::string& name = "Room Viewer") : UIElement(name) {};
        ~RoomViewer() = default;

        void Draw(bool& out_mousedOver, bool* p_open = NULL, const std::string &title = "");

    };
}
