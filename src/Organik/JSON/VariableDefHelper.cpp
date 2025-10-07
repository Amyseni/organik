#include "Synthetik.h"
#include "VariableDefHelper.h"
#include "Globals.h"    
#include "Logging.h"
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "imgui/imgui.h"
#include "Organik/UI/VariableViewer.h"
#include "Organik/UI/UIManager.h"
const int32_t VariableDefHelper::GetDisplayFlagsForVar(int32_t varID) const 
{
    auto it = GetVariableDefinitions()->find(varID);
    if (it != GetVariableDefinitions()->end())
    {
        return it->second.m_DisplayFlags;
    }
    return 0;
}
const int32_t VariableDefHelper::GetDisplayFlagsForVar(const char* varName) const 
{
    int32_t varID = GetVarIDForName(varName);
    if (varID != -1)
    {
        return GetDisplayFlagsForVar(varID);
    }
    auto it = GetVariableDefinitions()->find(varID);
    if (it != GetVariableDefinitions()->end())
    {
        return it->second.m_DisplayFlags;
    }
    return 0;
}
const int32_t VariableDefHelper::GetVarIDForName(const char* varName) const 
{
    return Code_Variable_FindAlloc_Slot_From_Name(nullptr, varName);
}
const VariableDef& VariableDefHelper::GetVarDefForID(int32_t varID)
{
    auto it = GetVariableDefinitions()->find(varID);
    if (it != GetVariableDefinitions()->end())
    {
        return it->second;
    }
    // Error_Show_Action
    //     (true, true, "VariableDefHelper::GetVarDefForID: No variable definition found for varID %d", varID);
    auto& newDef = *(::new (&m_VariableDefs[varID]) VariableDef());
    newDef.m_ID = varID;
    newDef.m_Name = "Unknown Variable";
    newDef.m_Description = "No description available.";
    newDef.m_DisplayFlags = (VariableDef::DisplayFlags) 0;
    return newDef;
}
std::unordered_map<int32_t, VariableDef>* const VariableDefHelper::GetVariableDefinitions()
{
    return &m_VariableDefs;
}
const std::unordered_map<int32_t, VariableDef>* const VariableDefHelper::GetVariableDefinitions() const
{
    return &m_VariableDefs;
}
VariableDefHelper::VariableDefHelper()
{
    m_isInitialized = false;
    GetVariableDefinitions()->clear();
    m_DefFilePath = std::filesystem::current_path() / "Mods" / "variable_definitions.json";
}
void VariableDefHelper::SetPath(const std::filesystem::path& path)
{
    m_DefFilePath = path;
}
VariableDefHelper* VariableDefHelper::Get()
{
    static VariableDefHelper s_VariableDefHelper;
    return &s_VariableDefHelper;
}
bool VariableDefHelper::Init() 
{   
    if (m_isInitialized)
        return true;
    
    auto *variableDefs = GetVariableDefinitions();
    variableDefs->clear();
    variableDefs->reserve(Variables::VARIABLES_END);
    for (int i=0;i<Variables::VARIABLES_END;i++)
    {
        int32_t varID = Variables::Hashes[i];
        
        auto varDef = VariableDef();
        varDef.m_Description = "No description available.";
        varDef.m_ID = varID;
        varDef.m_DisplayFlags = (VariableDef::DisplayFlags) 0;
        varDef.m_Name = Variables::VariableNamesArr[i];
        GetLogger()->LogFormatted("VariableDefHelper: Initialized empty definition for varID %d: %s, %s",
            varID, varDef.m_Name.c_str(), varDef.m_Description.c_str()
        );
        ::new (&(*variableDefs)[varID]) VariableDef(std::forward<const VariableDef&>(varDef));
    }
    return true;
}
bool VariableDefHelper::Load() 
{
    if (m_isInitialized)
        return true;
    
    std::ostringstream strStream;
    RValue fileContentsRV;
    RValue variableDefObj;
    auto *definitionsMap = GetVariableDefinitions();
    if (!std::filesystem::exists(m_DefFilePath))
    {
        Error_Show_Action(true, true, "VariableDefHelper: No variable definition file found at %s", m_DefFilePath.string().c_str());
        return false;
    }
    
    {
        std::ifstream inFile(m_DefFilePath.generic_string());
        if (!inFile.is_open())
        {
            Error_Show_Action(true, true, "VariableDefHelper: Failed to open variable definition file at %s", m_DefFilePath.string().c_str());
            return false;
        }
        std::string line;
        
        while (std::getline(inFile, line))
        {
            strStream << line << "\n";
        }
        YYCreateString(&fileContentsRV, strStream.str().c_str());
        variableDefObj = DoBuiltin(&gml_json_parse, { fileContentsRV });
        inFile.close();
    }

    if (variableDefObj.m_Kind != VALUE_OBJECT && variableDefObj.m_Kind != VALUE_ARRAY) {
        GetLogger()->LogFormatted("VariableDefHelper: Failed to parse variable definition file at %s. Expected %s, got %s", m_DefFilePath.string().c_str()
            , "object or array", variableDefObj.GetKindName());
        return false;
    }

    GetLogger()->LogFormatted("VariableDefHelper: Loaded variable definition file. Object result kind: %s.", variableDefObj.GetKindName());
    
    YYObjectBase *defObj = (YYObjectBase*) variableDefObj.ToPointer();
    if (!defObj)
    {
        GetLogger()->LogFormatted("VariableDefHelper: Failed to parse variable definition file at %s. Expected %s, got %s", m_DefFilePath.string().c_str()
            , "object", variableDefObj.GetKindName());
        return false;
    }
    const auto& typeMapCache = *gsGetDisplayTypeMap(true);

    CHashMap<int, RValue*, 3>::iterator it = defObj->m_YYVarsMap->begin();
    const CHashMap<int, RValue*, 3>::Element* elem = nullptr;
    while(defObj->m_YYVarsMap->FindNextValue(it, &elem))
    {
        int32_t varID = elem->m_Key;
        RValue* varDefRV = elem->m_Value;

        if (!varID || !varDefRV || (varDefRV->m_Kind != VALUE_OBJECT))
        {
            GetLogger()->LogFormatted("VariableDefHelper: Skipping invalid variable definition for varID %d", varID);
            continue;
        }

        auto &varDef = (*definitionsMap)[varID];

        YYObjectBase* varDefObj = (YYObjectBase*) varDefRV->ToPointer();
        
        if (!varDefObj)
        {
            GetLogger()->LogFormatted("VariableDefHelper: Skipping invalid variable definition for varID %d: not an object", varID);
            continue;
        }
        GetLogger()->LogFormatted("VariableDefHelper: Loaded variable definition for varID %d", varID);
        
        varDef.m_ID = varID;
        varDef.m_Name = Code_Variable_Find_Name(nullptr, 5, varID);
        GetLogger()->LogFormatted("VariableDefHelper: Loaded variable name for varID %d: %s", varID, varDef.m_Name.c_str());
        RValue* nameRV = varDefObj->InternalReadYYVar(VAR_HASH(vardisplayname));
        if (nameRV && nameRV->m_Kind == VALUE_STRING)
            varDef.m_Name = nameRV->ToCString();
        GetLogger()->LogFormatted("VariableDefHelper: Loaded variable name for varID %d: %s", varID, varDef.m_Name.c_str());
        RValue* descRV = varDefObj->InternalReadYYVar(VAR_HASH(vardisplaydesc));
        if (descRV && descRV->m_Kind == VALUE_STRING)
            varDef.m_Description = descRV->ToCString();
        RValue* typeRV = varDefObj->InternalReadYYVar(VAR_HASH(vardisplaytype));
        GetLogger()->LogFormatted("VariableDefHelper: Loaded variable type for varID %d: %s", varID, typeRV ? typeRV->ToCString() : "null");
        RValue* fmtRV = varDefObj->InternalReadYYVar(VAR_HASH(vardisplayformat));
        GetLogger()->LogFormatted("VariableDefHelper: Loaded variable format for varID %d: %s", varID, fmtRV ? fmtRV->ToCString() : "null");
        RValue* specRV = varDefObj->InternalReadYYVar(VAR_HASH(vardisplayspecial));
        GetLogger()->LogFormatted("VariableDefHelper: Loaded variable special for varID %d: %s", varID, specRV ? specRV->ToCString() : "null");
        if (
            (typeRV && typeRV->m_Kind == VALUE_STRING)
            && (fmtRV && fmtRV->m_Kind == VALUE_STRING)
            && (specRV && specRV->m_Kind == VALUE_STRING)
        )
        {
            const auto &[intType, intFmt, intSpc] = std::tuple{
                Code_Variable_FindAlloc_Slot_From_Name(nullptr, typeRV->ToCString()),
                Code_Variable_FindAlloc_Slot_From_Name(nullptr, fmtRV->ToCString()),
                Code_Variable_FindAlloc_Slot_From_Name(nullptr, specRV->ToCString())
            };
            const auto& typeMapCache = *gsGetDisplayTypeMap(true);
            if (intType <= 0 || intFmt < 0 || intSpc < 0)
            {
                GetLogger()->LogFormatted("VariableDefHelper: Skipping invalid variable definition for varID %d: invalid type/format/special (%s:%d, %s:%d, %s:%d)", 
                        varID,
                        typeRV->ToCString(), intType,
                        fmtRV->ToCString(), intFmt,
                        specRV->ToCString(), intSpc
                    );
                continue;
            }
            GetLogger()->LogFormatted("VariableDefHelper: Loaded variable type for varID %d: %s", varID, typeRV ? typeRV->ToCString() : "null");
            GetLogger()->LogFormatted("VariableDefHelper: Loaded variable format for varID %d: %s", varID, fmtRV ? fmtRV->ToCString() : "null");
            GetLogger()->LogFormatted("VariableDefHelper: Loaded variable special for varID %d: %s", varID, specRV ? specRV->ToCString() : "null");
            if (typeMapCache.size() > 0 && typeMapCache.contains(intType)
                && typeMapCache.contains(intFmt)
                && typeMapCache.contains(intSpc))
            {
                GetLogger()->LogFormatted("VariableDefHelper: Loaded variable definition for varID %d: %s (%s, %s, %s)", 
                    varID, varDef.m_Name.c_str(), typeRV->ToCString(), fmtRV->ToCString(), specRV->ToCString()
                );
                varDef.m_DisplayFlags = (VariableDef::DisplayFlags) (typeMapCache.at(intType) | typeMapCache.at(intFmt) | typeMapCache.at(intSpc));
            }
            else
            {
                GetLogger()->LogFormatted("VariableDefHelper: Skipping invalid variable definition for varID %d: invalid type/format/special (%s:%d, %s:%d, %s:%d)", 
                        varID,
                        typeRV->ToCString(), intType,
                        fmtRV->ToCString(), intFmt,
                        specRV->ToCString(), intSpc
                    );
                continue;
            }
            GetLogger()->LogFormatted("VariableDefHelper: Loaded variable definition for varID %d: %s (%s, %s, %s)", 
                varID, varDef.m_Name.c_str(), typeRV->ToCString(), fmtRV->ToCString(), specRV->ToCString()
            );
            
            //::new (&(*definitionsMap)[varID]) VariableDef(std::forward<const VariableDef&>(varDef));
            //varID)) VariableDef(std::forward<const VariableDef&>(varDef));
        }
    }
    m_isInitialized = true;
    return true;
}
void VariableDef::DrawDisplayValueInTable(RValue* value){
    if (!value) {
        ImGui::Text("null");
        return;
    }
    if (m_DisplayFlags == 0)
    {
        switch (value->GetKind())
        {
        case VALUE_INT32:
        case VALUE_INT64:
        case VALUE_REAL:
        ImGui::Text("%.2f", parseRValueNumber<double>(*value));
        case VALUE_PTR:
        case VALUE_FUNCTION:
        case VALUE_ACTION:
        case VALUE_ACCESSOR:
        case VALUE_UNDEFINED:
        case VALUE_UNSET:
            ImGui::Text("Unsafe to display. Indirect pointer: %p", value);
            break;
        case VALUE_STRING:
            ImGui::Text("\"%s\"", value->ToCString());
            break;
        case VALUE_BOOL:
            ImGui::Text("%s", value->ToBoolean() ? "true" : "false");
            break;
        case VALUE_VEC3:
            ImGui::Text("R: %.2f, G: %.2f, B: %.2f", value->m_RGB->x, value->m_RGB->y, value->m_RGB->z);
            break;
        case VALUE_VEC4:
            ImGui::Text("R: %.2f, G: %.2f, B: %.2f, A: %.2f", value->m_RGBA->x, value->m_RGBA->y, value->m_RGBA->z, value->m_RGBA->w);
            break;
        case VALUE_ARRAY:
            UIManager::GetInstance()->GetElement<InstanceVariableViewer>()->DisplayVariableValue("Array", value);
            break;
        }
    }
    ImGui::Text("%s", GetRepresentation(*value).c_str());
}
void VariableDef::DrawEditorInPopup(RValue* value){

}
//? DISPLAY FLAGS = LOW BYTE
//? TYPE FLAGS = 2ND BYTE
//? SPECIAL FLAGS = HI BYTE
//? HIGHEST BYTE UNUSED
const std::unordered_map<uint32_t, int32_t>* gsGetDisplayTypeMap(bool _const)
{
    static auto gs_TypeToEnumMap = std::unordered_map<uint32_t,int32_t> {
        DISPLAYFLAGS_DO(MAKE_MAPSTRING)
    };
    return &gs_TypeToEnumMap;
}
std::unordered_map<uint32_t, int32_t>& gsGetDisplayTypeMap(int32_t _const )
{
    static auto gs_TypeToEnumMap = std::unordered_map<uint32_t,int32_t> {
        DISPLAYFLAGS_DO(MAKE_MAPSTRING)
    };
    return gs_TypeToEnumMap;
}
bool VariableDefHelper::Save() 
{
    for (const auto& [varID, varDef] : *GetVariableDefinitions())
    {
        GetLogger()->LogFormatted("VariableDefHelper: Saving variable definition for varID %d: %s (%s, 0x%X)", 
            varID, varDef.m_Name.c_str(), varDef.m_Description.c_str(), varDef.m_DisplayFlags
        );
        YYObjectBase* objStruct = (YYObjectBase*) Alloc(sizeof(YYObjectBase), __FILE__, __LINE__, false);
        objStruct->m_YYVarsMap = new CHashMap<int, RValue*, 3>();
    }
    return true;
}
const std::string VariableDef::GetRepresentation(const RValue& value) const
{
    switch (m_DisplayFlags & 0xFF00)
    {
    case TYPE_INT:
        switch(m_DisplayFlags & 0xFF)
        {
        case DISPLAY_PERCENT:
            return GetLogger()->ParseFormatting("%d%%", 
                parseRValueNumber<int32_t>(value)
            );
        case DISPLAY_MULTIPLIER:
            return GetLogger()->ParseFormatting("x%d", 
                parseRValueNumber<int32_t>(value)
            );
        case DISPLAY_ROTATION:
            return GetLogger()->ParseFormatting("%d°", 
                parseRValueNumber<int32_t>(value));
        case DISPLAY_TIME:
            if (parseRValueNumber<double>(value) >= 3600.0f)
                return GetLogger()->ParseFormatting("%d:%02d:%02d", 
                    parseRValueNumber<int32_t>(value) / 3600, 
                    (parseRValueNumber<int32_t>(value) % 3600) / 60, 
                    parseRValueNumber<int32_t>(value) % 60
                );
            else if (parseRValueNumber<double>(value) >= 120.0f)
                return GetLogger()->ParseFormatting("%02d:%02d", 
                    (parseRValueNumber<int32_t>(value) % 3600) / 60, 
                    parseRValueNumber<int32_t>(value) % 60
                );
            else if (parseRValueNumber<double>(value) >= 0.0f)
                return GetLogger()->ParseFormatting("%02d s", 
                    parseRValueNumber<int32_t>(value)
                );
            else
                return GetLogger()->ParseFormatting("-%02d s", 
                    parseRValueNumber<int32_t>(value)
                );
            break;
        case DISPLAY_HEX:
            return GetLogger()->ParseFormatting("0x%X", 
                parseRValueNumber<int32_t>(value)
            );
            break;
        case DISPLAY_FLATNUM:
        default:
            return GetLogger()->ParseFormatting("%d", 
                parseRValueNumber<int32_t>(value)
            );
        }
        break;
        case TYPE_REAL:
            switch (m_DisplayFlags & 0xFF)
            {
            case DISPLAY_PERCENT:
                return GetLogger()->ParseFormatting("%.2f%%", 
                    parseRValueNumber<double>(value)
                );
            case DISPLAY_MULTIPLIER:
                return GetLogger()->ParseFormatting("x%.2f", 
                    parseRValueNumber<double>(value)
                );
            case DISPLAY_ROTATION:
                return GetLogger()->ParseFormatting("%.2f°", 
                    parseRValueNumber<double>(value)
                );
            case DISPLAY_HEX:
                return GetLogger()->ParseFormatting("0x%X", 
                    parseRValueNumber<int32_t>(value)
                );
            case DISPLAY_TIME:
                if (parseRValueNumber<double>(value) >= 3600.0f)
                {
                    return GetLogger()->ParseFormatting("%d:%02d:%02.2f", 
                        (parseRValueNumber<int32_t>(value) / 3600), 
                        (parseRValueNumber<int32_t>(value) % 3600) / 60, 
                        (parseRValueNumber<int32_t>(value) % 60) 
                            + (parseRValueNumber<double>(value) - (double) parseRValueNumber<int32_t>(value))
                        );
                }
                else if (parseRValueNumber<double>(value) >= 120.0f)
                {
                    return GetLogger()->ParseFormatting("%02d:%02d", 
                        ((int)(parseRValueNumber<double>(value)) % 3600) / 60, 
                        (int)(parseRValueNumber<double>(value)) % 60
                    );
                }
                else if (parseRValueNumber<double>(value) >= 0.0f)
                {
                    return GetLogger()->ParseFormatting("%.2f s", 
                        parseRValueNumber<double>(value)
                    );
                }
                else
                {
                    return GetLogger()->ParseFormatting("-%.2f s", 
                        parseRValueNumber<double>(value)
                    );
                }
                break;
            case DISPLAY_FLATNUM:
            default:
                return GetLogger()->ParseFormatting("%.2f", 
                    parseRValueNumber<double>(value)
                );  
                break;
            } 
        break; // end case TYPE_REAL:
    case TYPE_STRING:
        return value.ToCString();
        break;
    case TYPE_COLOR:
        return GetLogger()->ParseFormatting("#%06X", 
            parseRValueNumber<int32_t>(value) & 0xFFFFFF
        );
        break;
    case TYPE_BOOL:
        return value.ToBoolean() ? "true" : "false";
        break;
    default:
        return GetLogger()->ParseFormatting("%.2f", 
            parseRValueNumber<double>(value)
        );
    }
}