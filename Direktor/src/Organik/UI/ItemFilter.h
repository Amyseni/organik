#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <vector>
#include <string>
#include <regex>
#include "imgui/imgui.h"

struct ItemFilter
{
    ImGuiTextFilter textFilter;
    std::string textFilterValue;
    std::unordered_map<int32_t, bool> variableFilters;
    enum : int32_t {
        FILTER_MATCH_ANY = 0x1 << 0,
        FILTER_MATCH_ALL = 0x1 << 1,
        FILTER_MATCH_NONE = 0x1 << 2,
    } variableFilterMode;


    static const char* getNameRepresentation(CInstance* inst)
    {
        RValue result;
        if (!inst || !inst->m_Object)
            return "Unknown";

        return inst->m_Object->m_Name;//result.ToCString();
    }
    void updateTextFilter(const std::string& _filterText)
    {
        if (_filterText.length() > 255)
            textFilterValue = _filterText.substr(0, 255);
        else
            textFilterValue = _filterText;
        strcpy_s(textFilter.InputBuf, textFilterValue.c_str());
        textFilter.InputBuf[255] = '\0';
    }


    ItemFilter(const std::string& _filterText = "", std::vector<int32_t> _variableFilters = {})
    {
        variableFilters.clear();
        for (const auto& varSlot : _variableFilters)
        {
            if (varSlot >= 0)
                variableFilters.insert_or_assign(varSlot, true);
        }
        textFilter = ImGuiTextFilter();
        updateTextFilter(_filterText);
            
    }
                    // Asset ID, Instance ptrs*
    std::unordered_map<int32_t, std::vector<CInstance*>> FilterInstances()
    {
        if (!textFilterValue.empty())
            updateTextFilter(textFilterValue);
        std::unordered_map<int32_t, std::vector<CInstance*>> filteredInstances;
        auto [lock, instances] = GetActiveInstances();
        for (auto [id, instance] : instances)
        {
            std::string objectName = getNameRepresentation(instance);
            if (textFilterValue.length())
                if (!textFilter.PassFilter(objectName.c_str()))
                    continue;

            if (!variableFilters.empty())
            {
                bool varMatch = false;
                bool anyMatch = false;
                bool allMatch = true;
                for (const auto& [varSlot, state] : variableFilters)
                {
                    RValue* var = instance->InternalReadYYVar(varSlot);
                    if (!var || (var->m_Kind == VALUE_NULL || 
                        var->m_Kind == VALUE_UNDEFINED || 
                        var->m_Kind == VALUE_UNSET))
                    {
                        allMatch = false;
                        if (variableFilterMode == FILTER_MATCH_ALL)
                        {
                            varMatch = false;
                            break;
                        }
                    }
                    anyMatch = true;
                    if (variableFilterMode == FILTER_MATCH_NONE)
                    {
                        varMatch = false;
                        break;
                    }
                    else if (variableFilterMode == FILTER_MATCH_ANY)
                    {
                        varMatch = true;
                        break;
                    }
                    continue;
                }

                switch(variableFilterMode)
                {
                    case FILTER_MATCH_ANY:
                        if (!anyMatch)
                            continue;
                        break;
                    case FILTER_MATCH_ALL:
                        if (!allMatch)
                            continue;
                        break;
                    case FILTER_MATCH_NONE:
                        if (anyMatch)
                            continue;
                        break;
                }
            }    
            int32_t objectIndex = instance->m_ObjectIndex;
            if (filteredInstances.contains(objectIndex))
            {
                filteredInstances[objectIndex].push_back(instance);
            }
            else
            {
                filteredInstances.insert(std::pair<int32_t, std::vector<CInstance*>>(objectIndex, std::vector<CInstance*>()));
                filteredInstances[objectIndex].push_back(instance);
            }
        }
        return filteredInstances;
    }

    void setVariableFilter(int32_t varSlot, bool state)
    {
        if (varSlot < 0)
            return;
        if (state)
            variableFilters.insert_or_assign(varSlot, true);
        else
            variableFilters.erase(varSlot);
    }
    

    // std::vector<T> FilterInstancesVector()
    // {
    //     std::vector<T> filteredInstances;
    //     for (auto [id,instance] : GetActiveInstances())
    //     {
    //         if (Matches(instance))
    //         {
    //             filteredInstances.push_back(instance);
    //         }
    //     }
    //     return filteredInstances;
    // }
    // void AddFilter(const std::string& _filterText)
    // {
    //     if (_filterText.empty())
    //         return;
    //     if (_filterText.find(filterSplitChar) != std::string::npos)
    //         processMultipleFilers(_filterText);
    //     else
    //         textFilters.push_back(_filterText);
    //     return;
    // }


    // // std::vector<std::pair<uint32_t, std::string>>
    // void addVariableFilter(const std::string& searchFilters)
    // {
    //     if (searchFilters.empty())
    //         return;
    //     std::stringstream ss();
        
    //     for(char *c = &searchFilters[0]; *c; c++)
    //     {
    //         if (*c == filterSplitChar)
    //         {
    //             variableFilters.push_back(Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), ss.str().c_str()));
    //             ss.clear();
    //             ss.str("");
    //         }
    //     }
    //     if (!ss.str().empty())
    //     {
    //         variableFilters.push_back(Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), ss.str().c_str()));
    //     }
    //     return variableFilters;
    // }

    // void addVariableFilter(int32_t varSlot)
    // {
    //     if (varSlot < 0)
    //         return;
    //     variableFilters.push_back(varSlot);
    // }

    // bool Matches(T item) const
    // {
    //     bool success = true;
    //     if (textFilters.empty() && variableFilters.empty() && filterFunctions.empty())
    //         return success;
            
    //     int totalMatches = 0;
    //     int textMatches = 0;
    //     int totalFilters = 0;
    //     int maxTextMatches = textFilters.size();
    //     for (const auto& _filterText : textFilters)
    //     {
    //         if (_filterText.empty())
    //             continue;
    //         totalFilters += maxTextMatches;
    //         std::regex filterRegex(_filterText, std::regex_constants::icase);
    //         if (!std::regex_search(getNameRepresentation(item), filterRegex))
    //             textMatches++;
    //         if (filterFlags & FILTER_MATCH_NONE && textMatches > 0)
    //             return false;
    //         else if (filterFlags & FILTER_MATCH_ALL && textMatches < maxTextMatches)
    //             return false;   
    //     }
        
    //     totalMatches += textMatches;
    //     if (!variableFilters.empty())
    //     {
    //         int varMatches = 0;
    //         int maxVarMatches = variableFilters.size();
    //         totalFilters += maxVarMatches;
    //         for (const auto& varSlot : variableFilters)
    //         {
    //             RValue* var = ((CInstance*)item)->InternalReadYYVar(varSlot);
    //             success = (var != nullptr && var->m_Kind != VALUE_UNSET && var->m_Kind != VALUE_UNDEFINED);
    //             if (var)
    //             {
    //                 if (filterFlags & FILTER_MATCH_ANY)
    //                     return true;
    //                 varMatches++;
    //             }
    //         }
    //         if (filterFlags & FILTER_MATCH_NONE && textMatches > 0)
    //             return false;
    //         else if (filterFlags & FILTER_MATCH_ALL && varMatches < maxVarMatches)
    //             return false;
    //     }
        
    //     if (!success)
    //         return false;

    //     if (filterFlags & FILTER_MATCH_ALL)
    //         return std::any_of (filterFunctions.begin(), filterFunctions.end(),
    //             [item](const Func& func) { return func(item); });

            
    //     return std::all_of(filterFunctions.begin(), filterFunctions.end(),
    //         [item](const Func& func) { return func(item); });
    // }
};