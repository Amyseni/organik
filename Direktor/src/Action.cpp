// #include "Synthetik.h"
// #include "Globals.h"
// #include "DefinitionHelpers/InstanceHelper.h"
// #include "DefinitionHelpers/VariableHelper.h"
// #include "Action.h"

// void ParseJSONRValue(RValue* jsonValue, RValue* out) {
//     if (!jsonValue)
//     {
//         *out = RValue(false);
//         return;
//     }
    
//     Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing RValue of kind %s", jsonValue->GetKindName());
    
//     RValueType kind = jsonValue->GetKind();
    
//     // String handling
//     if (kind == VALUE_STRING)
//     {
//         std::string str = std::string(YYGetString(jsonValue, 0));
//         if (str.starts_with("###"))
//         {
//             *out = Code_Variable_FindAlloc_Slot_From_Name(nullptr, str.substr(3).c_str());
//             (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
//         }
//         else if (str.starts_with("##"))
//         {
//             RValue tmp1;
//             YYCreateString(&tmp1, str.substr(2).c_str());
//             *out = DoBuiltin(&gml_asset_get_index, { tmp1 });
//             (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
//         }
//         else
//         {
//             YYCreateString(out, str.c_str());
//             (*out).m_Kind = static_cast<RValueType>(VALUE_STRING & MASK_KIND_RVALUE);
//         }
//     }
//     // Array handling
//     else if (kind == VALUE_ARRAY)
//     {
//         // Create a new vector container
//         std::vector<double> dummy_array = {0.0};
//         *out = RValue();
//         YYCreateArray(out, 0, dummy_array.data());

//         // Process array items
//         std::vector<RValue*> jsonVec = jsonValue->ToRefVector();
        
//         for (auto &item : jsonVec)
//         {
//             Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing array item of type: %s", item->GetKindName());

//             RValue parsedValue(5);
//             ParseJSONRValue(item, &parsedValue);
//             out->push_back(parsedValue);
//             Organik::GetLogger()->LogFormatted("ParseJSONRValue: added item of type: %s to vector", out->back().GetKindName());
//         }
//         (*out).m_Kind = static_cast<RValueType>(VALUE_ARRAY & MASK_KIND_RVALUE);
//         (*out).m_Flags = 0;
//     }
//     // Object handling
//     else if (kind == VALUE_OBJECT)
//     {
//         // Create a new map container
//         YYObjectBase* pObj = jsonValue->ToObject();
        
        
//         // Get object property names
//         std::vector<RValue*> refVec = DoBuiltin(&gml_variable_instance_get_names, { *jsonValue }).ToRefVector();
        
//         for (auto name : refVec)
//         {
//             RValue tmp2 = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name->ToCString());
//             int32_t propHash = parseRValueNumber<int32_t>(tmp2);
            
//             Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing object member: %s (%d)", 
//                 name->ToCString(), propHash);
            
//             // Parse the property value directly into the map
//             RValue* propValue = (jsonValue->ToObject())->InternalGetYYVarRef(propHash);
//             ParseJSONRValue(propValue, (*pObj)[propHash]);
            
//             Organik::GetLogger()->LogFormatted("ParseJSONRValue: added item of type: %s to map", 
//                 (*pObj)[propHash]->GetKindName());
//         }
//     }
//     else if (kind == VALUE_BOOL)
//     {
//         Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsing boolean value: %s", 
//             jsonValue->ToBoolean() ? "true" : "false");
//         *out = RValue(jsonValue->ToBoolean());
//         (*out).m_Kind = static_cast<RValueType>(VALUE_BOOL & MASK_KIND_RVALUE);
//         Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsed boolean RValue of kind %s", (*out).GetKindName());
//     }
//     // Integer handling
//     else if (kind == VALUE_INT32)
//     {
//         *out = parseRValueNumber<int32_t>(*jsonValue);
//         (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
//     }
//     // Long integer handling
//     else if (kind == VALUE_INT64)
//     {
//         int64_t val64 = parseRValueNumber<int64_t>(*jsonValue);
//         if (val64 < INT32_MAX)
//         {
//             *out = parseRValueNumber<int32_t>(*jsonValue);
//             (*out).m_Kind = static_cast<RValueType>(VALUE_INT32 & MASK_KIND_RVALUE);
//         }
//         else
//         {
//             *out = parseRValueNumber<int64_t>(*jsonValue);
//             (*out).m_Kind = static_cast<RValueType>(VALUE_INT64 & MASK_KIND_RVALUE);
//         }
//     }
//     // Real (double) handling
//     else if (kind == VALUE_REAL)
//     {
//         *out = parseRValueNumber<double>(*jsonValue);
//         (*out).m_Kind = static_cast<RValueType>(VALUE_REAL & MASK_KIND_RVALUE);
//     }
//     // Default case for other types
//     else
//     {
//         *out = *reinterpret_cast<int64_t*>(std::launder(&jsonValue->m_Real));
//         (*out).m_Kind = static_cast<RValueType>(jsonValue->m_Kind & MASK_KIND_RVALUE);
//     }
    
//     Organik::GetLogger()->LogFormatted("ParseJSONRValue: Parsed RValue of kind %s", (*out).GetKindName());
// }
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
// bool doGetFrom(CInstance* self, CInstance* other, RValue* params, RValue* result) {
//     if (!self || !params) return false;
//     RValue fromStr = params->ContainsValue(VAR_HASH(from)) ? (*params)[VAR_HASH(from)] : RValue(false);
//     if (fromStr.m_Kind != VALUE_STRING) {
//         Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
//         *result = RValue();
//         return false;
//     }
//     std::istringstream iss(fromStr.ToCString());
//     std::string objectPart, variablePart;
//     if (!std::getline(iss, objectPart, '.') || !std::getline(iss, variablePart)) {
//         Error_Show_Action("doGetFrom: 'from' parameter should be in the form of object.variable", true, true);
//         *result = RValue();
//         return false;
//     }
//     int32_t objectHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, objectPart.c_str());
//     int32_t variableHash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, variablePart.c_str());

//     CInstance* targetInstance = objectHash == VAR_HASH(self) ? self : CInstance::FirstOrDefault([&](CInstance* inst) -> bool {
//         return (inst && inst->m_ObjectIndex == objectHash);
//     });
//     RValue *resultRV = targetInstance->InternalReadYYVar(variableHash);

//     if (!params->ContainsValue(VAR_HASH(scale))) {
//         *result = resultRV ? *resultRV : RValue();
//         return resultRV != nullptr;
//     }
    
//     RValue *scaleRV = &(*params)[VAR_HASH(scale)];
//     if (scaleRV->GetKind() == VALUE_OBJECT)
//     {
//         RValue out;
//         doGetFrom(targetInstance, nullptr, scaleRV, &out);
//         int i = 0;
//         while (out.GetKind() == VALUE_OBJECT && i < LIMIT_RECURSION_GETVAR) {
//             RValue tmp = RValue(out);
//             out = RValue();
//             doGetFrom(targetInstance, nullptr, &tmp, &out);
//             i++;
//         }
//         if (out.GetKind() == VALUE_REAL || out.GetKind() == VALUE_INT32 || out.GetKind() == VALUE_INT64) {
//             double scaleFactor = parseRValueNumber<double>(out);
//             if (resultRV && (resultRV->GetKind() == VALUE_REAL || resultRV->GetKind() == VALUE_INT32 || resultRV->GetKind() == VALUE_INT64)) {
//                 *result = RValue(resultRV->ToDouble() * scaleFactor);
//                 return true;
//             } else {
//                 Error_Show_Action("doGetFrom: Cannot scale non-numeric value.", true, true);
//                 *result = RValue();
//                 return false;
//             }
//         }
//         return false;
//     }
//     else if (scaleRV->GetKind() == VALUE_REAL || scaleRV->GetKind() == VALUE_INT32 || scaleRV->GetKind() == VALUE_INT64) {
//         double scaleFactor = parseRValueNumber<double>(scaleRV);
//         if (resultRV && (resultRV->GetKind() == VALUE_REAL || resultRV->GetKind() == VALUE_INT32 || resultRV->GetKind() == VALUE_INT64)) {
//             *result = RValue(parseRValueNumber<double>(resultRV) * scaleFactor);
//             return true;
//         } else {
//             Error_Show_Action("doGetFrom: Cannot scale non-numeric value.", true, true);
//             *result = RValue();
//             return false;
//         }
//     } else {
//         Error_Show_Action("doGetFrom: 'scale' parameter should be a number or a map resolving to a number.", true, true);
//         *result = RValue();
//         return false;
//     }
// }
// bool doRequiresChance(CInstance* self, CInstance* other, RValue* params, RValue* result) {
//     if (!self || !params) return false;
//     RValue chanceRV = params->ContainsValue(VAR_HASH(chance)) ? (*params)[VAR_HASH(chance)] : RValue(10.0);
//     RValue scaleRV = params->ContainsValue(VAR_HASH(scale)) ? (*params)[VAR_HASH(scale)] : RValue(1.0);
//     double scaleFactor = 1.0;
//     if (scaleRV.GetKind() == VALUE_MAP) {
//         RValue out;
//         doGetFrom(self, nullptr, &scaleRV, &out);
//         while (out.GetKind() == VALUE_MAP) {
//             RValue tmp = out;
//             out = RValue();
//             doGetFrom(self, nullptr, &tmp, &out);
//         }
//         if (out.GetKind() == VALUE_REAL || out.GetKind() == VALUE_INT32 || out.GetKind() == VALUE_INT64) {
//             scaleFactor = parseRValueNumber<double>(out);
//         } else {
//             Error_Show_Action("doRequiresChance: 'scale' resolved to non-numeric value.", true, true);
//             *result = RValue(false);
//             return false;
//         }
//     }
//     double chance = parseRValueNumber<double>(chanceRV);
//     double finalChance = chance * scaleFactor;

//     double roll = ((*Utils::getrng())() % 100000) / 1000.0;
//     *result = RValue(roll < finalChance);
//     return result->ToBoolean();
// }
// std::unordered_map<int32_t, RValue> const* GetEventCodeReverseMap() {
//     static std::unordered_map<int32_t, RValue> eCodeReverseMap = {
//     	//! MAP: EVENT_CREATE
//         {
//             EVENT_CREATE, 
//             RValue({
//                 {0,      "create"},

// 				//* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance* a = nullptr, CInstance* b = nullptr, RValue* c = nullptr, RValue* out = nullptr) -> bool {
//                         *out = RValue("create");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_CREATE

//     	//! MAP: EVENT_DESTROY
//         {
//             EVENT_DESTROY,
//             RValue({
//                 {0,     "destroy"},
                
//                 //* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("destroy");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_DESTROY

//     	//! MAP: EVENT_ALARM
//         {
//             EVENT_ALARM, 
//             RValue({
//                 {SUBEV_ALARM0,     "alarm_0"},           {SUBEV_ALARM1, "alarm_1"},         {SUBEV_ALARM2, "alarm_2"},
//                 {SUBEV_ALARM3,     "alarm_3"},           {SUBEV_ALARM4, "alarm_4"},         {SUBEV_ALARM5, "alarm_5"},
//                 {SUBEV_ALARM6,     "alarm_6"},           {SUBEV_ALARM7, "alarm_7"},         {SUBEV_ALARM8, "alarm_8"},
//                 {SUBEV_ALARM9,     "alarm_9"},         {SUBEV_ALARM10, "alarm_10"},       {SUBEV_ALARM11, "alarm_11"},

// 				//* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("alarm");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_ALARM 

//     	//! MAP: EVENT_STEP
//         {
//             EVENT_STEP, 
//             RValue({
//                 {0, "step_normal"},       {1, "step_begin"},        {2, "step_end"},
                
//                 //* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("step");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })        
//         },
// 		//!END: EVENT_STEP

//     	//! MAP: EVENT_MOUSE
//         {
//             EVENT_MOUSE, 
//             RValue({
//                 {INT32_MAX-1,   "mouse_any"}, 

// 				//* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("mouse");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_MOUSE

//     	//! MAP: EVENT_OTHER
//         {
//             EVENT_OTHER, 
//             RValue({
//                 {SUBEV_USER0,      "user_0"},          {SUBEV_USER1,  "user_1"},         {SUBEV_USER2,  "user_2"},   
//                 {SUBEV_USER3,      "user_3"},          {SUBEV_USER4,  "user_4"},         {SUBEV_USER5,  "user_5"},
//                 {SUBEV_USER6,      "user_6"},          {SUBEV_USER7,  "user_7"},         {SUBEV_USER8,  "user_8"},
//                 {SUBEV_USER9,      "user_9"},         {SUBEV_USER10, "user_10"},        {SUBEV_USER11, "user_11"},
//                 {SUBEV_USER12,    "user_12"},         {SUBEV_USER13, "user_13"},        {SUBEV_USER14, "user_14"},
//                 {SUBEV_USER15,    "user_15"},

// 				//* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("other");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_OTHER

//     	//! MAP: EVENT_DRAW
//         {
//             EVENT_DRAW, 
//             RValue({
//                 {DRAW_GUI,     "draw_gui"},    {DRAW_RESIZE,      "draw_resize"},    {DRAW_BEGIN,   "draw_begin"},  
//                 {DRAW_END,     "draw_end"}, {DRAW_GUI_BEGIN,   "draw_gui_begin"},  {DRAW_GUI_END, "draw_gui_end"},
//                 {DRAW_PRE,     "draw_pre"},      {DRAW_POST,        "draw_post"},

// 				//* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("draw");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_DRAW

//     	//! MAP: EVENT_KEYPRESS
//         {
//             EVENT_KEYPRESS, 
//             RValue({
//                 {INT32_MAX-1,    "key_press"},

// 				//* base event name getter
//                 {INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("keypress");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_KEYPRESS

// 		//! MAP: EVENT_PRE_CREATE
// 		{
//             EVENT_PRE_CREATE,
// 			RValue({
// 				{INT32_MAX-1,   "pre_create"},
				
//                 //* base event name getter
// 				{INT32_MAX, Action(std::function(
//                     [](CInstance*, CInstance*, RValue*, RValue* out) -> bool {
//                         *out = RValue("precreate");
//                         return true;
//                     } //* Action
//                 ))}   //? Pair
//             })
//         },
// 		//! END: EVENT_PRE_CREATE
//     };
//     return &eCodeReverseMap;
// }
// bool ParseTriggers(RValue* jsonValue)
// {
//     if (jsonValue->m_Kind != VALUE_ARRAY) {
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Expected array, got %s", jsonValue->GetKindName());
//         return false;
//     }
    
//     auto refVec = jsonValue->ToRefVector();
//     Organik::GetLogger()->LogFormatted("ParseTriggers: Parsing triggers array of size %d at %p", refVec.size(), jsonValue);
    
//     int triggerIndex = 0;
//     using DoAction = bool(*)(CInstance*, CInstance*, RValue*, RValue*);
//     for (const auto& item : refVec)
//     {
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Processing trigger %d of kind %s at %p", triggerIndex++, item->GetKindName(), item);
//         if (item->m_Kind != VALUE_OBJECT) {
//             Organik::GetLogger()->LogFormatted("ParseTriggers: Skipping non-object trigger %d", triggerIndex-1);
//             continue;
//         }
        
//         RValue objParsed;
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Calling ParseJSONRValue on trigger %d", triggerIndex-1);
//         ParseJSONRValue(item, &objParsed);
        
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
//         Organik::GetLogger()->LogFormatted("ParseTriggers: Trigger %d map at %p with %d members", triggerIndex-1, obj, obj->m_YYVarsMap->m_UsedCount);
        
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
// bool doApplyVariables(CInstance* self, CInstance* other, VariableMap * params, RValue* result)
// {
//     auto varMapIt = (*params).find(VAR_HASH(variables));
//     if (varMapIt == (*params).end()) return true;

//     RValue* varMapRV = &varMapIt->second;
//     if (!varMapRV || varMapRV->m_Kind != VALUE_OBJECT) return true;

//     VariableMap *varMap = (VariableMap *)varMapRV->ToPointer();
//     for (const auto& [varHash, value] : *varMap) {
//         RValue* targetVar = self->InternalGetYYVarRef(varHash);
//         if (targetVar) {
//             *targetVar = value;
//         }
//     }

//     return true;
// }
// bool doCallScript(CInstance* self, CInstance* other, VariableMap * params, RValue* result)
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

// bool doCallBuiltin(CInstance* self, CInstance* other, VariableMap * params, RValue* result)
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