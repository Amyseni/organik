#pragma once

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "imgui/imgui.h"
#include <vector>
#include <string>
#include <sstream>

using namespace YYTK;

struct InstanceVariableViewer
{
    int VisibleInstance = 0; // Currently visible instance
    
    void Draw(std::vector<CInstance*> instances);

private:
    void DisplayVariableValue(const std::string& name, RValue* value);
    std::string GetValuePreview(RValue* value);
    void DisplayArrayPopup(RValue *array);
    std::string editingVariable;
    int32_t editingInstanceId;
    RValue* editingValue = nullptr;
    RValue* editingValueArray = nullptr;
    static bool showPopup;
    static bool showArrayPopup;


    // Edit value buffers
    double editDoubleValue = 0.0;
    int32_t editIntValue = 0;
    std::string editStringValue;
    bool editBoolValue = false;

    void PrepareEditValue(RValue* value);
    void DisplayArrayElement(size_t index, RValue* element, std::string name = "");
    void DisplayEditPopup();
};

// Function to show the variable viewer window
void ShowVariableViewer(bool* p_open, std::vector<CInstance*> instances);