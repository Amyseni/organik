#pragma once

#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "imgui/imgui.h"
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
        CInstance* VisibleInstance = 0; // Currently visible instance
        InstanceVariableViewer(std::string name = "Instance Variable Viewer") : UIElement(name) {};
        ~InstanceVariableViewer() = default;
        void Draw(bool& out_mousedOver, bool* p_open = NULL, const std::string &title = "");
        int32_t GetHash() override {
            static int32_t hash = typeid(InstanceVariableViewer).hash_code();
            return hash;
        }
        void GetName(char* buf) override
        {
            sprintf(buf, "Instance Variable Viewer (%p)", reinterpret_cast<void*>(VisibleInstance));
        }
        void DrawObjectParentsFilterList();
        void DrawObjectList();
        void DisplayVarTableEntry(CInstance* instance, const std::string& memberName, RValue* memberValue);

    private:
        void DrawInner();
        void DisplayVariableValue(const std::string& name, RValue* value);
        std::string GetValuePreview(RValue* value);
        std::string editingVariable;
        int32_t editingInstanceId;
        RValue* editingValue = nullptr;
        RValue* editingValueArray = nullptr;
        bool rowClicked = false;
        static bool showPopup;
        static bool showArrayPopup;
        static bool matchNone;
        static bool matchAny;        
        static bool matchAll;

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