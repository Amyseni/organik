#include "Synthetik.h"
#include "Organik/UI/UIManager.h"
#include "Organik/UI/ChatBox.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Globals.h"
#include "Variant.h"
#include "Action.h"
#include "stack"
#include "Events.h"

// HOOK_GLOBAL(gml_Object_obj_chest_weapon_wood_arena_Create_0, (CInstance* Self, CInstance* Other) -> void)
// {
/*     Organik::GetLogger()->LogFormatted("gml_Object_obj_chest_weapon_wood_arena_Create_0 called: %p", Self);
    super(Self, Other);
    Organik::GetLogger()->LogFormatted("gml_Object_obj_chest_weapon_wood_arena_Create_0 after super: %p", Self->m_ID);

    if (!Self)
        return;
    RValue dummy;
    auto rng = Utils::getrng();
    int chestType = (*rng)() % getNonArenaChests().size();
    RValue x = parseRValueNumber<double>(*Self->InternalGetYYVarRef(VAR_HASH(x)));
    RValue y = parseRValueNumber<double>(*Self->InternalGetYYVarRef(VAR_HASH(y)));
    RValue newChestType = RValue(static_cast<double>(getNonArenaChests()[chestType]));
    RValue selfID = Self->m_ID;
    std::vector<RValue*> args = {
        &x,
        &y,
        &newChestType
    };

    RValue newChest;
//     gml_Script_scr_instance_create(GetGlobalInstance(), GetGlobalInstance(), &newChest, static_cast<int>(args.size()), args.data());
//     gml_instance_destroy(&dummy, Self, Self, 1, &selfID); */

// }
// HOOK_GLOBAL(gml_Object_obj_weapon_DE_61_Step_0, (CInstance * Self, CInstance *Other)->void)
// {
//     RValue result;
//     gml_event_inherited(&result, Self, Other, 0, nullptr);
//     CInstance *objStats = CInstance::FirstOrDefault([&](CInstance *instance)
//                                                     { return instance && instance->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_stats]; });
//     CInstance *objStatistics = CInstance::FirstOrDefault([&](CInstance *instance)
//                                                          { return instance && instance->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_statistics]; });
//     RValue *giveEffect = Self->InternalGetYYVarRef(VAR_HASH(giveeffect));
//     RValue *removeEffect = Self->InternalGetYYVarRef(VAR_HASH(removeeffect));
//     RValue *effectHoldActive = Self->InternalGetYYVarRef(VAR_HASH(effectholdactive));
//     RValue *spread = Self->InternalGetYYVarRef(VAR_HASH(spread));
//     RValue *spreadMinimum = Self->InternalGetYYVarRef(VAR_HASH(spreadminimum));
//     RValue *bulletDamageModifier = Self->InternalGetYYVarRef(VAR_HASH(bulletdamagemodifier));
//     RValue *stacks = Self->InternalGetYYVarRef(VAR_HASH(stacks));
//     RValue *bulletDamageModifier2 = Self->InternalGetYYVarRef(VAR_HASH(bulletdamagemodifier2));
//     RValue *prevStat = Self->InternalGetYYVarRef(VAR_HASH(prevstat));
//     RValue *runHeadshotKills = objStatistics->InternalGetYYVarRef(VAR_HASH(runheadshotkills));
//     RValue *modHeadshot = objStats->InternalGetYYVarRef(VAR_HASH(mod_headshotmultiplier));
//     if (giveEffect && giveEffect->ToBoolean())
//     {
//         if (objStats)
//             *modHeadshot = modHeadshot->ToDouble() + 0.8f;
//         *giveEffect = RValue(false);
//     }
//     else if (removeEffect && removeEffect->ToBoolean())
//     {
//         if (objStats)
//             *modHeadshot = modHeadshot->ToDouble() - 0.8f;
//         *removeEffect = RValue(false);
//     }
//     if (effectHoldActive && effectHoldActive->ToBoolean())
//     {
//         if (runHeadshotKills->ToDouble() > prevStat->ToDouble())
//         {
//             *stacks = stacks->ToDouble() + 1.0;
//             *bulletDamageModifier2 = bulletDamageModifier2->ToDouble() + 0.0125f;
//             *spread = *spreadMinimum;
//             if ((static_cast<int>(stacks->ToDouble()) % 10) == 0)
//             {
//                 RValue scrResult;
//                 RValue strStacks = DoBuiltin(&gml_string, {*stacks});
//                 RValue popupColor = RValue(Rarities::RarityColor[Rarities::LEGENDARY]);
//                 RValue two = RValue(2.0);
//                 RValue twoAgain = RValue(2.0);
//                 std::vector<RValue *> args = {
//                     &strStacks,
//                     &popupColor,
//                     &two,
//                     &twoAgain};
//                 gml_Script_scr_popup(Self, Self, &scrResult, static_cast<int>(args.size()), args.data());
//             }
//             *prevStat = runHeadshotKills->ToDouble();
//         }
//     }
// }

// HOOK_GLOBAL(gml_Object_obj_enm_workerbot_MOBA_Alarm_11, (CInstance * Self, CInstance *Other)->void)
// {
//     Organik::GetLogger()->LogFormatted("gml_Object_obj_enm_workerbot_MOBA_Alarm_11 called: %p", Self);
//     return;
// }
// HOOK_GLOBAL(gml_Object_obj_enm_turret_MOBA_Base_Alarm_11, (CInstance * Self, CInstance *Other)->void)
// {
//     Organik::GetLogger()->LogFormatted("gml_Object_obj_enm_turret_MOBA_Base_Alarm_11 called: %p", Self);
//     return;
// }
// HOOK_GLOBAL(gml_Object_obj_weapon_PARENT_Step_0, (CInstance * Self, CInstance *Other)->void)
// {
//     if (!Self)
//     {
//         super(Self, Other);
//         return;
//     }

//     if (!Self->InternalGetYYVarRef(VAR_HASH(isLocal))->ToBoolean())
//     {
//         super(Self, Other);
//         return;
//     }

//     RValue *organikPerfReload = Self->InternalReadYYVar(VAR_HASH(Organik_ArtifactPerfReload));
//     RValue *organikOnkillFx = Self->InternalReadYYVar(VAR_HASH(Organik_Onkill));
//     if (organikPerfReload)
//     {
//         RValue *perfectActiveReloadBuffShots = Self->InternalGetYYVarRef(VAR_HASH(perfectactivereloadbuffshots));
//         if (perfectActiveReloadBuffShots->ToDouble() > 0.0)
//         {
//             double perfectReloadScaling = organikPerfReload->ToDouble();
//             *perfectActiveReloadBuffShots = 0.0;
//             *Self->InternalGetYYVarRef(VAR_HASH(Organik_ArtifactNextShot)) = RValue(true);
//         }
//     }
//     CInstance *objStatistics = CInstance::FirstOrDefault([&](CInstance *instance)
//                                                          { return instance && instance->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_statistics]; });
//     if (organikOnkillFx)
//     {
//         RValue *organikOnkillFxTrigger = DoBuiltin(&gml_ds_map_copy, {}).ToObject()->InternalGetYYVarRef(VAR_HASH(Organik_HeadshotStackingTemp));
//         RValue *runheadshotkills = objStatistics->InternalGetYYVarRef(VAR_HASH(runheadshotkills));
//         RValue *prevkills = Self->InternalGetYYVarRef(VAR_HASH(prevkills));
//         RValue *damageBuffTemp = Self->InternalGetYYVarRef(VAR_HASH(Organik_DamageBuffTemp));
//         // if (!damageBuffTemp->ToPointer())
//         // {

//         // }
//         // if (headshotStackingTemp->ToDouble() > 0.0)
//         // {
//         //     if (runheadshotkills->ToDouble() > prevkills->ToDouble())
//         //     {

//         //     }
//         // }
//     }
//     super(Self, Other);
// }
// HOOK_GLOBAL(gml_Object_obj_boss_parent_Destroy_0, (CInstance * Self, CInstance *Other)->void)
// {
//     super(Self, Other);
//     CInstance::ForEach([&](CInstance *instance)
//                        {
//         if (!instance) return;
//         RValue* isLocal = instance->InternalGetYYVarRef(VAR_HASH(isLocal));
//         if (isLocal && isLocal->ToBoolean())
//         {
//             RValue* timedtime = instance->InternalReadYYVar(VAR_HASH(timedtime));
//             if (!timedtime)
//             {
//                 return;
//             }
//             if (timedtime->ToDouble() > 0.0)
//             {
//                 RValue *weaponvariantname = instance->InternalGetYYVarRef(VAR_HASH(weaponvariantname));
//                 RValue *weaponvariantnameshort = instance->InternalGetYYVarRef(VAR_HASH(weaponvariantnameshort));
                
//                 *instance->InternalGetYYVarRef(VAR_HASH(rollvariant)) = RValue(static_cast<double>(Variant::DIVINE));
//                 *instance->InternalGetYYVarRef(VAR_HASH(variantcolor)) = RValue(static_cast<double>(Rarities::RarityColor[Rarities::LEGENDARY]));
//                 *instance->InternalGetYYVarRef(VAR_HASH(tooltip_rarity)) = RValue(static_cast<double>(Rarities::LEGENDARY));
//                 *timedtime = RValue(-1.0);

//                 *instance->InternalGetYYVarRef(VAR_HASH(timed)) = RValue(false); 
//                 *instance->InternalGetYYVarRef(VAR_HASH(timedtimer)) = RValue(false);
//                 *instance->InternalGetYYVarRef(VAR_HASH(timedbuffspawned)) = RValue(false);
                
//                 RValue wepX = RValue(static_cast<double>(instance->m_X));
//                 RValue wepY = RValue(static_cast<double>(instance->m_Y));
//                 Variant timelessV = (*getUserVariants())[Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>("T I M E L E S S"))];
//                 RValue divineDropLightning = RValue(timelessV.spawnID);
//                 RValue soundFeedback = RValue(timelessV.soundID);
//                 RValue arg1;
//                 RValue arg2;
//                 RValue unused;
//                 std::vector<RValue*> args;
//                 doLocalizationScriptAlt(arg1, arg2, args, weaponvariantname, "@1", timelessV.name.c_str());
//                 doLocalizationScriptAlt(arg1, arg2, args, weaponvariantnameshort, "@1", timelessV.nameshort.c_str());
//                 /* instance->m_DeleteOwnProperty(instance, &unused, Code_Variable_Find_Name(nullptr, 0, VAR_HASH(timedtime)), false); */

//                 DoBuiltin(&gml_audio_play_sound, { soundFeedback, RValue(15.0), RValue(false) });

//                 args = { &wepX, &wepY, &divineDropLightning };
//                 gml_Script_scr_instance_create(instance, instance, &unused, static_cast<int>(args.size()), args.data());
//             }
//         } });
// }
// HOOK_GLOBAL(gml_Object_obj_MOBA_ctrl_PreCreate_0, (CInstance * Self, CInstance *Other)->void)
// {
//     // *GetLogAllCalls() = true;
//     super(Self, Other);
// }
// HOOK_GLOBAL(gml_Object_obj_enm_turret_MOBA_Base_Step_0, (CInstance * Self, CInstance *Other)->void)
// {
//     RValue result;
//     gml_event_inherited(&result, Self, Other, 0, nullptr);
//     return;
// }
// HOOK_GLOBAL(gml_Object_obj_enm_turret_parent_Other_24, (CInstance * Self, CInstance *Other)->void)
// {
//     RValue *path = Self ? Self->InternalReadYYVar(VAR_HASH(path)) : nullptr;
//     if (Self->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_enm_turret_MOBA_Base])
//     {
//         Organik::GetLogger()->LogFormatted("gml_Object_obj_enm_turret_parent_Other_24 detected turret_MOBA_Base");
//         return;
//     }
//     if (path)
//     {
//         Organik::GetLogger()->LogFormatted("gml_Object_obj_enm_turret_parent_Other_24 deleting path %p", path);
//         if (path->ToDouble() != 0.0)
//         {
//             if (DoBuiltin(&gml_path_exists, {*path}).ToBoolean())
//             {
//                 Organik::GetLogger()->LogFormatted("gml_Object_obj_enm_turret_parent_Other_24 deleting path %p", path);
//                 DoBuiltin(&gml_path_delete, {*path});
//             }
//         }
//     }
//     return;
// }
int callstackLen = 0;
std::unordered_map<int32_t, bool> eventsHandled = {};
HOOK_GLOBAL(Perform_Event_Object, (CInstance * Self, CInstance *Other, int objIndex, int eventCode, int subEventCode)->void)
{

    if (!UIManager::isImGuiInitialized())
    {
        super(Self, Other, objIndex, eventCode, subEventCode);
        return;
    }
    super(Self, Other, objIndex, eventCode, subEventCode);

    // RValue *rvTriggersVec = Self->InternalReadYYVar(VAR_HASH(event_triggers));

    // if (!rvTriggersVec || !(rvTriggersVec->GetKind() == VALUE_ARRAY))
    // {
    //     super(Self, Other, objIndex, eventCode, subEventCode);
    //     return;
    // }

    // for (RValue* trigger : triggerList) {
    //     Organik::GetLogger()->LogFormatted("Checking event trigger: %p", trigger);

    //     auto p_Trigger = trigger->ToEventTrigger();

    //     RValue out;
    //     Action* p_Action = p_Trigger->GetActions();
    //     while (p_Action) {
    //         if (p_Action->pfnAction) {
    //             (*p_Action)(Self, Other, 0, &out);
    //         }
    //         p_Action = (p_Action->m_Next != p_Action) ? p_Action->m_Next : nullptr;
    //     }
    // }
}

HOOK_GLOBAL(Script_Perform, (int index, CInstance *self, CInstance *other, int argc, RValue *Result, DynamicArrayOfRValue *Arguments)->void)
{
    super(index, self, other, argc, Result, Arguments);

    // Check for actions with matching triggers
    // auto actions = Organik::ActionRegistry::GetInstance()->GetScriptActions(self->m_ObjectIndex, index);
    // if (!actions || actions->empty()) return;

    // // Process matching actions
    // for (Action* action : *actions) {
    //     // Skip if null
    //     if (!action || !action->trigger) continue;

    //     // Get the trigger as a script trigger
    //     GameScriptTrigger* trigger = dynamic_cast<GameScriptTrigger*>(action->trigger);
    //     if (!trigger) continue;

    //     // Check if trigger should fire
    //     bool shouldFire = true;

    //     // Handle chance-based triggers
    //     if (trigger->flags & Trigger::CHANCE) {
    //         double chance = *trigger->GetAmount();
    //         double roll = (std::rand() % 10000) / 100.0; // 0-100 with 2 decimal precision
    //         shouldFire = (roll <= chance);
    //     }

    //     // Handle interval-based triggers
    //     if (shouldFire && (trigger->flags & Trigger::EVERY_X)) {
    //         double interval = *trigger->GetInterval();
    //         double count = trigger->GetTriggerCount();
    //         shouldFire = (std::fmod(count, interval) < 0.001); // Close enough to zero
    //     }

    //     // Fire the action if conditions are met
    //     if (shouldFire && action->pfnAction) {
    //         RValue result;
    //         std::unordered_map<int32_t, ObjectVariableRef> params;
    //         (action->*(action->pfnAction))(self, other, params, &result);

    //         // Increment trigger count
    //         trigger->triggerCount += 1.0;

    //         // If there's a chain of actions, execute them
    //         Action* nextAction = action->pNext;
    //         while (nextAction) {
    //             if (nextAction->pfnAction) {
    //                 RValue chainResult;
    //                 (nextAction->*(nextAction->pfnAction))(self, other, params, &chainResult);
    //             }
    //             nextAction = nextAction->pNext;
    //         }
    //     }
    // }
}
// HOOK_METHOD(CObjectGM, GetEventRecursive, (int32_t index, int32_t number) -> CEvent*)
// {
//     auto self = this;
//     auto myID = this->m_ID;
//     auto mask = this->m_Mask;
//     auto depth = this->m_Depth;
//     CEvent* event = super(index, number);
//     if (event)
//     {
//         if (!UIManager::isImGuiInitialized())
//             return event;
//         if ((myID >> 2) == 0xE8)
//         {
//             Organik::GetLogger()->LogFormatted(
//                 "CObjectGM::GetEventRecursive: found event %s (%d) for object %d (mask 0x%X, depth %d) at %p", 
//                 this->m_Name, number, myID, mask, depth, event, event->m_Code ? event->m_Code->m_Functions->m_CodeFunction : nullptr
//             );
//         }
//     }
//     return event;
// }
auto Direktor::GMHook::Hooks = std::unordered_map<PVOID, Direktor::GMHook*>();
auto Direktor::GMHook::HooksByType = std::unordered_map<int32_t, std::vector<Direktor::GMHook*>>();
std::shared_mutex Direktor::GMHook::s_HookMutex;
std::string eventSpecficName(int32_t eventCode, int32_t subCode, EventDetailType detailType = EV_WEAPON)
{
    char subCodeBuf[] = { static_cast<char>(subCode & 0xFF), '\0' };
    switch(eventCode)
    {
        case 2: return "Alarm " + std::to_string(subCode);
        case 3:
        {
            switch(subCode)
            {
                case 0: return "Step - Begin";
                case 1: return "Step - Normal";
                case 2: return "Step - End";
                default: return "Step " + std::to_string(subCode);
            }
        }
        case 5: return "Key " + std::string(subCodeBuf) + " Press";
        case 6:
        {
            switch(subCode)
            {
                case 0: return "Mouse Left Button";
                case 1: return "Mouse Right Button";
                case 2: return "Mouse Middle Button";
                case 3: return "Mouse Wheel Up";
                case 4: return "Mouse Wheel Down";
                default: return "Mouse Button " + std::to_string(subCode);
            }
        }
        case 7:
        {
            switch(subCode)
            {
                case 4: return "Other - Room Start";
                case 5: return "Other - Room End";
                case 10: return "Other - User 1";
                case 11: return "Other - User 1";
                case 12: return "Other - User 2";
                case 13: return "Other - User 3";
                case 14: return "Other - Apply Variant (User 4)";
                case 15: return "Other - User 5";
                case 16: return "Other - User 6";
                case 17: return "Other - User 7";
                case 18: return "Other - User 8";
                case 19: return "Other - User 9";
                case 20: return "Other - User 10";
                case 21: return "Other - User 11";
                case 22: return "Other - User 12 (Network)";
                case 23: return "Other - User 13";
                case 24: return "Other - User 14";
                case 25: return "Other - User 15";
                default: return "Other " + std::to_string(subCode);
            }
        }
        case 9: return "Key " + std::string(subCodeBuf) + " Press";
        case 10: return "Key " + std::string(subCodeBuf) + " Release";
        default: return eventTypeName(eventCode) + " (" + std::to_string(subCode) + ")";
    }
}

// HOOK_GLOBAL_PRIORITY(ExecuteIt, -9999, (CInstance* Self, CInstance* Other, CCode* code, RValue* res, int flags) -> bool)
// {
//     std::shared_lock<std::shared_mutex> lock(Direktor::GMHook::AcquireSharedLock());
//     if (Direktor::GMHook::HasHooksToInstall<Direktor::EventHook>())
//     {
        
//     }
// }

std::stack<Action *> actionStack;
HOOK_GLOBAL(ExecuteIt, (CInstance * Self, CInstance *Other, CCode *Code, RValue *Result, int flags)->bool)
{
    if (!UIManager::isImGuiInitialized())
    {
        return super(Self, Other, Code, Result, flags);
    }
    if (!Code)
    {
        Organik::GetLogger()->LogFormatted("ExecuteIt: CCode is null!!!");
        return super(Self, Other, Code, Result, flags);
    }
    bool result = true;
    if (Code->m_Token.m_AfterLength + Code->m_Token.m_BeforeLength >= 1)
    {
        Organik::GetLogger()->LogFormatted("ExecuteIt: %s -- %d, %d", (Code->m_Name ? Code->m_Name : (Code->m_Functions ? Code->m_Functions->m_Name : "NameErr")), Code->m_Token.m_BeforeLength, Code->m_Token.m_AfterLength);
        // Organik::GetLogger()->LogFormatted("ExecuteIt: %s -- 0x%d", (Code->m_Token.m_Value.GetKindName()), Code->m_Token.m_Value.GetArrayLength());
        // Organik::GetLogger()->LogFormatted("ExecuteIt: %s -- 0x%d", (Code->m_Token.m_Value[0].GetKindName()), Code->m_Token.m_Value[0].GetArrayLength());
        // Organik::GetLogger()->LogFormatted("ExecuteIt: %s -- 0x%d", (Code->m_Token.m_Value[1].GetKindName()), Code->m_Token.m_Value[1].GetArrayLength());
        RValue out;
        for (int i=0;i<Code->m_Token.m_BeforeLength;i++)
        {
            auto val = Code->m_Token.m_BeforeArray[i];
            Organik::GetLogger()->LogFormatted("ExecuteIt: -- %p, %p", val.first, val.second);
            if (val.first && val.second)
            {
                if (out.GetKind() != VALUE_UNDEFINED)
                    *(val.second->InternalGetYYVarRef(VAR_HASH(input))) = out;
                if (!val.first(Self, Other, val.second))
                    break;
                out = *val.second->InternalGetYYVarRef(VAR_HASH(result));
            }
            else
            {
                Organik::GetLogger()->LogFormatted("ExecuteIt: Before token %d has null function or context! (%p, %p)", 
                    i, val.first, val.second);
            }
        }
        
        Code->m_Functions->m_CodeFunction(Self, Other);
        out = RValue();
        for (int i=0;i<Code->m_Token.m_AfterLength;i++)
        {
            auto val = Code->m_Token.m_AfterArray[i];
            if (val.first && val.second)
            {
                if (out.GetKind() != VALUE_UNDEFINED)
                    *(val.second->InternalGetYYVarRef(VAR_HASH(input))) = out;
                if (!val.first(Self, Other, val.second))
                    return true;
                out = *val.second->InternalGetYYVarRef(VAR_HASH(result));
            }
            else
            {
                Organik::GetLogger()->LogFormatted("ExecuteIt: Before token %d has null function or context! (%p, %p)", 
                    i, val.first, val.second);
            }
        }
        return true;
    }
    result = super(Self, Other, Code, Result, flags);
    return result;
}
//     // if (!Organik::UIManager::isImGuiInitialized())
//     //     return result;

//     if (!Code)
//         return super(Self, Other, Code, Result, flags);

//     static auto eventHandlersMap = std::unordered_map<void*, void*>();
//     if (eventHandlersMap.empty())
//     {
//         // read in Event Handlers
//         std::ifstream fileIn("event_handlers.log");
//         std::string line;
//         while (std::getline(fileIn, line))
//         {
//             std::istringstream iss(line);
//             std::string name;
//             std::string addr;
//             std::string codeAddr;
//             uintptr_t address;
//             uintptr_t codeAddress;
//             if (std::getline(iss, name, ',') && std::getline(iss, addr, ',') && std::getline(iss, codeAddr, ','))
//             {
//                 address = std::stoul(addr, nullptr, 16);
//                 codeAddress = std::stoul(codeAddr, nullptr, 16);
//                 // Store the event handler in the map
//                 eventHandlersMap.insert_or_assign(reinterpret_cast<void*>(address), reinterpret_cast<void*>(codeAddress));
//             }
//         }
//     }
//     static std::ofstream fileOut("event_handlers.log", std::ios::app);
//     if (!eventHandlersMap.contains(reinterpret_cast<PVOID>(Code->m_Functions->m_CodeFunction)))
//     {
//         eventHandlersMap.insert_or_assign(reinterpret_cast<PVOID>(Code->m_Functions->m_CodeFunction), reinterpret_cast<PVOID>(Code));
//         fileOut << (Code->m_Name ? Code->m_Name : (Code->m_Functions ? Code->m_Functions->m_Name : "NameErr")) << "," << std::hex << Code->m_Functions->m_CodeFunction << "," << reinterpret_cast<PVOID>(Code) << std::dec << std::endl;
//     }
//     if (*GetLogAllCalls() || false)
//     {
//         Organik::GetLogger()->LogFormatted("ExecuteIt: %s -- 0x%p", (Code->m_Name ? Code->m_Name : (Code->m_Functions ? Code->m_Functions->m_Name : "NameErr")), Code->m_Functions->m_CodeFunction);
//     }
//     bool result = super(Self, Other, Code, Result, flags);
//     return result;
// }
