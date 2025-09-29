#include "Synthetik.h"
#include "Globals.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "DefinitionHelpers/VariableHelper.h"
#include "Action.h"
#include "Organik/UI/VariableViewer.h"
#include <regex>

int32_t Action::CreateCount = 0;
ActionLinks::ActionLinks() : m_Next(nullptr), m_Prev(nullptr) {}
ActionLinks::ActionLinks(Action* _head, Action* _next) 
    : m_Next(_next), m_Prev(nullptr)
    {
    }
void ParseJSONRValue(RValue* jsonValue, RValue* out) {
    if (!jsonValue)
    {
        *out = RValue(false);
        return;
    }
    
    Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing RValue of kind %s", jsonValue->GetKindName());

    uint32_t kind = jsonValue->GetKind();

    // String handling
    if (kind == VALUE_STRING)
    {
        std::string str = std::string(YYGetString(jsonValue, 0));
        if (str.starts_with("###"))
        {
            *out = Code_Variable_FindAlloc_Slot_From_Name(nullptr, str.substr(3).c_str());
            (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
        }
        else if (str.starts_with("##"))
        {
            RValue tmp1;
            YYCreateString(&tmp1, str.substr(2).c_str());
            *out = DoBuiltin(&gml_asset_get_index, { tmp1 });
            (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
        }
        else
        {
            YYCreateString(out, str.c_str());
            (*out).m_Kind = static_cast<RValueType>(VALUE_STRING & MASK_KIND_RVALUE);
        }
    }
    // Array handling
    else if (kind == VALUE_ARRAY)
    {
        // Create a new vector container
        std::vector<double> dummy_array = {0.0};
        *out = RValue();
        YYCreateArray(out, 0, dummy_array.data());

        // Process array items
        std::vector<RValue*> jsonVec = jsonValue->ToRefVector();
        
        for (auto &item : jsonVec)
        {
            Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing array item of type: %s", item->GetKindName());

            RValue parsedValue(5);
            ParseJSONRValue(item, &parsedValue);
            out->push_back(parsedValue);
            Organik::GetLogger()->LogFormatted("ParseJSONRValue: added item of type: %s to vector", out->back().GetKindName());
        }
        (*out).m_Kind = static_cast<RValueType>(VALUE_ARRAY & MASK_KIND_RVALUE);
        (*out).m_Flags = 0;
    }
    // Object handling
    else if (kind == VALUE_OBJECT)
    {
        // Create a new map container
        YYObjectBase* pObj = jsonValue->ToObject();
        
        
        // Get object property names
        std::vector<RValue*> refVec = DoBuiltin(&gml_variable_instance_get_names, { *jsonValue }).ToRefVector();
        
        for (auto name : refVec)
        {
            RValue tmp2 = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name->ToCString());
            int32_t propHash = parseRValueNumber<int32_t>(tmp2);
            
            Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing object member: %s (%d)", 
                name->ToCString(), propHash);
            
            // Parse the property value directly into the map
            RValue* propValue = (jsonValue->ToObject())->InternalGetYYVarRef(propHash);
            ParseJSONRValue(propValue, (*pObj)[propHash]);
            
            Organik::GetLogger()->LogFormatted("ParseJSONRValue: added item of type: %s to map", 
                (*pObj)[propHash]->GetKindName());
        }
    }
    else if (kind == VALUE_BOOL)
    {
        Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing boolean value: %s", 
            jsonValue->ToBoolean() ? "true" : "false");
        *out = RValue(jsonValue->ToBoolean());
        (*out).m_Kind = static_cast<RValueType>(VALUE_BOOL & MASK_KIND_RVALUE);
        Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsed boolean RValue of kind %s", (*out).GetKindName());
    }
    // Integer handling
    else if (kind == VALUE_INT32)
    {
        *out = parseRValueNumber<int32_t>(*jsonValue);
        (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
    }
    // Long integer handling
    else if (kind == VALUE_INT64)
    {
        int64_t val64 = parseRValueNumber<int64_t>(*jsonValue);
        if (val64 < INT32_MAX)
        {
            *out = parseRValueNumber<int32_t>(*jsonValue);
            (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
        }
        else
        {
            *out = parseRValueNumber<int64_t>(*jsonValue);
            (*out).m_Kind = static_cast<RValueType>(VALUE_INT64 & MASK_KIND_RVALUE);
        }
    }
    // Real (double) handling
    else if (kind == VALUE_REAL)
    {
        *out = parseRValueNumber<double>(*jsonValue);
        (*out).m_Kind = static_cast<RValueType>(VALUE_REAL & MASK_KIND_RVALUE);
    }
    // Default case for other types
    else
    {
        *out = *reinterpret_cast<int64_t*>(std::launder(&jsonValue->m_Real));
        (*out).m_Kind = static_cast<RValueType>(jsonValue->m_Kind & MASK_KIND_RVALUE);
    }
    
    Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsed RValue of kind %s", (*out).GetKindName());
}
// bool doRequiresVars (CInstance* self, CInstance* other, RValue* params, RValue* result) {
//     if (!self || !params) return false;
//     RValue mode = params->ContainsValue(VAR_HASH(mode)) ? (*params)[VAR_HASH(mode)] : RValue("equal");
//     int32_t modeHash = mode.m_Kind == VALUE_STRING ? Code_Variable_FindAlloc_Slot_From_Name(nullptr, mode.ToCString()) : VAR_HASH(equal);
//     RValue objectRV = params->ContainsValue(VAR_HASH(object)) ? (*params)[VAR_HASH(object)] : RValue(-2);
//     int32_t objectHash = objectRV.m_Kind == VALUE_STRING ? Code_Variable_FindAlloc_Slot_From_Name(nullptr, objectRV.ToCString()) :
//                          objectRV.m_Kind == VALUE_INT32 ? parseRValueNumber<int32_t>(objectRV) : -2;

//     YYObjectBase* vars = params->ContainsValue(VAR_HASH(variables)) ? (*params)[VAR_HASH(variables)].ToObject() : nullptr;
//     CInstance* targetInstance = self;
//     if (objectHash != -2 && objectHash != VAR_SELF) {
//         targetInstance = CInstance::FirstOrDefault([&](CInstance* inst) -> bool {
//             return (inst && inst->m_ObjectIndex == objectHash);
//         });
//         if (!targetInstance) {
//             Organik::GetLogger()->LogFormatted("doRequiresVars: No instance found with object index %d", objectHash);
//             *result = RValue(false);
//             return false;
//         }
//         auto keys = DoBuiltin(&gml_ds_map_keys_to_array, { RValue(vars) }).ToRefVector();

//         for (const RValue*& key : keys) 
//         {
//             RValue *tmp = targetInstance->InternalReadYYVar(parseRValueNumber<int32_t>(key));
//             if (!tmp) return false;
//             if (modeHash == VAR_HASH(equal)) {
//                 Organik::GetLogger()->LogFormatted("doRequiresVars: 'equal' currently unimplemented");
//                 return tmp != nullptr;
//             } else if (modeHash == VAR_HASH(isset)) {
//                 return tmp != nullptr;
//             } else if (modeHash == VAR_HASH(isunset)) {
//                 return tmp == nullptr;
//             } else {
//                 Organik::GetLogger()->LogFormatted("doRequiresVars: Unknown mode hash %d", modeHash);
//                 *result = RValue(true);
//                 return true;
//             }
//         }
//         return true;
//     }
//     return true;
// }

std::unordered_map<int32_t, PFN_ACTIONHANDLER>* GetActionHandlers()
{
    static std::unordered_map<int32_t, PFN_ACTIONHANDLER> actionHandlers;
    if (actionHandlers.size() == 0)
    {
        actionHandlers = {
            { VAR_HASH(apply_variables), &doApplyVariables },
            //{ Code_Variable_FindAlloc_Slot_From_Name(nullptr, "call_script"), doCallScript },
            //{ Code_Variable_FindAlloc_Slot_From_Name(nullptr, "call_builtin"), doCallBuiltin },
            //{ Code_Variable_FindAlloc_Slot_From_Name(nullptr, "requires_variables"), doRequiresVars },
            { VAR_HASH(rng_chance), &doRequiresChance },
            { VAR_HASH(variable_get), &doGetFrom },
        };
    }
    return &actionHandlers;
}
void RegisterActionTypeHandler(int32_t actionType, PFN_ACTIONHANDLER handler)
{
    auto handlers = GetActionHandlers();
    (*handlers)[actionType] = handler;
}

bool doGetFrom(CInstance* self, CInstance* other, YYObjectBase* context) {
    if (!context) return false;
    RValue* result = context->InternalGetYYVarRef(VAR_HASH(result));
    RValue* optimized = context->InternalGetYYVarRef(VAR_HASH(optimized));
    
    Organik::GetLogger()->LogFormatted("doGetFrom: Starting with optimized flag: %s", 
        optimized->ToBoolean() ? "true" : "false");
    if (!optimized->ToBoolean() || true ) {
        Organik::GetLogger()->LogFormatted("doGetFrom: Processing 'from' parameter");
        RValue *fromStr = context->InternalGetYYVarRef(VAR_HASH(from));
        if (fromStr->m_Kind != VALUE_STRING) {
            Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
            *result = RValue();
            return false;
        }
        std::istringstream iss(fromStr->ToCString());
        std::string objectPart, variablePart;
        if (!std::getline(iss, objectPart, '.') || !std::getline(iss, variablePart)) {
            Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
            *result = RValue();
            return false;
        }

        Organik::GetLogger()->LogFormatted("doGetFrom: Parsed 'from' as object '%s' and variable '%s'", 
            objectPart.c_str(), variablePart.c_str());

        int32_t objectHash = -1;
        if (objectPart._Equal("self"))
            objectHash = VAR_HASH(self);
        else if (objectPart._Equal("other"))
            objectHash = VAR_HASH(other);
        else
            objectHash = parseRValueNumber<int32_t>(
                DoBuiltin(&gml_asset_get_index, std::vector<RValue>{objectPart.c_str()})
            );

        Organik::GetLogger()->LogFormatted("doGetFrom: Resolved object '%s' to hash %d", objectPart.c_str(), objectHash);
        int32_t variableHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, variablePart.c_str());
        if (objectHash <= 0) {
            Error_Show_Action("doGetFrom: 'from' parameter contains invalid object name", true, true);
            *result = RValue();
            return false;
        }

        Organik::GetLogger()->LogFormatted("doGetFrom: Parsed 'from' as object hash %d and variable hash %d", objectHash, variableHash);
        *(context->InternalGetYYVarRef(VAR_HASH(object))) = RValue(objectHash);
        *(context->InternalGetYYVarRef(VAR_HASH(variable))) = RValue(variableHash);
        *(optimized) = RValue(true);
        //? re:scale, will get optimized as it is called, if necessary
    }
    
    Organik::GetLogger()->LogFormatted("doGetFrom: Retrieving values using object hash and variable hash");
    
    int32_t objectHash = parseRValueNumber<int32_t>(context->InternalReadYYVar(VAR_HASH(object)));
    int32_t variableHash = parseRValueNumber<int32_t>(context->InternalReadYYVar(VAR_HASH(variable)));
    CInstance* targetInstance;
    Organik::GetLogger()->LogFormatted("doGetFrom: Looking for instance.", objectHash);
    if ((objectHash <= 0) || variableHash <= 0) {
        Error_Show_Action("doGetFrom: 'from' parameter contains invalid object or variable name", true, true);
        *result = RValue(false);
        return false;
    }
    else if (objectHash == VAR_HASH(self) && self)
    {
        Organik::GetLogger()->LogFormatted("doGetFrom: 'from' resolves to self instance");
        targetInstance = self;
    }
    else if (objectHash == VAR_HASH(other) && other)
    {
        Organik::GetLogger()->LogFormatted("doGetFrom: 'from' resolves to other instance");
        targetInstance = other;
    }
    else {
        Organik::GetLogger()->LogFormatted("doGetFrom: Looking for instance with object hash %d", objectHash);
        auto objectData = Object_Data(objectHash);
        targetInstance = (objectData->m_Instances.m_Count ? objectData->m_Instances.m_First->m_Object : nullptr);
    }
    
    if (!targetInstance) Error_Show_Action("doGetFrom: No instance found for specified object", true, true);
    Organik::GetLogger()->LogFormatted("doGetFrom: Found instance %p", targetInstance);

    RValue *resultRV = targetInstance->InternalReadYYVar(variableHash);
    if (!resultRV) {
        Error_Show_Action("doGetFrom: Variable not found on target instance", true, true);
        *result = RValue();
        return false;
    }
    RValue *scaleRV = context->InternalReadYYVar(VAR_HASH(scale));
    if (!scaleRV) {
        Organik::GetLogger()->LogFormatted("doGetFrom: No scale parameter, returning raw value of kind %s", resultRV->GetKindName());
        *result = resultRV ? *resultRV : RValue(1.0);
        return true;
    }
    
    double scaleFactorFinal = 1.0;
    Organik::GetLogger()->LogFormatted("doGetFrom: Processing scale factor of kind %s", scaleRV->GetKindName());
    if (scaleRV->GetKind() == VALUE_OBJECT)
    {
        //? let the function optimize itself
        doGetFrom(targetInstance, targetInstance, scaleRV->ToObject());
        RValue *out = scaleRV->ToObject()->InternalGetYYVarRef(VAR_HASH(result));

        if (out->GetKind() == VALUE_REAL || out->GetKind() == VALUE_INT32 || out->GetKind() == VALUE_INT64) {
            scaleFactorFinal = parseRValueNumber<double>(out);    
        } else {
            Error_Show_Action("doGetFrom: 'scale' resolved to non-numeric value.", true, true);
            *result = RValue(false);
            return false;
        }
    } else if (scaleRV->GetKind() == VALUE_REAL || scaleRV->GetKind() == VALUE_INT32 || scaleRV->GetKind() == VALUE_INT64) {
        scaleFactorFinal = parseRValueNumber<double>(scaleRV);
    } else {
        Error_Show_Action("doGetFrom: 'scale' parameter should be a number or a map resolving to a number.", true, true);
        *result = RValue(false);
        return false;
    }

    Organik::GetLogger()->LogFormatted("doGetFrom: Applying scale factor %f", scaleFactorFinal);
    if (resultRV && (resultRV->GetKind() == VALUE_REAL || resultRV->GetKind() == VALUE_INT32 || resultRV->GetKind() == VALUE_INT64)) {
        *result = RValue(parseRValueNumber<double>(resultRV) * scaleFactorFinal);
        return true;
    } else {
        Error_Show_Action("doGetFrom: Cannot scale non-numeric value.", true, true);
        *result = RValue(false);
        return false;
    }
    Organik::GetLogger()->LogFormatted("doGetFrom: Warning: reached end of function unexpectedly");
    *result = RValue(1.0);
    return true;
}
bool doRequiresChance(CInstance* self, CInstance* other, YYObjectBase* context) {
    if (!self || !context) return false;
    RValue *chanceRV = context->InternalReadYYVar(VAR_HASH(chance)) ? context->InternalReadYYVar(VAR_HASH(chance)) : nullptr;
    RValue *scaleRV = context->InternalReadYYVar(VAR_HASH(scale)) ? context->InternalReadYYVar(VAR_HASH(scale)) : nullptr;
    double scaleFactor = 1.0;
    if (scaleRV && scaleRV->GetKind() == VALUE_OBJECT) {
        RValue out;
        YYObjectBase* scaleObj = scaleRV->ToObject();
        doGetFrom(self, other, scaleObj);
        static thread_local int recursionCount = 0;
        recursionCount++;
        while (scaleObj->InternalGetYYVarRef(VAR_HASH(result))->GetKind() == VALUE_OBJECT
                && recursionCount < LIMIT_RECURSION_GETVAR) {

            YYObjectBase* tmp = scaleObj;
            scaleObj = scaleObj->InternalGetYYVarRef(VAR_HASH(result))->ToObject();
            recursionCount++;
            doGetFrom(self, other, scaleObj);
            recursionCount--;
            if (recursionCount < 0) Error_Show_Action("doRequiresChance: Recursion count negative, logic error!", true, true);
            if (tmp == scaleObj->InternalGetYYVarRef(VAR_HASH(result))->ToObject()) break; // prevent infinite loop on self-referencing objects
        }
        out = *(scaleObj->InternalGetYYVarRef(VAR_HASH(result)));
        if (out.GetKind() == VALUE_REAL || out.GetKind() == VALUE_INT32 || out.GetKind() == VALUE_INT64) {
            scaleFactor = parseRValueNumber<double>(out);
        } else {
            Error_Show_Action("doRequiresChance: 'scale' resolved to non-numeric value.", true, true);
            (*context->InternalGetYYVarRef(VAR_HASH(result))) = RValue(false);
            return false;
        }
    }
    else if (parseRValueNumber<double>(*scaleRV) > 0.0) {
        scaleFactor = parseRValueNumber<double>(*scaleRV);
    }
    double chance = parseRValueNumber<double>(chanceRV);
    double finalChance = chance * scaleFactor;

    double roll = ((*Utils::getrng())() % 100000) / 1000.0;
    (*context->InternalGetYYVarRef(VAR_HASH(result))) = RValue(roll < finalChance);
    return (*context->InternalGetYYVarRef(VAR_HASH(result))).ToBoolean();
}
CCode::CCode(bool _watch, int _kind, int _compiled, const char* _name, YYGMLFuncs* _functions, const char* _str, int _codeIndex,
    int _localsCount, int _argsCount, int _flags, YYObjectBase* _prototype, int _offset, RToken _token, RValue _value,
    PVOID _vmInstance, PVOID _vmDebugInfo, const char* _code, CCode* _next)
    : m_Watch(_watch), m_Next(_next), m_Kind(_kind), m_Compiled(_compiled), m_Name(_name), m_Functions(_functions), m_Str(_str),
      m_CodeIndex(_codeIndex), m_LocalsCount(_localsCount), m_ArgsCount(_argsCount), m_Flags(_flags),
      m_Prototype(_prototype), m_Offset(_offset), m_Token(_token), m_Value(_value),
      m_VmInstance(_vmInstance), m_VmDebugInfo(_vmDebugInfo), m_Code(_code)
{
}
CEvent::CEvent(PFN_ACTIONHANDLER pfn, int32_t objIndex) {
    Error_Show_Action("CEvent::CEvent: Deprecated, use overload with Action*!", true, true);
}
CEvent::CEvent(Action* actions, int32_t objIndex)
{   
    m_Code = reinterpret_cast<CCode*>(&actions);
    this->m_OwnerObjectID = objIndex;
}
CEvent* CEvent::InsertIntoMap(CHashMap<int64_t, CEvent*, 3>* eventMap, int32_t eventID, int32_t eventSubID,
    Action* pAction, int32_t m_OwnerObjectID, bool replaceIfExists)
{
    Error_Show_Action("CEvent::InsertIntoMap: Deprecated, use overload with PFN_ACTIONHANDLER!", true, true);
    return nullptr;
}
Action **CurrentActionGlobal()
{
    static Action* currentAction = nullptr;
    return &currentAction;
}
YYObjectBase* volatile* CurrentContextGlobal()
{
    static YYObjectBase* currentContext = nullptr;
    return &currentContext;
}
// std::unordered_map<CCode*, bool>* PatchedEventsThisIter()
// {
//     static std::unordered_map<CCode*, bool> patchedEvents;
//     return &patchedEvents;
// }
static std::unordered_map<CCode*, bool> patchedEventsThisIter;
void ClearPatchCache()
{
    patchedEventsThisIter.clear();
}
std::unordered_map<CCode*, bool>* PatchedCodeThisIter()
{
    return &patchedEventsThisIter;
}
static inline struct CEventAllocator {
    std::vector<CEvent *> allocatedEvents = {};
    CEvent* Allocate(CCode* existingCode, int32_t newOwnerObjectID) {
        CEvent* newEvent = new CEvent();
        newEvent->m_Code = existingCode;
        newEvent->m_OwnerObjectID = newOwnerObjectID;
        allocatedEvents.push_back(newEvent);
        return newEvent;
    }
    ~CEventAllocator() {
        for (CEvent* event : allocatedEvents) {
            delete event;
        }
        allocatedEvents.clear();
    }
} _CEventAllocator;

//! this function adds the CCode pointer to PatchedCodeThisIter
//! which must be cleared by the caller using ClearPatchCache after recursing any children for this action
//! MUST BE DONE PER ACTION CALL, NOT PER EVENT TRIGGER
CEvent* CEvent::InsertIntoMap(CHashMap<int64_t, CEvent*, 3>* eventMap, int32_t eventID, int32_t eventSubID,
    PFN_ACTIONHANDLER pfn, YYObjectBase* actionActualInnerParams, int32_t m_OwnerObjectID, bool appendToChain, bool after)
{

    Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Inserting event ID %d, subID %d", eventID, eventSubID);
    if (!eventMap || !pfn) return nullptr;
    Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Event map and function pointer valid");
    
    int64_t eventKey = (int64_t(eventID) << 32) | int64_t(eventSubID);
    auto elem = eventMap->FirstOrDefault([&](const int64_t& key, const CHashMapHash& hash, CEvent* value) -> bool {
        return key == eventKey;
    });

    if (!elem || !(elem->m_Value)) 
        Error_Show_Action("CEvent::InsertIntoMap: Invalid event map or element!", true, true);
    
    if (PatchedCodeThisIter()->contains(elem->m_Value->m_Code))
    {
        if (elem->m_Value->m_OwnerObjectID != m_OwnerObjectID)
        {
            // this sucks but we have to allocate a new CEvent to ensure we get called by ExecuteIt for this object
            // it looks like we are leaking the current pointer but it is still referenced by the parent object event
            elem->m_Value = _CEventAllocator.Allocate(elem->m_Value->m_Code, m_OwnerObjectID);
        }
        return elem->m_Value;

    }
    PatchedCodeThisIter()->emplace(elem->m_Value->m_Code, true);
    Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Retrieved event map element %p CODE %p", elem->m_Value, elem->m_Value->m_Code);
    //! FUCK ALL THAT SHIT ABOUT EVENT MAPS, WE CAN CREATE OUR OWN ARRAYS INSIDE THE CODE OBJECTS
    
    if (after)
    {

        int32_t len = elem->m_Value->m_Code->m_Token.m_AfterLength;
        Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Allocating %d bytes for AfterArray", (len + 1) << 3);
        elem->m_Value->m_Code->m_Token.m_AfterLength = ++len;

        YYArrayResize((RValue**) &(elem->m_Value->m_Code->m_Token.m_AfterArray), len << 3, __FILE__, 0x4a7);
        elem->m_Value->m_Code->m_Token.m_AfterArray[len - 1].first = pfn;
        elem->m_Value->m_Code->m_Token.m_AfterArray[len - 1].second = actionActualInnerParams;

        Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: New After size: %d", elem->m_Value->m_Code->m_Token.m_AfterLength);
        Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: MemoryWATCH: %p, %p, %p, %p, %p, %p, %d", 
            elem->m_Value->m_Code->m_Token.m_AfterArray, elem->m_Value->m_Code->m_Token.m_AfterArray + 1,
            (elem->m_Value->m_Code->m_Token.m_AfterArray[0].first), (elem->m_Value->m_Code->m_Token.m_AfterArray[0].second),
            (elem->m_Value->m_Code->m_Token.m_AfterArray[len - 1].first), (elem->m_Value->m_Code->m_Token.m_AfterArray[len - 1].second),
            elem->m_Value->m_Code->m_Token.m_AfterLength);
    }
    else
    {
        int32_t len = elem->m_Value->m_Code->m_Token.m_BeforeLength;
        Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Allocating %d bytes for BeforeArray", (len + 1) << 3);
        elem->m_Value->m_Code->m_Token.m_BeforeLength = ++len;

        YYArrayResize((RValue**) &(elem->m_Value->m_Code->m_Token.m_BeforeArray), len << 3, __FILE__, 0x4a7);
        elem->m_Value->m_Code->m_Token.m_BeforeArray[len - 1].first = pfn;
        elem->m_Value->m_Code->m_Token.m_BeforeArray[len - 1].second = actionActualInnerParams;
        
        Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: New Before size: %d", elem->m_Value->m_Code->m_Token.m_BeforeLength);
        Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: MemoryWATCH: %p, %p, %p, %p, %p, %p, %d", 
            elem->m_Value->m_Code->m_Token.m_BeforeArray, elem->m_Value->m_Code->m_Token.m_BeforeArray + 1,
            (elem->m_Value->m_Code->m_Token.m_BeforeArray[0].first), (elem->m_Value->m_Code->m_Token.m_BeforeArray[0].second),
            (elem->m_Value->m_Code->m_Token.m_BeforeArray[len - 1].first), (elem->m_Value->m_Code->m_Token.m_BeforeArray[len - 1].second),
            elem->m_Value->m_Code->m_Token.m_BeforeLength);
        // void* newPtr = malloc((len + 1) << 3);

        // if (!newPtr)
        //     Error_Show_Action("CEvent::InsertIntoMap: Failed to allocate memory for BeforeArray", true, true);

        // if (len)
        // {
        //     memcpy(newPtr, elem->m_Value->m_Code->m_Token.m_BeforeArray, len << 3);
        //     free(elem->m_Value->m_Code->m_Token.m_BeforeArray);
        // }
        // ++len;
        // elem->m_Value->m_Code->m_Token.m_BeforeLength = len;
        // *reinterpret_cast<void**>(&elem->m_Value->m_Code->m_Token.m_BeforeArray) = newPtr;

        // elem->m_Value->m_Code->m_Token.m_BeforeArray[len - 1].first = pfn;
        // elem->m_Value->m_Code->m_Token.m_BeforeArray[len - 1].second = actionActualInnerParams;

        
    }
    return elem->m_Value;
}
    // auto thisActionChainHash = (uint32_t)pfn ^ (uint32_t)actionActualInnerParams;
    // YYObjectBase* sharedObjEventParams = GetSharedObjectEventParams(m_OwnerObjectID, eventID, eventSubID);
    // RValue* thisActionParams = sharedObjEventParams->InternalGetYYVarRef(m_OwnerObjectID);
    // if (thisActionParams->GetKind() != VALUE_OBJECT)
    // {
    //     FREE_RValue(thisActionParams);
    //     (*std::launder(&thisActionParams->m_Pointer)) = actionActualInnerParams;
    //     thisActionParams->m_Kind = VALUE_OBJECT;
    //     thisActionParams->m_Flags = 0;
    // }
    // YYObjectBase* thisActionParamsObj = sharedObjEventParams->InternalGetYYVarRef(m_OwnerObjectID)->ToObject();
    // if (PatchedEventsThisIter()->contains(thisActionParamsObj)) // this is the same event we just patched on a parent
    // {
    //     //so we are looking up the action chain we have been editing already
    //     std::vector<RValuePair<PFN_ACTIONHANDLER, YYObjectBase*>>* hookChainTarget = &((*ActionChainsByContext())[thisActionParamsObj]);
        
    //     //! it turns out that YYArrayResize implicitly assumes you are pointing within some sort of struct the engine manages
    //     //! but can operate on any RValue pointer reference, and can allocate in place of nullptr so we can do this :)

    //     // length argument is the number of bytes the final memory block needs space for, so usually
    //     // it is shifted left 4 because RValues are 16 bytes, in this case we aren't storing RValues though
    //     // so we shift twice instead.
    //     if (after)
    //     {
    //         elem->m_Value->m_Code->m_Token.m_AfterLength += 1;
    //         YYArrayResize((reinterpret_cast<RValue**>(&elem->m_Value->m_Code->m_Token.m_AfterArray)), 
    //             elem->m_Value->m_Code->m_Token.m_AfterLength << 2, __FILE__, 0x4a7);
            
    //         elem->m_Value->m_Code->m_Token.m_AfterArray[elem->m_Value->m_Code->m_Token.m_AfterLength - 1] 
    //                 = RValuePair<PFN_ACTIONHANDLER, YYObjectBase*>(pfn, actionActualInnerParams);
    //     }
    //     else
    //     {
    //         elem->m_Value->m_Code->m_Token.m_BeforeLength += 1;
    //         YYArrayResize((reinterpret_cast<RValue**>(&elem->m_Value->m_Code->m_Token.m_BeforeArray)), 
    //             elem->m_Value->m_Code->m_Token.m_BeforeLength << 2, __FILE__, 0x4a7);
            
    //         elem->m_Value->m_Code->m_Token.m_BeforeArray[elem->m_Value->m_Code->m_Token.m_BeforeLength - 1] 
    //                 = RValuePair<PFN_ACTIONHANDLER, YYObjectBase*>(pfn, actionActualInnerParams);
    //     }
    // }
    // RValue theActualArrayWrapper = RValue();
    // *std::launder(&theActualArrayWrapper.m_Pointer) = 
    // if (elem && elem->m_Value) {
    //     Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Modifying existing event");
    //     if (!elem->m_Value->m_Code)
    //         Error_Show_Action("CEvent::InsertIntoMap: Invalid existing event code (null)!", true, true);

        
    //     Action *& pAction = reinterpret_cast<Action*&>(elem->m_Value->m_Code);
    //     RValue newActionPair;
    //     YYObjectBase* parameters = nullptr;
    //     if (!params)
    //     {
    //         Error_Show_Action("CEvent::InsertIntoMap: Dummy params creation not supported due to requiring unique pointers. FIX ME!", true, true);
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Creating dummy parameter object");
    //         RValue dummyPtr;
    //         StructCreate(&dummyPtr);
    //         parameters = dummyPtr.ToObject();
    //     }
    //     else
    //     {
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Using provided parameter object");
    //         parameters = params;
    //     }
    //     Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Created new action pair of kind %s", newActionPair.GetKindName());
    //     //! Handle the case where there is no existing action chain array
    //     //! edit: where we just created the array (which means m_ArrLength is still 0)
    //     if (pAction->m_Token.m_Value.GetKind() != VALUE_ARRAY || pAction->m_Token.m_ArrLength < 2)
    //     {
    //         PFN_ACTIONHANDLER original = reinterpret_cast<PFN_ACTIONHANDLER>(elem->m_Value->m_Code->m_Functions->m_CodeFunction);

    //         //? I don't like it, but we will just create the entire 2d array inside this condition and then return
    //         //? it's much simpler than trying to create a chain of condiitonals handling each part of the array
    //         //? and this is an initializer so it only happens when we load or reload
    //         //TODO: Fix?
            
    //         // RValue dummy_array = RValue();
    //         // std::vector<double> dummy_data = {1.0};
    //         // Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Creating new 2D array for action chain");
    //         // YYCreateArray(&(pAction->m_Token.m_Value), 1, dummy_data.data());
    //         // pAction->m_Token.m_Value.push_back(RValue());
    //         // YYCreateArray(&(pAction->m_Token.m_Value[0]), 1, dummy_data.data());
    //         // YYCreateArray(&(pAction->m_Token.m_Value[1]), 1, dummy_data.data());

            
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: assuming we already created outer array with 2 inner arrays");
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: outerkind: %s, outerlen %d,arrlen1 %d, arrlen2 %d, arrkind1 %s, arrkind2 %s"
    //             , pAction->m_Token.m_Value.GetKindName()
    //             , pAction->m_Token.m_Value.GetArrayLength()
    //             , pAction->m_Token.m_Value[0].GetArrayLength()
    //             , pAction->m_Token.m_Value[1].GetArrayLength()
    //             , pAction->m_Token.m_Value[0].GetKindName()
    //             , pAction->m_Token.m_Value[1].GetKindName()
    //         );
            
    //         auto &refPair1 = pAction->m_Token.m_Value[0][0];
    //         auto &refPair2 = pAction->m_Token.m_Value[1][0];
    //         refPair1.SetPairFirst(after ? original : pfn);
    //         refPair1.SetPairSecond(after ? nullptr : params);
    //         refPair2.SetPairFirst(after ? pfn : original);
    //         refPair2.SetPairSecond(after ? params : nullptr);
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Pushed new action pairs into array");
    //         GetLogger()->LogFormatted("CEvent::InsertIntoMap: pair1 first: %p, second: %p", refPair1.GetPairFirst<PFN_ACTIONHANDLER>(), refPair1.GetPairSecond<YYObjectBase*>());
    //         GetLogger()->LogFormatted("CEvent::InsertIntoMap: pair2 first: %p, second: %p", refPair2.GetPairFirst<PFN_ACTIONHANDLER>(), refPair2.GetPairSecond<YYObjectBase*>());
    //         pAction->m_Token.m_ArrLength = pAction->m_Token.m_Value.GetArrayLength();
    //         pAction->m_Token.m_Index = 1;
    //         pAction->m_Token.m_Position = after ? 0 : 1; // Position is the index of the original function
            
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Set array metadata: Length %d, Index %d, Position %d", 
    //             pAction->m_Token.m_ArrLength, pAction->m_Token.m_Index, pAction->m_Token.m_Position);
    //         (*PatchedActionArrays())[params] = pAction->m_Token.m_Value.ToPointer();
    //         if (pAction->m_Token.m_Value.ToPointer() != (*PatchedActionArrays())[params])
    //             Error_Show_Action("CEvent::InsertIntoMap: Array ref pointer mismatch after editing!", true, true);
    //         return elem->m_Value;
    //     }
    //     //? we have an existing array
    //     std::vector<double> dummy_data = {1.0};
    //     if (!appendToChain)
    //     {
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Inserting new action pair into existing array without chaining");
    //         if(after)
    //         {
    //             Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Appending after original function");
    //             //? yay the easy one
    //             pAction->m_Token.m_Value.push_back(RValue());
    //             YYCreateArray(&(pAction->m_Token.m_Value[pAction->m_Token.m_ArrLength]), 1, dummy_data.data());
    //             auto &refPair = pAction->m_Token.m_Value[pAction->m_Token.m_ArrLength][0];
    //             refPair.SetPairFirst(pfn);
    //             refPair.SetPairSecond(params);
    //             pAction->m_Token.m_ArrLength++;
    //         }
    //         else
    //         {
    //             Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Inserting before original function");
    //             //? OH BOY TIME TO RESIZE AN ARRAY AND INSERT SOMETHING IN THE MIDDLE
    //             //! MOST FAVORITE OF TASKS
    //             int32_t targetPosition = pAction->m_Token.m_Position;
    //             pAction->m_Token.m_Value.push_back(RValue());
    //             pAction->m_Token.m_ArrLength++;

    //             Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Shifted array to make space at position %d", targetPosition);
    //             for (int32_t i = pAction->m_Token.m_ArrLength - 1; i > targetPosition; i--)
    //             {
    //                 Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Shifting index %d to index %d", i - 1, i);
    //                 pAction->m_Token.m_Value[i] = pAction->m_Token.m_Value[i - 1];
    //             }
    //             Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Inserting new action pair at position %d", targetPosition);
    //             YYCreateArray(&(pAction->m_Token.m_Value[targetPosition]), 1, dummy_data.data());
    //             auto &refPair = pAction->m_Token.m_Value[targetPosition][0];
    //             refPair.SetPairFirst(pfn);
    //             refPair.SetPairSecond(params);
    //             pAction->m_Token.m_Position++; // move the original function position forward
    //         }
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Updated array metadata: Length %d, Index %d, Position %d", 
    //             pAction->m_Token.m_ArrLength, pAction->m_Token.m_Index, pAction->m_Token.m_Position);
            
    //         if (pAction->m_Token.m_Value.ToPointer() != (*PatchedActionArrays())[params])
    //             Error_Show_Action("CEvent::InsertIntoMap: Array ref pointer mismatch after editing!", true, true);
    //         // (*PatchedActionArrays())[params] = pAction->m_Token.m_Value.ToPointer();
    //         return elem->m_Value;
    //     }
    //     else
    //     {
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Appending new action pair to existing chain");
    //         int32_t targetPos = after ? pAction->m_Token.m_ArrLength - 1 : pAction->m_Token.m_Position - 1;
    //         if (targetPos < 0) Error_Show_Action("CEvent::InsertIntoMap: Cannot append to chain before start of array!", true, true);
            
    //         auto &refArr = pAction->m_Token.m_Value[targetPos];
    //         auto *refPair = &(refArr.push_back(RValue()));
    //         int32_t newIndex = refArr.GetArrayLength() - 1;
    //         refPair->SetPairFirst(pfn);
    //         refPair->SetPairSecond(params);

    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Appended new action pair to index %d of inner array %d", newIndex, targetPos);
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Created pair: %p, %p", refPair->GetPairFirst<PFN_ACTIONHANDLER>(), refPair->GetPairSecond<YYObjectBase*>());
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Pair at original location: %p, %p", 
    //                         pAction->m_Token.m_Value[targetPos][newIndex].GetPairFirst<PFN_ACTIONHANDLER>(), 
    //                         pAction->m_Token.m_Value[targetPos][newIndex].GetPairSecond<YYObjectBase*>());
    //         Organik::GetLogger()->LogFormatted("CEvent::InsertIntoMap: Array Length: %d, Location %p, pair %p", refArr.GetArrayLength(), &refPair, refPair);
    //         if (pAction->m_Token.m_Value.ToPointer() != (*PatchedActionArrays())[params])
    //             Error_Show_Action("CEvent::InsertIntoMap: Array ref pointer mismatch after editing!", true, true);
            
    //         // (*PatchedActionArrays())[params] = pAction->m_Token.m_Value.ToPointer();
    //         return elem->m_Value;
    //     }
    // }
    // Error_Show_Action(true, true, "CEvent::InsertIntoMap: Failed to overwrite existing event %d/%d on %d", eventID, eventSubID, m_OwnerObjectID);
    // return nullptr;

    //     }
    //     RValue* pLastArrSlot = &(pAction->m_Token.m_Value[pAction->m_Token.m_Value.GetArrayLength() - 1]);

    //     if (!pLastArrSlot->GetKind() == VALUE_ARRAY)
    //     {
    //         RValue dummy_array = RValue();
    //         std::vector<double> dummy_data = {1.0};
    //         YYCreateArray(pLastArrSlot, 1, dummy_data.data());
    //     }
    //     pAction->m_Token.m_Value[pAction->m_Token.m_Value.GetArrayLength() - 1] = RValue(std::pair{
    //         after ? original : pfn, 
    //         after ? nullptr : params.ToObject()
    //     });
    //     pAction->m_Token.m_ArrLength = min(pAction->m_Token.m_ArrLength, 1);
        
    //     if (!appendToChain)
    //     {
    //         pAction->m_Token.m_Value.push_back(RValue());
    //         pAction->m_Token.m_ArrLength++;
    //     }
    //     pAction->m_Token.m_Value[pAction->m_Token.m_Value.GetArrayLength() - 1].push_back(after ? RValue(std::pair{original, nullptr}) : newActionPair);
    //     pAction->m_Token.m_Value[pAction->m_Token.m_Value.GetArrayLength() - 1].push_back(after ? newActionPair : RValue(std::pair{original, nullptr}));

    //     if (!pAction->m_Token.m_ArrLength)
    //         Error_Show_Action(true, true, "CEvent::InsertIntoMap (%d, %d): Failed to create token array!", eventID, eventSubID);
    //     return elem->m_Value;
    // }
    // Error_Show_Action(true, true, "CEvent::InsertIntoMap: Failed to overwrite existing event %d/%d on %d", eventID, eventSubID, m_OwnerObjectID);
// }

// // bool processHandler(RValue* item, YYObjectBase* volatile actionConstParams, int32_t objectIndex, int32_t eventCode, int32_t eventSubCode, bool after) {
// //     auto handlers = GetActionHandlers();

// //     if (item->m_Kind != VALUE_OBJECT) {
// //         Organik::GetLogger()->LogFormatted("ParseActions: Skipping non-object action item of kind %s", item->GetKindName());
// //         return false;
// //     }
// //     RValue *type = item->ToObject()->InternalGetYYVarRef(VAR_HASH(type));
// //     if (!type || type->m_Kind != VALUE_STRING) {
// //         Organik::GetLogger()->LogFormatted("ParseActions: 'type' is missing or not a string in action item");
// //         return false;
// //     }
// //     int32_t typeHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, type->ToCString());
    
// //     if (handlers->contains(typeHash)) {

// //         Organik::GetLogger()->LogFormatted("ParseActions: Found handler for type hash %d in action item of type %s", typeHash, type->ToCString());

// //         RValue params;
// //         StructCreate(&params); // not a memory leak, we keep the yyobjectbase pointer
// //         auto it = item->ToObject()->m_YYVarsMap->begin();

// //         while (it.getNextElement()) {
// //             Organik::GetLogger()->LogFormatted("ParseActions: Parsing member: Key hash %d, Type: %s",
// //                 it->m_Hash, it->m_Value->GetKindName());
// //             *params.ToObject()->InternalGetYYVarRef(it->m_Hash) = *(it->m_Value);
// //         }

// //         auto objectData = Object_Data(objectIndex);
// //         recurse(objectData, [&](CObjectGM* child) {
// //             Organik::GetLogger()->LogFormatted("ParseActions: Recursing into child object %d", child->m_ID);
// //             if (child->m_ID <= 5) {
// //                 Organik::GetLogger()->LogFormatted("ParseActions: Skipping some weird object %d", child->m_ID);
// //                 Organik::GetLogger()->LogFormatted("ParseActions: (IDs 5 and under are supposed to be reserved?) also only objects that are unused or crash seem to have these IDs", child->m_ID);
// //                 Organik::GetLogger()->LogFormatted("ParseActions: Note for later: weapon_MOD_103 and 104 are examples of this to look at.", child->m_ID);
// //                 return;
// //             }
// //             CEvent::InsertIntoMap(child->m_EventsMap, eventCode, eventSubCode, (*GetActionHandlers())[typeHash], 
// //                     actionConstParams, child->m_ID, true, after);
// //         });
// //     } else {
// //         Organik::GetLogger()->LogFormatted("ParseActions: No handler found for type hash %d in action item of type %s", typeHash, type->ToCString());
// //         return false;
// //     }
// //     return true;
// // }
// bool ParseActions(YYObjectBase* jsonValue, int32_t objectIndex, int32_t eventCode, int32_t eventSubCode, bool after)
// {
//     Organik::GetLogger()->LogFormatted("ParseActions: Starting to parse actions for object %d, event %d/%d", 
//         objectIndex, eventCode, eventSubCode);

//     if (!jsonValue) {
//         Organik::GetLogger()->LogFormatted("ParseActions: No JSON value provided");
//         return false;
//     }

//     auto refVecRequires = (jsonValue->InternalReadYYVar(VAR_HASH(action_requires)) ? 
//         jsonValue->InternalReadYYVar(VAR_HASH(action_requires))->ToRefVector() : std::vector<RValue*>());

//     Organik::GetLogger()->LogFormatted("ParseActions: Parsing requires actions array of size %d from %p", (jsonValue->InternalReadYYVar(VAR_HASH(action_requires))->GetArrayLength()), jsonValue);


//     for (auto item : refVecRequires)
//     {
//         YYObjectBase* actionConstParams = item->ToObject();
//         if (!processHandler(item, actionConstParams))
//             Organik::GetLogger()->LogFormatted("ParseActions: Failed to process action item in requires array");
        
//         ClearPatchCache();
//     }

//     auto refVecActions = (jsonValue->InternalReadYYVar(VAR_HASH(actions)) ?
//         jsonValue->InternalReadYYVar(VAR_HASH(actions))->ToRefVector() : std::vector<RValue*>());

//     Organik::GetLogger()->LogFormatted("ParseActions: Parsing actions array of size %d from %p", (jsonValue->InternalReadYYVar(VAR_HASH(actions))->GetArrayLength()), jsonValue);

//     for (auto item : refVecActions)
//     {
//         RValue actionConstParams = const RValue(item->ToObject());
//         if (!processHandler(item, actionConstParams, objectIndex, eventCode, eventSubCode, after))
//             Organik::GetLogger()->LogFormatted("ParseActions: Failed to process action item in actions array");
//         ClearPatchCache();
//     }

//     return true;
// }
//         Organik::GetLogger()->LogFormatted("ParseTriggers: After parsing, trigger %d has kind %s (flags=%d)", 
//             triggerIndex-1, objParsed.GetKindName(), objParsed.m_Flags);
        
//         if (objParsed.m_Kind != VALUE_OBJECT) {
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Skipping parsed non-object trigger %d", triggerIndex-1);
//             continue;
//         }
        
//         auto it = objParsed.ToObject()->m_YYVarsMap->begin();
//         while (it != objParsed.ToObject()->m_YYVarsMap->end()) {
//             //! ### type ###
//             RValue typeRV = RValue::ReadStructValue(it->m_Value, VAR_HASH(type));
//             if (!(typeRV.GetKind() == VALUE_STRING))
//                 Error_Show_Action("ParseTriggers: 'type' is missing or not a string.", true, true);

//             int32_t typeHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, typeRV.ToCString());
//             if (typeHash == VAR_HASH(event)) {
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d is an event trigger", triggerIndex-1);
//             }
//             else 
//             {
//                 Error_Show_Action("ParseTriggers: Unsupported trigger type.", true, true);
//                 break;
//             }

//             //! ### object ###
//             RValue objectRV = RValue::ReadStructValue(it->m_Value, VAR_HASH(object));
//             int32_t objectHash = -1;
//             if (objectRV.GetKind() == VALUE_STRING)
//                 objectHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, objectRV.ToCString());
//             else
//                 objectHash = parseRValueNumber<int32_t>(objectRV);

//             if (objectHash <= 0)
//                 Error_Show_Action("ParseTriggers: 'object' is missing or not a string.", true, true);

//             //! ### when ### !//
//             RValue whenRV = RValue::ReadStructValue(it->m_Value, VAR_HASH(when));
//             if (!(whenRV.GetKind() == VALUE_STRING))
//                 Error_Show_Action("ParseTriggers: 'when' is missing or not a string.", true, true);

//             int32_t whenHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, whenRV.ToCString());

//             //! ### event ### !//
//             RValue eventRV = RValue::ReadStructValue(it->m_Value, VAR_HASH(event));
//             if (!(eventRV.GetKind() == VALUE_STRING))
//                 Error_Show_Action("ParseTriggers: 'event' is missing or not a string.", true, true);

//             int32_t eventHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, eventRV.ToCString());
//             const std::pair<const int32_t, const int32_t> eventCodePair = GetEventHashes()->contains(eventHash) ? 
//                 (*GetEventHashes())[eventHash] : std::make_pair(-1,-1);

//             if ((eventCodePair.first + eventCodePair.second) <= 0)
//                 Error_Show_Action("ParseTriggers: 'event' is not valid.", true, true);
//             RValue subEventRV;
//             RValue eventName;
//             const int32_t eventCode = eventCodePair.first;
//             RValue nameGetter = ((*GetEventCodeReverseMap()).at(eventCode)[INT32_MAX]);
//             nameGetter(&eventName, nullptr);
//             if (eventCodePair.second <= 0)
//             {
//                 //? ### subevent ### ?//
//                 subEventRV = RValue::ReadStructValue(it->m_Value, VAR_HASH(subevent));
//                 if (!(subEventRV.GetKind() == VALUE_STRING))
//                     Error_Show_Action( GetLogger()->ParseFormatting("ParseTriggers: 'subevent' is missing or not a string.\n'%s' does not have an implicit subevent.", eCodeReverseMap[eventCodePair.first]).c_str(), true, true);

//                 int32_t subEventHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, subEventRV.ToCString());
//                 if (subEventHash <= 0)
//                     Error_Show_Action(
//                         GetLogger()->ParseFormatting("ParseTriggers: 'subevent' is not valid.\n'%s' requires a valid subevent.", 
//                             .c_str(), true, true
//             }
            
            
//             //? ### action_requires ### ?//
            
//             //! ### actions ### !//

//             Organik::GetLogger()->LogFormatted("ParseTriggers: Parsed member: Key hash %d, Type: %s", 
//                 it->m_Hash, it->m_Value->GetKindName());
//             ++it;
//         }
        
        
        
        
        
        
//         }
//         //! REWRITE ZONE

//         EventMap obj = EventMap(objParsed.ToObject());
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d map at %p with %d members", triggerIndex-1, obj, obj->m_YYVarsMap->m_CurrentSize);
        
//         // Log all keys in the map
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d keys:", triggerIndex-1);
//         for (const auto& [key, val] : *obj) {
//             Organik::GetLogger()->LogFormatted("  - Key hash: %d, Type: %s", key, val.GetKindName());
//         }

//         RValue typeRV = (*obj)[VAR_HASH(type)];
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d type RValue is kind %s", 
//             triggerIndex-1, typeRV.GetKindName());
            
//         int32_t type = Code_Variable_FindAlloc_Slot_From_Name(nullptr,
//             typeRV.m_Kind == VALUE_STRING ? typeRV.ToCString() : nullptr
//         );
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d type hash = %d", triggerIndex-1, type);
        
//         if (type == VAR_HASH(event))
//         {
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d is an event trigger", triggerIndex-1);

//             // Log each important field from the JSON

//             RValue whenRV = obj->hasValue(VAR_HASH(when)) ? Code_Variable_FindAlloc_Slot_From_Name(nullptr, (*obj)[VAR_HASH(when)].ToCString()): RValue(VAR_HASH(after));
//             RValue actionsArray = obj->hasValue(VAR_HASH(actions)) ? (*obj)[VAR_HASH(actions)] : RValue();
//             Organik::GetLogger()->LogFormatted("ParseTriggers: 'actions' field is %d", 
//                 obj->hasValue(VAR_HASH(actions)) ? actionsArray.GetKind() : -1);

//             RValue requiresArr = obj->hasValue(VAR_HASH(action_requires)) ? (*obj)[VAR_HASH(action_requires)] : RValue();
//             Organik::GetLogger()->LogFormatted("ParseTriggers: 'action_requires' field is %d",
//                 obj->hasValue(VAR_HASH(action_requires)) ? requiresArr.GetKind() : -1);

//             RValue objectRV = obj->hasValue(VAR_HASH(object)) ? (*obj)[VAR_HASH(object)] : RValue();
//             Organik::GetLogger()->LogFormatted("ParseTriggers: 'object' field is %d",
//                 obj->hasValue(VAR_HASH(object)) ? objectRV.GetKind() : -1);

//             RValue eventRV = obj->hasValue(VAR_HASH(event)) ? (*obj)[VAR_HASH(event)] : RValue();
//             Organik::GetLogger()->LogFormatted("ParseTriggers: 'event' field is %d",
//                 obj->hasValue(VAR_HASH(event)) ? eventRV.GetKind() : -1);

//             RValue subeventRV = obj->hasValue(VAR_HASH(subevent)) ? (*obj)[VAR_HASH(subevent)] : RValue();
//             Organik::GetLogger()->LogFormatted("ParseTriggers: 'subevent' field is %d",
//                 obj->hasValue(VAR_HASH(subevent)) ? subeventRV.GetKind() : -1);

//             std::string eventName = eventRV.m_Kind == VALUE_STRING ? eventRV.ToCString() : 
//                 (eventRV.m_Kind == VALUE_INT32 ? std::to_string(parseRValueNumber<int32_t>(eventRV)) : "");

//             Organik::GetLogger()->LogFormatted("ParseTriggers: Event name is '%s'", eventName.c_str());

//             if (!eventName.length()) {
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: ERROR - Event name is empty, skipping");
//                 Error_Show_Action("Trigger parsing: 'event' is missing or not a string.", true, true);
//                 continue;
//             }

//             int32_t eventHash = eventRV.m_Kind == VALUE_STRING ? 
//                 Code_Variable_FindAlloc_Slot_From_Name(nullptr, eventName.c_str()) : 
//                 (eventRV.m_Kind == VALUE_INT32 ? parseRValueNumber<int32_t>(eventRV) : 0);
            
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Event hash is %d", eventHash);

            
//             auto eventCode = (*GetEventHashes()).contains(eventHash) ? 
//                 (*GetEventHashes())[eventHash] : std::make_pair(-1,-1);
            
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Event type %s (event=%d, subevent=%d)", 
//                 eventName.c_str(), eventCode.first, eventCode.second);

//             if (actionsArray.GetKind() != VALUE_ARRAY) {
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: ERROR - 'actions' is not an array, breaking");
//                 Error_Show_Action("Trigger parsing: 'actions' is not an array.", true, true);
//                 break;
//             }
            
//             if (requiresArr.GetKind() != VALUE_ARRAY) {
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: ERROR - 'flags' is not an object, breaking");
//                 Error_Show_Action("Trigger parsing: 'flags' is not an object.", true, true);
//                 break;
//             }

//             int32_t objIndex = objectRV.m_Kind == VALUE_STRING ? 
//                 parseRValueNumber<int32_t>(DoBuiltin(&gml_asset_get_index, { objectRV.ToCString() })) : parseRValueNumber<int32_t>(objectRV);
            
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Object index resolved to %d", objIndex);
            
//             if (objIndex <= 0) {
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: ERROR - Invalid object index %d (%d, %s), breaking", 
//                     objIndex, parseRValueNumber<int32_t>(DoBuiltin(&gml_asset_get_index, { objectRV.ToCString() })), 
//                     parseRValueNumber<int32_t>(objectRV)
//                 );
//                 Error_Show_Action("Trigger parsing: 'object' did not resolve to a valid object.", true, true);
//                 break;
//             }

//             if (objIndex == VAR_HASH(self)) {
//                 Error_Show_Action("Trigger parsing: 'self' is ambiguous.", true, true);
//             }
//             else if (objIndex == VAR_HASH(other)) {
//                 objIndex = -3;
//                 Error_Show_Action("Trigger parsing: 'other' is ambiguous.", true, true);
//             }
//             else if (objIndex == VAR_HASH(global)) {
//                 objIndex = OBJ_HASH(outgame);
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: Object is 'global (outgame)' (index %d)", objIndex);
//             }
//             EventTrigger* trigger = new EventTrigger(objIndex, eventCode.first, eventCode.second);
//             using DoAction = bool(*)(CInstance*, CInstance*, RValue*, RValue*);
//             if (!trigger->params) trigger->params = new VariableMap();
//             Action<DoAction> requiresAction();

//             auto requiresArray = requiresArr.ToRefVector();
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Requires array at %p with %zu entries", 
//                 requiresArray, requiresArray.size());

//             // Log all flags
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Requires array contains:");
//             for (auto& condition : requiresArray) {
//                 Organik::GetLogger()->LogFormatted("  - Type: %s, Flags: %d",
//                     condition->GetKindName(), condition->m_Flags);

//                 if (condition->GetKind() != VALUE_MAP) {
//                     Organik::GetLogger()->LogFormatted("ParseTriggers: 'requires' condition is of kind %s, expected object", condition->GetKindName());
//                     Error_Show_Action("Trigger parsing: 'requires' condition is not an object.", true, true);
//                     break;
//                 }

//                 auto conditionMap = condition->ToMap();
//                 int32_t objectIndex = -2;
                
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: Processing 'requires' condition map at %p with %d entries", 
//                     conditionMap, conditionMap ? conditionMap->size() : 0);

//                 if (!conditionMap->size()) {
//                     Error_Show_Action("Trigger parsing: 'requires' condition is an empty object.", true, true);
//                     break;
//                 }
//                 if (!(conditionMap->hasValue(VAR_HASH(type)))) {
//                     Error_Show_Action("Trigger parsing: 'requires' condition missing 'type' field.", true, true);
//                     break;
//                 }
//                 int32_t reqSelfParam = -2;
//                 int32_t reqOtherParam = -3;

//                 if (conditionMap->hasValue(VAR_HASH(object))) {
//                     RValue objVal = (*(conditionMap->GetMap()))[VAR_HASH(object)];
//                     reqSelfParam = objVal.m_Kind == VALUE_STRING ? 
//                         parseRValueNumber<int32_t>(DoBuiltin(&gml_asset_get_index, { objVal.ToCString() })) : parseRValueNumber<int32_t>(objVal);
//                     if (reqSelfParam <= 0) {
//                         Organik::GetLogger()->LogFormatted("ParseTriggers: ERROR - Invalid object index %d (%d, %s), breaking", 
//                             reqSelfParam, parseRValueNumber<int32_t>(DoBuiltin(&gml_asset_get_index, { objVal.ToCString() })), 
//                             parseRValueNumber<int32_t>(objVal)
//                         );
//                         Error_Show_Action("Trigger parsing: 'requires' condition 'object' did not resolve to a valid object.", true, true);
//                         break;
//                     }
//                     conditionMap->objectIndex = reqSelfParam;
//                 } else {
//                     conditionMap->objectIndex = trigger->objectOwner;
//                     Organik::GetLogger()->LogFormatted("ParseTriggers: 'requires' condition object index defaulted to trigger owner %d", trigger->objectOwner);
//                 }
//                 RValue type = (*conditionMap)[VAR_HASH(type)];
//                 int32_t condType = type.m_Kind == VALUE_STRING ? 
//                     Code_Variable_FindAlloc_Slot_From_Name(nullptr, type.ToCString()) : 
//                     (type.m_Kind == VALUE_INT32 ? parseRValueNumber<int32_t>(type) : 0);

//                 if (!condType) {
//                     Error_Show_Action("Trigger parsing: 'requires' condition 'type' is invalid.", true, true);
//                     break;
//                 }
//                 Organik::GetLogger()->LogFormatted("ParseTriggers: 'requires' condition type is %d", condType);
//                 DoAction* condFnc;
//                 auto map = new VariableMap();
//                 (*map)[VAR_HASH(variables)] = new VariableMap();
//                 conditionMap->CopyTo(map);
//                 condFnc = (*GetActionTypeHandlers()).contains(condType) ? 
//                     (*GetActionTypeHandlers())[condType] : nullptr;
                
//                 if (!condFnc) {
//                     Error_Show_Action("Trigger parsing: 'requires' condition 'type' has no handler.", true, true);
//                     break;
//                 }
//                 if (requiresAction == nullptr) {
//                     Organik::GetLogger()->LogFormatted("ParseTriggers: Creating new requiresAction for condition of type %d", condType);
//                     requiresAction = Action::Create(*condFnc, map);
//                     requiresAction->type = condType;
//                 }
//                 else {
//                     Organik::GetLogger()->LogFormatted("ParseTriggers: Appending condition of type %d to requiresAction at %p", condType, requiresAction);
//                     requiresAction->Append(*condFnc, map);
//                 }
//             };

//             auto actionsVec = actionsArray.ToRefVector();
//             Action *chainStart = nullptr;
//             Action *chainPrev = nullptr;
//             for (auto actionItem : (actionsVec))
//             {
//                 auto p_ActionMap = actionItem->ToMap();

//                 if (!p_ActionMap->hasValue(VAR_HASH(type))) {
//                     Error_Show_Action("Action parsing: Missing 'type'.", true, true);
//                     break;
//                 }

//                 int32_t actionType = Code_Variable_FindAlloc_Slot_From_Name(nullptr, (*p_ActionMap)[VAR_HASH(type)].ToCString());
//                 if (!GetActionTypeHandlers()->contains(actionType)) {
//                     Error_Show_Action("Action parsing: Unrecognized action type.", true, true);
//                     break;
//                 }
//                 trigger->AppendAction(*(*GetActionTypeHandlers())[actionType], p_ActionMap);
//             }
//             RegisterEventTriggers(trigger);
//         }
//     }
//     return true;
// }
static int lookupRecursionGuard = 0;
static std::regex variableGetRegex = std::regex(R"(^variable_get$)", std::regex_constants::optimize);
void OptimizeVarLookup(YYObjectBase* context)
{
    Organik::GetLogger()->LogFormatted("doGetFrom: Checking if recursion is neeeded for context %p", context);
    RValue *fromStr = context->InternalGetYYVarRef(VAR_HASH(from));
    RValue *scaleRV = context->InternalGetYYVarRef(VAR_HASH(scale));
    if (lookupRecursionGuard > LIMIT_RECURSION_GETVAR)
    {
        Error_Show_Action("OptimizeVarLookup: Recursion limit reached when optimizing 'from' parameter", true, true);
        return;
    }
    if (fromStr->GetKind() == VALUE_OBJECT)
    {
        std::string typeStr = fromStr->ToObject()->InternalGetYYVarRef(VAR_HASH(type))->ToString();
        if (std::regex_match(typeStr, variableGetRegex))
        {
            lookupRecursionGuard++;
            YYObjectBase* fromObj = fromStr->ToObject();
            OptimizeVarLookup(fromObj);
            fromStr->SetPairFirst(&doGetFrom);
            fromStr->SetPairSecond(fromObj);
            fromStr->m_Kind = VALUE_ACTION;
            lookupRecursionGuard--;
        }
    }
    else
    {
        std::istringstream iss(fromStr->ToCString());
        std::string objectPart, variablePart;
        if (!std::getline(iss, objectPart, '.') || !std::getline(iss, variablePart)) {
            Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
            return;
        }

        Organik::GetLogger()->LogFormatted("doGetFrom: Parsed 'from' as object '%s' and variable '%s'", 
            objectPart.c_str(), variablePart.c_str());

        int32_t objectHash = -1;
        if (objectPart._Equal("self"))
            objectHash = VAR_HASH(self);
        else if (objectPart._Equal("other"))
            objectHash = VAR_HASH(other);
        else
            objectHash = parseRValueNumber<int32_t>(
                DoBuiltin(&gml_asset_get_index, std::vector<RValue>{objectPart.c_str()})
            );

        Organik::GetLogger()->LogFormatted("doGetFrom: Resolved object '%s' to hash %d", objectPart.c_str(), objectHash);
        int32_t variableHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, variablePart.c_str());
        if (objectHash <= 0) {
            Error_Show_Action("doGetFrom: 'from' parameter contains invalid object name", true, true);
            return;
        }

        Organik::GetLogger()->LogFormatted("doGetFrom: Parsed 'from' as object hash %d and variable hash %d", objectHash, variableHash);
        *(context->InternalGetYYVarRef(VAR_HASH(object))) = RValue(objectHash);
        *(context->InternalGetYYVarRef(VAR_HASH(variable))) = RValue(variableHash);
    }
    if (scaleRV->GetKind() == VALUE_OBJECT)
    {
        std::string typeStr = scaleRV->ToObject()->InternalGetYYVarRef(VAR_HASH(type))->ToString();
        if (std::regex_match(typeStr, variableGetRegex))
        {
            lookupRecursionGuard++;
            YYObjectBase* scaleObj = scaleRV->ToObject();
            OptimizeVarLookup(scaleObj);
            scaleRV->SetPairFirst(&doGetFrom);
            scaleRV->SetPairSecond(scaleObj);
            scaleRV->m_Kind = VALUE_ACTION;
            lookupRecursionGuard--;
        }
    }
    if (fromStr->m_Kind != VALUE_STRING) {
        Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
        return;
    }
    std::istringstream iss(fromStr->ToCString());
    std::string objectPart, variablePart;
    if (!std::getline(iss, objectPart, '.') || !std::getline(iss, variablePart)) {
        Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
        return;
    }

    Organik::GetLogger()->LogFormatted("doGetFrom: Parsed 'from' as object '%s' and variable '%s'", 
        objectPart.c_str(), variablePart.c_str());

    int32_t objectHash = -1;
    if (objectPart._Equal("self"))
        objectHash = VAR_HASH(self);
    else if (objectPart._Equal("other"))
        objectHash = VAR_HASH(other);
    else
        objectHash = parseRValueNumber<int32_t>(
            DoBuiltin(&gml_asset_get_index, std::vector<RValue>{objectPart.c_str()})
        );

    Organik::GetLogger()->LogFormatted("doGetFrom: Resolved object '%s' to hash %d", objectPart.c_str(), objectHash);
    int32_t variableHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, variablePart.c_str());
    if (objectHash <= 0) {
        Error_Show_Action("doGetFrom: 'from' parameter contains invalid object name", true, true);
        return;
    }

    Organik::GetLogger()->LogFormatted("doGetFrom: Parsed 'from' as object hash %d and variable hash %d", objectHash, variableHash);
    *(context->InternalGetYYVarRef(VAR_HASH(object))) = RValue(objectHash);
    *(context->InternalGetYYVarRef(VAR_HASH(variable))) = RValue(variableHash);
}
bool doApplyVariables(CInstance* self, CInstance* other, YYObjectBase* context)
{
    if (!context) return false;
    Organik::GetLogger()->LogFormatted("doApplyVariables: Starting with context %p", context);
    RValue *optimizationRV = context->InternalGetYYVarRef(VAR_HASH(optimized));
    if (optimizationRV->m_Kind != VALUE_BOOL)
    {
        Organik::GetLogger()->LogFormatted("doApplyVariables: optimizing");
        RValue *optimizedVarAlloc = context->InternalGetYYVarRef(VAR_HASH(optimized));
        *optimizedVarAlloc = RValue(true);
        optimizedVarAlloc->m_Kind = VALUE_BOOL;

        auto scaleVar = context->InternalGetYYVarRef(VAR_HASH(scale));
        Organik::GetLogger()->LogFormatted("doApplyVariables: scaleVar is %p of kind %s", scaleVar, scaleVar ? scaleVar->GetKindName() : "null");
        auto objectVar = context->InternalGetYYVarRef(VAR_HASH(object));
        Organik::GetLogger()->LogFormatted("doApplyVariables: objectVar is %p of kind %s", objectVar, objectVar ? objectVar->GetKindName() : "null");

        
        if (objectVar->GetKind() != VALUE_STRING && objectVar->GetKind() != VALUE_INT32 
                && objectVar->GetKind() != VALUE_REAL && objectVar->GetKind() != VALUE_INT64)
        {
            Error_Show_Action(true, true, "doApplyVariables: 'object' parameter is of invalid type %s.", objectVar->GetKindName());
        }

        if (objectVar->GetKind() == VALUE_STRING)
        {
            Organik::GetLogger()->LogFormatted("doApplyVariables: resolving object name '%s' to index", objectVar->ToCString());
            std::vector<RValue> params = { *objectVar };
            int32_t objectHash = parseRValueNumber<int32_t>(
                DoBuiltin(&gml_asset_get_index, params)
            );
            if (objectHash <= 0) Error_Show_Action("doApplyVariables: 'object' parameter contains invalid object name.", true, true);
            *objectVar = RValue(objectHash);
        }
        else
        {
            Organik::GetLogger()->LogFormatted("doApplyVariables: resolving object index from value of kind %s", objectVar->GetKindName());
            int32_t objectHash = parseRValueNumber<int32_t>(*objectVar);
            if (objectHash <= 0) Error_Show_Action("doApplyVariables: 'object' parameter contains invalid object index.", true, true);
            *objectVar = RValue(objectHash);
        }
        Organik::GetLogger()->LogFormatted("doApplyVariables: objectVar is now %d", parseRValueNumber<int32_t>(objectVar));
        if (scaleVar)
        {
            Organik::GetLogger()->LogFormatted("doApplyVariables: optimizing scaleVar %p of kind %s", scaleVar, scaleVar->GetKindName());
            if (scaleVar->GetKind() == VALUE_OBJECT)
            {
                OptimizeVarLookup(scaleVar->ToObject());
                scaleVar->SetPairFirst(&doGetFrom);
                scaleVar->SetPairSecond(scaleVar->ToObject());
                scaleVar->m_Kind = VALUE_ACTION;
            }
        }
        auto &variablesMap = context->InternalReadYYVar(VAR_HASH(variables))->ToObject()->m_YYVarsMap;
        CHashMap<int, RValue *, 3>::iterator iter = variablesMap->begin();
        const CHashMap<int, RValue *, 3>::Element *elem;
        while (variablesMap->FindNextValue(iter, &elem))
        {
            Organik::GetLogger()->LogFormatted("doApplyVariables: optimizing variable entry key %d of kind %s",
                elem->m_Key, elem->m_Value ? elem->m_Value->GetKindName() : "null");
            if (elem->m_Value->GetKind() == VALUE_OBJECT)
            {
                Organik::GetLogger()->LogFormatted("doApplyVariables:  - is object, optimizing");
                YYObjectBase* varObj = elem->m_Value->ToObject();
                OptimizeVarLookup(varObj);
                elem->m_Value->SetPairFirst(&doGetFrom);
                elem->m_Value->SetPairSecond(varObj);
                elem->m_Value->m_Kind = VALUE_ACTION;
            }
            else
            {
                Organik::GetLogger()->LogFormatted("doApplyVariables:  - is not object, hashing name");
                int32_t varHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, iter->m_Value->ToCString());
                *iter->m_Value = RValue(varHash);
            }
        }
    }
    Organik::GetLogger()->LogFormatted("doApplyVariables: optimized.");
    int32_t objectIndex = parseRValueNumber<int32_t>(context->InternalGetYYVarRef(VAR_HASH(object)));
    Organik::GetLogger()->LogFormatted("doApplyVariables: target object index is %d", objectIndex);
    if (objectIndex <= 0)
    {
        Error_Show_Action("doApplyVariables: 'object' parameter contains invalid object index.", true, true);
        return false;
    }
    CInstance* targetInstance = CInstance::FirstOrDefault([&](CInstance* inst) -> bool {
        return inst->m_ObjectIndex == objectIndex;
    });

    Organik::GetLogger()->LogFormatted("doApplyVariables: target instance is %p", targetInstance);
    if (!targetInstance)
    {
        Error_Show_Action("doApplyVariables: No instance found for specified object index.", true, true);
        return false;
    }
    auto scaleRV = context->InternalGetYYVarRef(VAR_HASH(scale));
    double multiplier = 1.0;
    if (scaleRV->m_Kind == VALUE_OBJECT || scaleRV->m_Kind == VALUE_ACTION)
    {
        if (scaleRV->GetKind() == VALUE_ACTION) 
        {
            scaleRV->GetPairFirst<PFN_ACTIONHANDLER>()(self, other, scaleRV->GetPairSecond<YYObjectBase*>());
            scaleRV = scaleRV->GetPairSecond<YYObjectBase*>()->InternalReadYYVar(VAR_HASH(result));
        }
        multiplier = parseRValueNumber<double>(scaleRV);
    }
    Organik::GetLogger()->LogFormatted("doApplyVariables: scale multiplier is %f", multiplier);
    RValue* variables = context->InternalGetYYVarRef(VAR_HASH(variables));
    if (!variables || variables->GetKind() != VALUE_OBJECT)
    {
        Error_Show_Action("doApplyVariables: 'variables' parameter is missing or not an object.", true, true);
        return false;
    }
    auto varsToApplyObj = context->InternalGetYYVarRef(VAR_HASH(variables));
    if (varsToApplyObj && varsToApplyObj->GetKind() == VALUE_OBJECT)
    {
        CHashMap<int, RValue *, 3> *varsMap = varsToApplyObj->ToObject()->m_YYVarsMap;
        CHashMap<int, RValue *, 3>::iterator iter = varsMap->begin();
        const CHashMap<int, RValue *, 3>::Element *elem = nullptr;
        while (varsMap->FindNextValue(iter, &elem))
        {
            if (!elem->m_Value) Organik::GetLogger()->LogFormatted("doApplyVariables: found null value for key %d, hash %d, hash_deleted %s",
                elem->m_Key, elem->m_Hash, (elem->m_Hash & HASH_DELETED) ? "yes" : "no");
            int32_t varKey = elem->m_Key;
            auto targetRef = targetInstance->InternalGetYYVarRef(varKey);
            switch(elem->m_Value->m_Kind)
            {
                case VALUE_INT32:
                case VALUE_INT64:
                case VALUE_REAL:
                    *targetRef = parseRValueNumber<double>(elem->m_Value) * multiplier;
                    continue;
                case VALUE_STRING:
                    if (targetRef->m_Kind != VALUE_STRING)
                        YYCreateString(targetRef, elem->m_Value->ToCString());
                    else
                        YYSetString(targetRef, elem->m_Value->ToCString());
                    continue;
                case VALUE_BOOL:
                    *targetRef = elem->m_Value->ToBoolean();
                    continue;
                default:
                    *targetRef = *(elem->m_Value);
                    continue;
            }
        }
    }
    return true;
}
// bool doCallScript(CInstance* self, CInstance* other, YYObjectBase* context)
// {
//     auto paramsIt = (*params).find(VAR_HASH(parameters));
//     auto scriptIt = (*params).find(VAR_HASH(script));
//     if (paramsIt == params->end() || scriptIt == params->end()) 
//         Error_Show_Action("doCallScript: Missing 'parameters' or 'script'.", true, true);

//     RValue paramsRV = paramsIt->second;
//     RValue scriptRV = scriptIt->second;
//     if (!paramsRV || paramsRV.m_Kind != VALUE_OBJECT) return true;
//     if (!scriptRV || (scriptRV.m_Kind != VALUE_STRING)) return true;

//     PFUNC_YYGMLScript theScript = nullptr;

//     auto* paramsMap = paramsRV.ToMap();
//     std::vector<RValue*> paramsVector;
//     for (const auto& [paramHash, value] : *paramsMap) {
//         RValue* param = self->InternalGetYYVarRef(paramHash);
//         if (param) {
//             paramsVector.emplace_back(param);
//         }
//     }
//     PVOID scriptPtr = GetProcAddress(GetModuleHandleA("Direktor.dll"), scriptRV.ToCString());
//     if (scriptPtr) {
//         theScript = (PFUNC_YYGMLScript)scriptPtr;
//     } else {
//         Error_Show_Action("doCallScript: Unable to find script function.", true, true);
//     }
//     if (theScript) {
//         (*theScript)(self, other ? other : self, result, static_cast<int>(paramsVector.size()), paramsVector.data());
//         return true;
//     }
//     return false;
// }

// bool doCallBuiltin(CInstance* self, CInstance* other)
// {
//     auto paramsIt = (*params).find(VAR_HASH(parameters));
//     auto scriptIt = (*params).find(VAR_HASH(function));
//     if (paramsIt == params->end() || scriptIt == params->end()) 
//         Error_Show_Action("doCallBuiltin: Missing 'parameters' or 'function'.", true, true);

//     RValue paramsRV = paramsIt->second;
//     RValue scriptRV = scriptIt->second;
//     if (!paramsRV || paramsRV.m_Kind != VALUE_OBJECT) return true;
//     if (!scriptRV || (scriptRV.m_Kind != VALUE_STRING)) return true;

//     PFN_Builtin theFunc = nullptr;

//     auto* paramsMap = paramsRV.ToMap();
//     std::vector<RValue> paramsVector;
//     for (const auto& [paramHash, value] : *paramsMap) {
//         RValue* param = self->InternalGetYYVarRef(paramHash);
//         if (param) {
//             paramsVector.emplace_back(*param);
//         }
//     }
//     PVOID functionPtr = GetProcAddress(GetModuleHandleA("Direktor.dll"), (std::string("gml_") + std::string(scriptRV.ToCString())).c_str());
//     if (functionPtr) {
//         theFunc = (PFN_Builtin)functionPtr;
//     } else {
//         Error_Show_Action("doCallBuiltin: Unable to find function.", true, true);
//     }
//     if (theFunc) {
//         (*theFunc)(*result, self, other ? other : self, static_cast<int>(paramsVector.size()), paramsVector.data());
//         return true;
//     }
//     return false;
// }