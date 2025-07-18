#pragma once
#include "../ModuleMain.h"
#include <string>
#include "UIElement.h"
#include <vector>
#include "YYToolkit/YYTK_Shared.hpp"

namespace Organik
{
    class BugReportWindow : public UIElement
    {
    public:
        BugReportWindow(const std::string& name = "Bug Report") : UIElement(name) {};
        ~BugReportWindow() = default;
        std::string BugReportBuf;

        void Draw(bool& out_mousedOver, bool* p_open = NULL, const std::string &title = "");

    };

}