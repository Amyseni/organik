#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "VariableViewer.h"
#include "../ModuleMain.h"
#include "UIManager.h"
#include "BuiltinCommands.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "../Logging.h"
#include "../Utils.h"
#include "imgui/imgui_impl_dx11.h"

bool InstanceVariableViewer::showPopup = false;
bool InstanceVariableViewer::showArrayPopup = false;
void InstanceVariableViewer::Draw(std::vector<CInstance*> instances)
{
    // Main Body: draw variables
    if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
    {
        for (CInstance* instance : instances)
        {
            if (instance)
            {
                int32_t ID = instance->GetMembers().m_ID;
                int32_t objectID = instance->GetMembers().m_ObjectIndex;
                
                // Get the object name using object_get_name()
                std::string objectName = std::string(g_ModuleInterface->CallBuiltin("object_get_name", {RValue(objectID)}).ToString());
                if (objectName.contains("chunk"))
                {
                    // Skip chunks
                    continue;
                }
                // Create unique tree node ID using instance pointer
                ImGui::PushID(instance);
                if (ImGui::Selectable(
                    ("##instance_" + std::to_string((int)instance)).c_str(),
                    (VisibleInstance && VisibleInstance == instance->GetMembers().m_ID), 
                    ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap
                ))
                {
                    VisibleInstance = instance->GetMembers().m_ID;
                }
                ImGui::SameLine(0,0);
                ImGui::Text("#%d (%d vars) - %s", 
                    ID, 
                    instance->GetMemberCount(),
                    (objectName.substr(0, 
                        min(
                            objectName.length(), 
                            20
                        )
                    ) + (objectName.length() > 20 ? "..." : "")).c_str()
                );
                ImGui::Separator();
                ImGui::PopID();
            }
        }
    }
    ImGui::EndChild();

    // Below area: draw variable viewer for selected instance
    ImGui::SameLine();
    if (VisibleInstance)
    {
        CInstance* instance = Organik::Utils::GetInstanceFromID(VisibleInstance);
        if (!instance)
            return;
        if (ImGui::BeginChild("##variables", ImVec2(0, 0), ImGuiChildFlags_Borders))
        {
            // Get object name for header
            int32_t objectIndex = instance->GetMembers().m_ObjectIndex;
            RValue objectNameRVal = g_ModuleInterface->CallBuiltin("object_get_name", {RValue(objectIndex)});
            std::string objectName = objectNameRVal.ToString();
            ImGui::Text("Variables for: %s (ID: %d)", 
                       objectName.c_str(), 
                       instance->GetMembers().m_ID);
            ImGui::Separator();
            
            // Get all instance members using ToRefMap()
            RValue instanceRValue(instance);
            std::vector<std::string> memberNames = Utils::getInstanceVariableNames(instance);
            std::map<std::string, RValue*> memberMap;
            for (const std::string& name : memberNames)
            {
                if (!instance->ToRValue().ContainsValue(name))
                {
                    // Skip if the instance does not contain this member
                    continue;
                }
                RValue* member = instance->GetRefMember(name);
                if (!member || (member->m_Kind == VALUE_NULL || 
                    member->m_Kind == VALUE_UNDEFINED || 
                    member->m_Kind == VALUE_UNSET))
                {
                    // If member is null, skip it
                    continue;
                }                
                memberMap.insert({name, member});
            }
            
            // Display all variables in a table
            if (ImGui::BeginTable("Variables", 3, 
                                 ImGuiTableFlags_Borders | 
                                 ImGuiTableFlags_RowBg | 
                                 ImGuiTableFlags_Resizable |
                                 ImGuiTableFlags_ScrollY))
            {
                // Setup columns
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                for (const auto& [memberName, memberValue] : memberMap)
                {
                    if (memberValue)
                    {
                        ImGui::TableNextRow();
                        
                        // Variable name
                        ImGui::TableSetColumnIndex(0);
                        ImGui::PushID(memberName.c_str());
                        
                        // Check if this row is clicked
                        bool rowClicked = false;
                        if (ImGui::Selectable(("##row_" + memberName).c_str(), (editingInstanceId && editingValue == memberValue), 
                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ))
                        {
                            if (!(memberValue->m_Kind == VALUE_ARRAY))
                            {
                                rowClicked = true;
                            }
                        }
                        ImGui::SameLine(0,0);
                        ImGui::Text("%s", memberName.c_str());
                        
                        // Variable type
                        ImGui::TableSetColumnIndex(1);
                        std::string typeName = memberValue->GetKindName();
                        ImGui::Text("%s", typeName.c_str());
                        
                        // Variable value
                        ImGui::TableSetColumnIndex(2);
                        if (!(memberValue->m_Kind == VALUE_ARRAY))
                            DisplayVariableValue(memberName, memberValue);
                        else
                        {
                            std::vector<RValue*> editingValueRefArray = memberValue->ToRefVector();
                            
                            // Show array summary first
                            if (ImGui::TreeNode(("Array[" + std::to_string(editingValueRefArray.size()) + "]").c_str()))
                            {
                                // Create a nested table for array elements
                                if (ImGui::BeginTable("ArrayElements", 3, 
                                                     ImGuiTableFlags_Borders | 
                                                     ImGuiTableFlags_RowBg | 
                                                     ImGuiTableFlags_Resizable))
                                {
                                    // Setup columns for array elements
                                    ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                                    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                                    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableHeadersRow();
                                    
                                    for (size_t i = 0; i < editingValueRefArray.size(); ++i) 
                                    {
                                        ImGui::PushID(editingValueRefArray[i]);
                                        
                                        ImGui::TableNextRow();
                                        
                                        // Index column
                                        ImGui::TableSetColumnIndex(0);
                                        ImGui::Text("[%zu]", i);
                                        
                                        // Type column
                                        ImGui::TableSetColumnIndex(1);
                                        std::string typeName = editingValueRefArray[i]->GetKindName();
                                        ImGui::Text("%s", typeName.c_str());
                                        bool rowArrayClicked = false;
                                        // Value column
                                        ImGui::TableSetColumnIndex(2);
                                        
                                        if (ImGui::Selectable(("##array_elem_" + std::to_string(i)).c_str(), false, 
                                            ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
                                        {
                                            rowArrayClicked = true;
                                        }
                                        
                                        ImGui::SameLine(0, 0);
                                        DisplayVariableValue(("Array[" + std::to_string(i) + "]"), editingValueRefArray[i]);
                                        
                                        // Handle row click for array elements
                                        if (rowArrayClicked && editingValueRefArray[i]->m_Kind != VALUE_ARRAY)
                                        {
                                            editingVariable = memberName + "[" + std::to_string(i) + "]";
                                            editingValue = editingValueRefArray[i];
                                            editingInstanceId = instance->GetMembers().m_ID;
                                            rowClicked = true;
                                            showPopup = true;
                                            PrepareEditValue(editingValueRefArray[i]);
                                        }
                                        
                                        ImGui::PopID();
                                    }
                                    ImGui::EndTable();
                                }
                                ImGui::TreePop();
                            }
                        }
                        // Handle row click - open edit popup
                        if (rowClicked && !showPopup)
                        {
                            editingVariable = memberName;
                            editingValue = memberValue;
                            editingInstanceId = instance->GetMembers().m_ID;
                            
                            if (memberValue->m_Kind != VALUE_ARRAY)
                            {
                                editingInstanceId = instance->GetMembers().m_ID;
                                editingValue = memberValue;
                                editingVariable = memberName;
                                showPopup = true;
                                PrepareEditValue(memberValue);
                            }
                        }
                        if (rowClicked && showPopup)
                        {
                            ImGui::OpenPopup("VarEdit");
                        }
                        DisplayEditPopup();
                        ImGui::PopID();
                    }
                }
                ImGui::EndTable();
            }
                
        }
        ImGui::EndChild();
    }
    else
    {
        ImGui::TextDisabled("Select an instance to view its variables");
    }
}

void InstanceVariableViewer::PrepareEditValue(RValue* value)
{
    if (!value) return;
    Organik::GetLogger()->LogFormatted("%s:%d - Preparing edit value for %p", __FUNCTION__, __LINE__, value);
    RValueType kind = value->m_Kind;
    
    if (kind == VALUE_REAL)
    {
        editDoubleValue = value->ToDouble();
        Organik::GetLogger()->LogFormatted("%s:%d - double=%f", __FUNCTION__, __LINE__, editDoubleValue);
    }
    else if (kind == VALUE_INT32 || kind == VALUE_INT64)
    {
        editIntValue = value->ToInt32();
        Organik::GetLogger()->LogFormatted("%s:%d - int=%d", __FUNCTION__, __LINE__, editIntValue);
    }
    else if (kind == VALUE_STRING)
    {
        editStringValue = value->ToString();
        Organik::GetLogger()->LogFormatted("%s:%d - string=%s", __FUNCTION__, __LINE__, editStringValue.c_str());
    }
    else if (kind == VALUE_BOOL)
    {
        editBoolValue = value->ToBoolean();
        Organik::GetLogger()->LogFormatted("%s:%d - bool=%d", __FUNCTION__, __LINE__, editBoolValue);
    }
    else
    {
        editStringValue = value->ToString();
        Organik::GetLogger()->LogFormatted("%s:%d - unsupported type, name=%s", __FUNCTION__, __LINE__, value->GetKindName().c_str());
    }
}

void InstanceVariableViewer::DisplayEditPopup()
{
    if (ImGui::BeginPopup("VarEdit", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup ))
    {
        if (!editingValue)
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            showPopup = false;
            return;
        }
        if (!(editingVariable.size()))
            ImGui::Text("Editing: %s", editingVariable.c_str());
        ImGui::Text("Type: %s", editingValue->GetKindName().c_str());
        ImGui::Separator();
        
        RValueType kind = editingValue->m_Kind;
        bool valueChanged = false;
        
        if (kind == VALUE_REAL || kind == VALUE_INT32 || kind == VALUE_INT64)
        {
            if (kind == VALUE_INT32 || kind == VALUE_INT64)
            {
                if (ImGui::InputInt("Value", &editIntValue))
                {
                    *editingValue = RValue(static_cast<double>(editIntValue));
                    valueChanged = true;
                }
            }
            else
            {
                if (ImGui::InputDouble("Value", &editDoubleValue, 0.0, 0.0, "%.6f"))
                {
                    *editingValue = RValue(static_cast<double>(editDoubleValue));
                    valueChanged = true;
                }
            }
        }
        else if (kind == VALUE_STRING)
        {
            char buffer[1024];
            strncpy_s(buffer, editStringValue.c_str(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';
            
            if (ImGui::InputText("Value", buffer, sizeof(buffer)))
            {
                *editingValue = RValue(std::string(buffer));
                valueChanged = true;
            }
        }
        else if (kind == VALUE_BOOL)
        {
            if (ImGui::Checkbox("Value", &editBoolValue))
            {
                *editingValue = RValue(editBoolValue);
                valueChanged = true;
            }
        }
        else
        {
            ImGui::TextDisabled("This type cannot be edited");
        }
        
        ImGui::Separator();
        
        ImGui::Text("Press Enter to apply changes, or Cancel to discard.");
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
    else 
    {
        showPopup = false;
    }
}
void InstanceVariableViewer::DisplayArrayElement(size_t index, RValue* element, std::string name)
{
    if (!(element->m_Kind == VALUE_ARRAY))
    {
        DisplayVariableValue(("Array[" + std::to_string(index) + "]"), element);
    }
    else
    {
        // Nested array - show size and allow drilling down
        std::vector<RValue*> nestedArray = element->ToRefVector();
        if (nestedArray.empty())
        {
            ImGui::Text("Array[%zu] (empty)", nestedArray.size());
            return;
        }
        if (ImGui::TreeNode(element, "Array[%zu]", nestedArray.size()))
        {
            // Create nested table for sub-array
            if (ImGui::BeginTable("NestedArray", 3, 
                                 ImGuiTableFlags_Borders | 
                                 ImGuiTableFlags_RowBg))
            {
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                
                for (size_t j = 0; j < nestedArray.size(); ++j)
                {
                    ImGui::PushID(nestedArray[j]);
                    ImGui::TableNextRow();
                    
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("[%zu]", j);
                    
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", nestedArray[j]->GetKindName().c_str());
                    
                    ImGui::TableSetColumnIndex(2);
                    DisplayArrayElement(j, nestedArray[j]);
                    
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }
}
void InstanceVariableViewer::DisplayArrayPopup(RValue* array)
{
     // GetLogger()->LogFormatted("%s:%d - Displaying array popup", __FUNCTION__, __LINE__);
    if (ImGui::BeginPopup("View/Edit Array", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup))
    {
        if (!editingValueArray)
        {
             // GetLogger()->LogFormatted("%s:%d - No array selected", __FUNCTION__, __LINE__);
            ImGui::TextDisabled("No array selected");
            editingInstanceId = 0;
            ImGui::EndPopup();
            return;
        }
        std::vector<RValue*> editingValueRefArray = editingValueArray->ToRefVector();
         // GetLogger()->LogFormatted("%s:%d - Editing array of size %zu", __FUNCTION__, __LINE__, editingValueRefArray.size());        
        
           
        ImGui::EndTable();
        
        ImGui::Separator();
        
        ImGui::EndPopup();
    }
}

void InstanceVariableViewer::DisplayVariableValue(const std::string& name, RValue* value)
{
    if (!value) {
        return;
    }
    RValueType kind = value->m_Kind;
    
    if (kind == VALUE_REAL || kind == VALUE_INT32 || kind == VALUE_INT64)
    {
        double realVal = value->ToDouble();
        int32_t intVal = value->ToInt32();
        
        // Show both representations for numbers
        if (realVal == (double)intVal)
        {
            ImGui::Text("%d", intVal);
        }
        else
        {
            ImGui::Text("%.6f", realVal);
        }
    }
    else if (kind == VALUE_STRING)
    {
        std::string strVal = value->ToString();
        ImGui::Text("\"%s\"", strVal.c_str());
    }
    else if (kind == VALUE_BOOL)
    {
        bool boolVal = value->ToBoolean();
        ImGui::Text("%s", boolVal ? "true" : "false");
    }
    else if (kind == VALUE_ARRAY)
    {
        // Show array info
        std::vector<RValue> arrayValues = value->ToVector();
        ImGui::Text("Array[%zu]", arrayValues.size());
    }
    else if (kind == VALUE_PTR)
    {
        void* ptr = value->ToPointer();
        ImGui::Text("0x%p", ptr);
    }
    else if (kind == VALUE_OBJECT)
    {
        YYObjectBase* obj = value->ToObject();
        ImGui::Text("Object: 0x%p", obj);
    }
    else if (kind == VALUE_UNDEFINED || kind == VALUE_UNSET)
    {
        ImGui::TextDisabled("undefined");
    }
    else if (kind == VALUE_NULL)
    {
        ImGui::TextDisabled("null");
    }
    else
    {
        // For other types, try to get string representation
        std::string strVal = value->ToString();
        std::string kindName = value->GetKindName();
        ImGui::Text("%s (%s)", strVal.c_str(), kindName.c_str());
    }
}

std::string InstanceVariableViewer::GetValuePreview(RValue* value)
{
    if (!value) {
        return "null";
    }
    RValueType kind = value->m_Kind;
    
    if (kind == VALUE_REAL || kind == VALUE_INT32 || kind == VALUE_INT64)
    {
        return std::to_string(value->ToDouble());
    }
    else if (kind == VALUE_STRING)
    {
        std::string str = value->ToString();
        if (str.length() > 30)
        {
            return "\"" + str.substr(0, 30) + "...\"";
        }
        return "\"" + str + "\"";
    }
    else if (kind == VALUE_BOOL)
    {
        return value->ToBoolean() ? "true" : "false";
    }
    else if (kind == VALUE_ARRAY)
    {
        return "Array[" + std::to_string(value->ToVector().size()) + "]";
    }
    else if (kind == VALUE_PTR)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << value->ToPointer();
        return ss.str();
    }
    else if (kind == VALUE_OBJECT)
    {
        return "<Object>";
    }
    else if (kind == VALUE_UNDEFINED || kind == VALUE_UNSET)
    {
        return "<undefined>";
    }
    else if (kind == VALUE_NULL)
    {
        return "<null>";
    }
    else
    {
        return "<unknown:" + std::to_string((int)kind) + ">";
    }
}

// Demonstrate creating a simple property editor.
void ShowVariableViewer(bool* p_open, std::vector<CInstance*> instances)
{
    ImGui::SetNextWindowSize(ImVec2(430, 750), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Organik Variable Viewer", p_open))
    {
        ImGui::End();
        return;
    }
    static InstanceVariableViewer variable_viewer;
    variable_viewer.Draw(instances);
    ImGui::End();
}