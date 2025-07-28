#pragma once

#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "imgui/imgui.h"
#include "UIElement.h"
#include <vector>
#include <string>
#include <sstream>

namespace Organik
{
    struct InstanceVariableViewer : public UIElement
    {
        enum {
            SomeValue = 0,
            AnotherValue = 1,
            YetAnotherValue = 2,
        };
        int VisibleInstance = 0; // Currently visible instance
        InstanceVariableViewer(std::string name = "Instance Variable Viewer") : UIElement(name) {};
        ~InstanceVariableViewer() = default;
        void Draw(bool& out_mousedOver, bool* p_open = NULL, const std::string &title = "");

    private:
        void DrawInner(std::vector<CInstance*> instances = {});
        void DisplayVariableValue(const std::string& name, RValue* value);
        std::string GetValuePreview(RValue* value);
        void DisplayArrayPopup(RValue *array);
        std::string editingVariable;
        int32_t editingInstanceId;
        RValue* editingValue = nullptr;
        RValue* editingValueArray = nullptr;
        bool rowClicked = false;
        static bool showPopup;
        static bool showArrayPopup;


        // Edit value buffers
        double editDoubleValue = 0.0;
        int32_t editIntValue = 0;
        int64_t editLongLongValue = 0;
        std::string editStringValue;
        bool editBoolValue = false;

        void PrepareEditValue(RValue* value);
        void DisplayArrayElement(size_t index, RValue* element, std::string name = "");
        void DisplayEditPopup();
    };
}
