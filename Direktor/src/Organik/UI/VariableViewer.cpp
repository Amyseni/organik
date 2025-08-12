#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include "VariableViewer.h"
#include "ItemFilter.h"
#include "UIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "../Logging.h"
#include "DefinitionHelpers/BuiltinHelper.h"
#include "../Utils.h"
#include "imgui/imgui_impl_dx11.h"

bool InstanceVariableViewer::showPopup = false;
bool InstanceVariableViewer::showArrayPopup = false;
bool matchAllVariables = false; // false = ay, true = all

std::map<int32_t, bool> variablesToMatchMap = std::map<int32_t, bool>();
std::map<int32_t, std::unordered_map<int32_t, std::string>> varMapCache;
std::string objectHasVariableFilter;
std::string variableNameFitler;
std::string variableTypeFilter;
#define VARHASH_FROMNAME(name) Organik::Variables::Hashes[Organik::Variables::##name]
#define VARNAME_FROMHASH(hash) Organik::Variables::HashToVariableMap[hash]
static bool dummyDoTheTHING = []() -> bool {
    // Initialize the variablesToMatchMap with all variables
    for (const auto& hash : Organik::Variables::Hashes) {
        variablesToMatchMap[hash] = false; // Default to not matching
    }
    return true;
}();
bool InstanceVariableViewer::matchAll = false;
bool InstanceVariableViewer::matchNone = false;
bool InstanceVariableViewer::matchAny = true;


static auto instanceFilter = ItemFilter();
void InstanceVariableViewer::DrawInner()
{
    // Main Body: draw variables
    static ImGuiTextFilter objectNameFilter;
    
    Organik::GetLogger()->LogFormatted("%s:%d Drawing %s", __FILE__, __LINE__, __FUNCTION__);
    if (ImGui::CollapsingHeader("Object Filters"))
    {
        Organik::GetLogger()->LogFormatted("%s:%d Object Filters %s", __FILE__, __LINE__, __FUNCTION__);
        if (ImGui::BeginChild("##object_filter", ImVec2(0.0f, 350.0f), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX))
        {
            ImGui::Text
            (
                "Filter objects. Confused? Try out the EZFilters below.\n"
                "All filters are partial matching, and case insensitive.\n"
                "Use - to invert the text filters (match WITHOUT only).\n"
                "As well as ',' to match EITHER/ANY from a list of terms.\n"
            );
            objectNameFilter.Draw("Object Name");
            ImGui::Text("Object Variables:\n"
                        "Click to select, hold Ctrl to select multiple.\n"
                        "Matches objects with a value set for selected variable(s).\n",
                        "See below.\n"
                    );

            
            static ImGuiTextFilter varNameFilter;
            varNameFilter.Draw("Variable Name");
            if (ImGui::BeginChild("##varnames_list", ImVec2(0.0f, 250.0f), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Borders))
            {
                std::for_each(Variables::HashToVariableMap.begin(), Variables::HashToVariableMap.end(),
                    [&](std::pair<int32_t, const char*> pair) {
                        if (instanceFilter.variableFilters.contains(pair.first))
                            return; // Skip if already filtered
                        if (!varNameFilter.PassFilter(pair.second))
                            return;
                        
                        if (ImGui::Selectable(pair.second, variablesToMatchMap.contains(pair.first) ? variablesToMatchMap[pair.first] : false, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
                        {
                            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                            {
                                instanceFilter.setVariableFilter(pair.first, true);
                            }
                        }
                    });
            }
            ImGui::EndChild();
            ImGui::SameLine();

            if (ImGui::BeginChild("##varnames_activelist", ImVec2(0.0f, 250.0f), ImGuiChildFlags_Borders))
            {
                std::for_each(Variables::HashToVariableMap.begin(), Variables::HashToVariableMap.end(),
                    [&](std::pair<int32_t, const char*> pair) {
                        if (!instanceFilter.variableFilters.contains(pair.first))
                            return;

                        if (ImGui::Selectable(pair.second, variablesToMatchMap.contains(pair.first) ? variablesToMatchMap[pair.first] : false, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
                        {
                            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                            {
                                instanceFilter.setVariableFilter(pair.first, false);
                            }
                        }
                    }
                );
            }
        }
        ImGui::EndChild();
    }
    (*Run_Room)->UpdateActive();
    instanceFilter.updateTextFilter(objectNameFilter.InputBuf);
	const auto& instanceMap = instanceFilter.FilterInstances();
    if (ImGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
    {
        std::for_each(instanceMap.begin(), instanceMap.end(), [&](std::pair<int32_t, std::vector<CInstance*>> pair) {
            int32_t objectID = pair.first;
            RValue objectNameRVal = RValue(pair.second.at(0)->m_Object->m_Name);
            //DoBuiltinRef(&gml_object_get_name, &objectNameRVal, {RValue(objectID)});
            std::string objectName = objectNameRVal.ToString();

            if (ImGui::TreeNode(objectName.c_str(), "%s (%d)", objectName.c_str(), static_cast<int>(pair.second.size())))
            {
                for (CInstance* instance : pair.second) 
                {
                    int32_t ID = instance->m_ID;
                    ImGui::PushID(instance);
                    if (ImGui::Selectable(
                        ("##instance_" + std::to_string((int)instance)).c_str(),
                        (VisibleInstance && VisibleInstance == instance), 
                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap
                    ))
                    {
                        VisibleInstance = instance;
                    }
                    ImGui::SameLine(0,0);
                    ImGui::Text("#%d (%d vars)", 
                        ID, 
                        instance->m_VariableCount
                    );
                    ImGui::Separator();
                    ImGui::PopID();
                }  
                ImGui::TreePop();
            }
        });
        // for (CInstance* instance : instances)
        // {
	    //     if (instance)
        //     {
	    //         int32_t ID = instance->m_ID;
        //         int32_t objectID = instance->m_ObjectIndex;
                
        //         // Get the object name using object_get_name()
        //         RValue objectNameRVal;
        //         DoBuiltinRef(&gml_object_get_name, objectNameRVal, {RValue(objectID)});
        //         std::string objectName = objectNameRVal.ToString();
        //         // Create unique tree node ID using instance pointer
        //         ImGui::PushID(instance);
	    //         if (ImGui::Selectable(
        //             ("##instance_" + std::to_string((int)instance)).c_str(),
        //             (VisibleInstance && VisibleInstance == instance->m_ID), 
        //             ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap
        //         ))
        //         {
        //             VisibleInstance = instance->m_ID;
        //         }
	    //         ImGui::SameLine(0,0);
        //         ImGui::Text("#%d (%d vars) - %s", 
        //             ID, 
        //             instance->GetMemberCount(),
        //             (objectName.substr(0, 
        //                 min(
        //                     objectName.length(), 
        //                     20
        //                 )
        //             ) + (objectName.length() > 20 ? "..." : "")).c_str()
        //         );
        //         ImGui::Separator();
        //         ImGui::PopID();
        //     }
        // }
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    CInstance* instance;
    if (instance = VisibleInstance)
    {
        // basically both of these checks should be unreachable code, but you know how I get down.
	    if (!instance)
            return;
        int id = instance->m_ID;

        if (!CInstance::FirstOrDefault([&](CInstance* inst) -> bool {
            return inst == VisibleInstance;
        }))
        {
            ImGui::Text("Instance with ID %d does not exist or is inactive.", VisibleInstance->m_ID);
            return;
        }
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        if (ImGui::BeginChild("##variables", ImVec2(0, 0), ImGuiChildFlags_Borders))
        {
            // Get object name for header
            int32_t objectIndex = instance->m_ObjectIndex;
            RValue objectNameRVal;
            DoBuiltinRef(&gml_object_get_name, &objectNameRVal, {RValue(objectIndex)});
	        Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            std::string objectName = objectNameRVal.ToString();
            ImGui::Text("Variables for: %s (ID: %d)", 
                       objectName.c_str(), 
                       id);
            ImGui::Separator();
            
	        Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            std::map<std::string, RValue*> memberMap;
            if (!varMapCache.contains(id) || (instance->GetMemberCount() != varMapCache[id].size()))
            {
                RValue refResult;
                constexpr int szInst = sizeof(CInstance);
                GetLogger()->LogFormatted("Caching variable map for instance ID %d", id);
                auto varmap = Organik::Utils::GetVariableMap(instance);
                // GetLogger()->LogFormatted("Returnd: %s: %d", refResult.GetKindName(), refResult.GetArrayLength());
                //std::vector<RValue*> refVector = refResult.ToRefVector(); 
                std::vector<std::string> memberNames;
                // for (RValue *memberNameRVal : refVector)
                // {
                //     Organik::GetLogger()->LogFormatted("%d: %s", id, memberNameRVal->ToCString());
                //     if (memberNameRVal->m_Kind == VALUE_STRING)
                //     {
                //         memberNames.push_back(memberNameRVal->ToString());
                //     }
                // }
                varMapCache.insert_or_assign(id, varmap);
                // for (const std::string& name : memberNames)
                // {
                //     int32_t slot = Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(name.c_str()));
                //     if (!slot || slot < 0)
                //     {
                //         continue;
                //     }
	            //     Organik::GetLogger()->LogFormatted("%s:%d --- %s %s", __FILE__, __LINE__, name.c_str(), __FUNCTION__);
                //     RValue* member = instance->InternalReadYYVar(slot);
                //     if (!member || (member->m_Kind == VALUE_NULL || 
                //         member->m_Kind == VALUE_UNDEFINED || 
                //         member->m_Kind == VALUE_UNSET))
                //     {
                //         // If member is null, skip it
                //         continue;
                //     }
                //     memberMap.insert({name, member});
	            //     Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                //     varMapCache[id].insert({slot, name});
                // }
            }
            else
            {
                Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                GetLogger()->LogFormatted("Using cached varmap for instance ID %d", id);
                for (const auto& [slot, name] : varMapCache[id])
                {
	                Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                    RValue* member = instance->InternalReadYYVar(slot);
                    if (!member || (member->m_Kind == VALUE_NULL || 
                        member->m_Kind == VALUE_UNDEFINED || 
                        member->m_Kind == VALUE_UNSET))
                    {
                        // If member is null, skip it
                        continue;
                    }
                    memberMap.insert({name, member});
                }
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
	            Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
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
                        rowClicked = false;
                        if (ImGui::Selectable(("##row_" + memberName).c_str(), (editingInstanceId && editingValue == memberValue), 
                            ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ))
                        {
                            if (!(memberValue->m_Kind == VALUE_ARRAY))
                            {
                                rowClicked = true;
                            }
                        }
	                    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                        ImGui::SameLine(0,0);
                        ImGui::Text("%s", memberName.c_str());
                        
                        // Variable type
                        ImGui::TableSetColumnIndex(1);
                        std::string typeName = memberValue->GetKindName();
                        ImGui::Text("%s", typeName.c_str());
                        
                        // Variable value
                        ImGui::TableSetColumnIndex(2);
                        
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                        DisplayVariableValue(memberName, memberValue);
                        // Handle row click - open edit popup
                        if (rowClicked && !showPopup)
                        {
                            editingVariable = memberName;
                            editingValue = memberValue;
                            editingInstanceId = instance->m_ID;
                            
                            if (memberValue->m_Kind != VALUE_ARRAY)
                            {
                                editingInstanceId = instance->m_ID;
                                editingValue = memberValue;
                                editingVariable = memberName;
                                showPopup = true;
                                PrepareEditValue(memberValue);
                            }
                        }
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
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
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        ImGui::TextDisabled("Select an instance to view its variables");
    }
}

void InstanceVariableViewer::PrepareEditValue(RValue* value)
{
    if (!value) return;
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
    Organik::GetLogger()->LogFormatted("%s:%d - Preparing edit value for %p", __FUNCTION__, __LINE__, value);
    RValueType kind = (RValueType) value->m_Kind;
    
    if (kind == VALUE_REAL)
    {
        editDoubleValue = REAL_RValue(value);
        Organik::GetLogger()->LogFormatted("%s:%d - double=%f", __FUNCTION__, __LINE__, editDoubleValue);
    }
    else if (kind == VALUE_INT32)
    {
        editIntValue = INT32_RValue(value);
        Organik::GetLogger()->LogFormatted("%s:%d - int=%d", __FUNCTION__, __LINE__, editIntValue);
    }
    else if (kind == VALUE_INT64)
    {
        editLongLongValue = INT64_RValue(value);
        Organik::GetLogger()->LogFormatted("%s:%d - int=%lld", __FUNCTION__, __LINE__, editLongLongValue);
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
        Organik::GetLogger()->LogFormatted("%s:%d - unsupported type, name=%s", __FUNCTION__, __LINE__, KIND_NAME_RValue(value));
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
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        if (!(editingVariable.size()))
            ImGui::Text("Editing: %s", editingVariable.c_str());
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        ImGui::Text("Type: %s", KIND_NAME_RValue(editingValue));
        ImGui::Separator();
        
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        RValueType kind = (RValueType)editingValue->m_Kind;
        bool valueChanged = false;
        
        if (kind == VALUE_REAL || kind == VALUE_INT32 || kind == VALUE_INT64)
        {
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
            if (kind == VALUE_INT32)
            {
                if (ImGui::InputInt("Value", &editIntValue, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<int32_t>(editIntValue));
                    valueChanged = true;
                }
            }
            else if (kind == VALUE_INT64)
            {
                ImS64 one = (ImS64)1;
                if (ImGui::InputScalar("Value", ImGuiDataType_S64, &editLongLongValue, &one, NULL, "%lld", ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<int64_t>(editLongLongValue));
                    valueChanged = true;
                }
            }
            else
            {
                if (ImGui::InputDouble("Value", &editDoubleValue, 0.1, 10.0, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    *editingValue = RValue(static_cast<double>(editDoubleValue));
                    valueChanged = true;
                }
            }
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        }
        else if (kind == VALUE_STRING)
        {
            char buffer[1024];
            strncpy_s(buffer, editStringValue.c_str(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';
            
            if (ImGui::InputText("Value", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                RValue newStr = RValue(0);
                YYCreateString(&newStr, buffer);
                *editingValue =  newStr;
                valueChanged = true;
            }
        }
        else if (kind == VALUE_BOOL)
        {
            if (ImGui::Checkbox("Value", &editBoolValue))
            {
                *editingValue = editBoolValue ? RValue(1): RValue(0);
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
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
        DisplayVariableValue(("Array[" + std::to_string(index) + "]"), element);
    }
    else
    {
        // Nested array - show size and allow drilling down
        int arrayLength = element->GetArrayLength();
        RValue* arrayValues = element->ToVector().data();
        if (!arrayLength || !arrayValues) 
        {
            ImGui::Text("Array[%zu] (empty)", arrayLength);
            return;
        }
        if (ImGui::TreeNode(element, "Array[%zu]", arrayLength))
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
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
                
                for (int32_t j = 0; j < arrayLength; ++j)
                {
                    ImGui::PushID(&arrayValues[j]);
                    ImGui::TableNextRow();
                    
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("[%zu]", j);
                    
                    ImGui::TableSetColumnIndex(1);
                    
                    ImGui::Text("%s", KIND_NAME_RValue(&arrayValues[j]));
                    
                    ImGui::TableSetColumnIndex(2);
                    DisplayArrayElement(j, &arrayValues[j]);
                    
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }
}

void InstanceVariableViewer::DisplayVariableValue(const std::string& name, RValue* value)
{
    if (!value) {
        return;
    }
    RValueType kind = (RValueType) value->m_Kind;
	    Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
    
    if (kind == VALUE_REAL)
    {
        double realVal = REAL_RValue(value);
        ImGui::Text("%.6f", realVal);
    }
    else if (kind == VALUE_INT32)
    {
        int32_t intVal = INT32_RValue(value);
        ImGui::Text("%d", intVal);
    }
    else if (kind == VALUE_INT64)
    {
        int64_t longlongval = INT64_RValue(value);
        ImGui::Text("%lld", longlongval);
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
	    Organik::GetLogger()->LogFormatted("%s:%d VALUE_ARRAY %s", __FILE__, __LINE__, __FUNCTION__);
        DisplayArrayElement(0, value, name);
        /* 
        // Show array info
        std::vector<RValue*> editingValueRefArray = value->ToRefVector();
                            
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
                        editingVariable = name + "[" + std::to_string(i) + "]";
                        editingValue = editingValueRefArray[i];
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
        */
    }
    else if (kind == VALUE_PTR)
    {
        void* ptr = value->m_Pointer;
        ImGui::Text("0x%p", ptr);
    }
    else if (kind == VALUE_OBJECT)
    {
        YYObjectBase* obj = value->ToObject();
        ImGui::Text("Object: 0x%p", obj);
    }
    else if (kind == VALUE_UNDEFINED || kind == VALUE_UNSET)
    {
	    Organik::GetLogger()->LogFormatted("%s:%d VALUE_UNDEFINED || kind == VALUE_UNSET %s", __FILE__, __LINE__, __FUNCTION__);
        ImGui::TextDisabled("undefined");
    }
    else if (kind == VALUE_NULL)
    {
        ImGui::TextDisabled("null");
	    Organik::GetLogger()->LogFormatted("%s:%d VALUE_NULL %s", __FILE__, __LINE__, __FUNCTION__);
    }
    else
    {
	    Organik::GetLogger()->LogFormatted("%s:%d VALUE_UNK %s", __FILE__, __LINE__, __FUNCTION__);
        // For other types, try to get string representation
        std::string strVal = value->ToString();
        std::string kindName = value->GetKindName();
        ImGui::Text("%s (%s)", strVal.c_str(), kindName.c_str());
    }
}

// Demonstrate creating a simple property editor.
void InstanceVariableViewer::Draw(bool& out_mousedOver, bool* p_open, const std::string &title)
{
    ImGui::SetNextWindowSize(ImVec2(430, 750), ImGuiCond_FirstUseEver);
	    // Organik::GetLogger()->LogFormatted("%s:%d --- %s", __FILE__, __LINE__, __FUNCTION__);
    if (!ImGui::Begin("Organik Variable Viewer", p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_ChildWindows))
        out_mousedOver = true;

    static InstanceVariableViewer variable_viewer;

    variable_viewer.DrawInner();
    ImGui::End();
}