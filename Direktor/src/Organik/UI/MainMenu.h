#include "../ModuleMain.h"
#include "UIElement.h"


namespace Organik
{
    struct MainMenu : public UIElement
    {
        public:
            MainMenu(const std::string& name = "Main Menu")
                : UIElement(name) {}
            ~MainMenu() = default;

            void Draw(bool& out_mousedOver, bool* p_open, const std::string &title);
            void DrawMenuBar();
    };
}