#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "VariableHelper.h"
#include "Globals.h"
#include "Action.h"
#include "Organik/UI/UIManager.h"
#include "Variant.h"

RValue playerID;
RValue playerName;
// HOOK_GLOBAL(gml_Object_obj_net_MS_client_Create_0, (CInstance* Self, CInstance* Other) -> void)
// {
//     CInstance* outgameObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool {
//         return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::outgame];
//     });

//     if (outgameObj)
//         (*outgameObj->InternalReadYYVar(VAR_HASH(LoggedOut))) = RValue(false);

//     super(Self, Other);

//     if (!Self)
//         return; 
    

//     YYCreateString(&playerID, "696969");

//     (*outgameObj->InternalReadYYVar(VAR_HASH(playerId))) = playerID;

// }
HOOK_GLOBAL(gml_Script_scr_weapon_variants_init_ultimate, (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*)
{

    if (!UIManager::isImGuiInitialized())
        return super(Self, Other, Result, ArgumentCount, Arguments);
    if (!Self)
        return super(Self, Other, Result, ArgumentCount, Arguments);
    if (!getUserVariants()->size())
        LoadVariants();
    if (!getUserVariants()->size())
        Error_Show_Action("No variants loaded. Please check your Mods/Variants folder. \n\nIf Mods/Variants/BASEGAME/variants.json is not present, reinstall the mod.\nIf issue persists please report this bug on GitHub or Discord.", true, true);
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);
    RValue* isLocal = Self->InternalReadYYVar(VAR_HASH(isLocal));
    RValue* variantpredestined = Self->InternalReadYYVar(VAR_HASH(variantpredestined));
    RValue* variantInit = Self->InternalReadYYVar(VAR_HASH(variantInit));
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);

    bool isLocalBool = false;
    if (isLocal != nullptr)
        isLocalBool = isLocal->ToBoolean();
    int32_t variantpredestinedInt = (variantpredestined != nullptr) ? variantpredestined->ToInt32() : -1;
    bool variantInitBool = false;
    if (variantInit != nullptr) 
        variantInitBool = variantInit->ToBoolean();

    if (!DoBuiltin(&gml_instance_exists, { 
        RValue(Organik::Objects::ObjIndexes[Organik::Objects::obj_control])
    }).ToBoolean()) {
        return super(Self, Other, Result, ArgumentCount, Arguments);
    }
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);
    if ((!isLocalBool && (variantpredestinedInt == -1)) || variantInitBool)
    {
        *Result = RValue(0);
        return Result;
    }
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);

    CInstance* outgameObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool {
        return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::outgame];
    });

    if (outgameObj->InternalGetYYVarRef(VAR_HASH(novariants))->ToBoolean())
    {
        return super(Self, Other, Result, ArgumentCount, Arguments);
    }

    CInstance* localPlayerObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool {
        return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_localPlayer];
    });
    CInstance* statsObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool {
        return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_stats];
    });
    CInstance* bossroomObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool {
        return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_bossroom];
    });
	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);

    auto luckRV = statsObj->InternalGetYYVarRef(VAR_HASH(mod_luck));

    if (luckRV->GetKind() != VALUE_REAL)
        *luckRV = RValue(1.0);

    double luckMod = statsObj ? parseRValueNumber<double>(luckRV) : 1.0;

    if(!outgameObj)
    {
        Error_Show_Action("Outgame object not found", true, true);
    }

	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);

    *Self->InternalGetYYVarRef(VAR_HASH(variantInit)) = RValue(true);
    auto rng = Utils::getrng();
    std::vector<int32_t> variantChoices = {};
    for (const auto [variantHash, variant] : *getUserVariants())
    {
        variantChoices.push_back(variantHash);
        GetLogger()->LogFormatted("Variant possible rolls: adding %d - %s", variantHash, variant.name.c_str());
    }
    int32_t rollRng = (*rng)() % variantChoices.size();
    GetLogger()->LogFormatted("Variant roll rng: %d / %d", rollRng, variantChoices.size());
    int32_t rollVariant = variantChoices[rollRng];
    // Variant kit 1 = 5 and 16
    // Variant kit 2 = 9 and 12
    // Cursed = 15

    int32_t selectedClass = parseRValueNumber<int32_t>(outgameObj->InternalGetYYVarRef(VAR_HASH(selectedclass)));
    int32_t classLevel = parseRValueNumber<int32_t>(outgameObj->InternalGetYYVarRef(VAR_HASH(currentclasslvl))->ToRefVector()[3]);
    int32_t selectedSubclass = parseRValueNumber<int32_t>(outgameObj->InternalGetYYVarRef(VAR_HASH(selectedsubclass)));
    int32_t subclassLevel = parseRValueNumber<int32_t>(outgameObj->InternalGetYYVarRef(VAR_HASH(subclasslvl))->ToRefVector()[3]->ToRefVector()[selectedSubclass]);
    RValue *toolTipRarity  = Self->InternalGetYYVarRef(VAR_HASH(tooltip_rarity));
    if (toolTipRarity->GetKind() != VALUE_REAL)
        *toolTipRarity = RValue(1.0);
    int32_t tooltipRarity = parseRValueNumber<int32_t>(Self->InternalGetYYVarRef(VAR_HASH(tooltip_rarity)));

	GetLogger()->LogFormatted("%s -- %d", __FUNCTION__, __LINE__);
    
    CInstance* pistolExtender = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
        bool ret = ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_perk_pistolextender];
        if (!ret) return ret;
        if (parseRValueNumber<int32_t>(ci->InternalGetYYVarRef(VAR_HASH(owner))) == parseRValueNumber<int32_t>(Self->InternalGetYYVarRef(VAR_HASH(owner))))
        {
            return true;
        }
        return false;
    });
    
    Variant v = (*getUserVariants()).contains(rollVariant) ? (*getUserVariants())[rollVariant] : Variant();
    //     GetLogger()->LogFormatted("Pistol Extender found: %d (%p)", pistolExtender ? pistolExtender->m_ID : -1, pistolExtender ? pistolExtender : 0x0);
    CInstance* ultradrop = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
        bool ret = ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_artefact_ultradrop];
        if (!ret) return ret;
        if (parseRValueNumber<int32_t>(ci->InternalGetYYVarRef(VAR_HASH(owner))) == parseRValueNumber<int32_t>(Self->InternalGetYYVarRef(VAR_HASH(owner))))
        {
            return true;
        }
        return false;
    });
    if (ultradrop && !(ultradrop->InternalGetYYVarRef(VAR_HASH(usedultra))->ToBoolean()))
    {
        while (v.rarity < 5.0)
        {
            rollVariant = variantChoices[(*rng)() % variantChoices.size()];
            v = (*getUserVariants()).contains(rollVariant) ? (*getUserVariants())[rollVariant] : Variant();
        }
        *ultradrop->InternalGetYYVarRef(VAR_HASH(usedultra)) = RValue(true);
        DoBuiltin(&gml_audio_play_sound, {
            DoBuiltin(&gml_asset_get_index, {RValue("sound_artefact_use")})
        });
        YYCreateString(ultradrop->InternalGetYYVarRef(VAR_HASH(buffnote)), "USED");
    }
    else 
    {    
        if ((*getUserVariants()).contains(rollVariant))
        {
            if (v.rarity >= 5.0)
            {
                double rerollChance = (1.0 * (5 * (tooltipRarity * 5 + 1))) * 100.0; // ~ 80% for 5, 90% for 6, 95% for 7
                if (selectedClass == 3 && classLevel >= 10)
                {
                    rerollChance -= 5.0;
                }

                if (tooltipRarity <= 3)
                {
                    rerollChance -= 8.0;
                }
                rerollChance = std::clamp(rerollChance, 0.0, 99.999);
                double reroll = (*rng)() % 1000000 * 0.0001;
                if ((reroll * luckMod) < rerollChance)
                {
                    rollVariant = variantChoices[(*rng)() % variantChoices.size()];
                    v = (*getUserVariants()).contains(rollVariant) ? (*getUserVariants())[rollVariant] : Variant();
                }
            }
        }

        if (bossroomObj && (*getUserVariants()).contains(Variant::ALPHA))
            rollVariant = Variant::ALPHA;

        *Self->InternalGetYYVarRef(VAR_HASH(startvariant)) = RValue(rollVariant);
        if (pistolExtender)
        {
            *Self->InternalGetYYVarRef(VAR_HASH(startvariant)) = RValue(1.0);
            if (rollVariant == Variant::STANDARD && (*getUserVariants()).contains(Variant::MILSPEC))
                rollVariant = Variant::MILSPEC;
        }

        if (variantpredestinedInt != -1)
            rollVariant = variantpredestinedInt;
    }
    gml_Script_scr_raritycolors_init(Self, Other, Result, 0, nullptr);
    *Self->InternalGetYYVarRef(VAR_HASH(rollvariant)) = RValue(static_cast<double>(rollVariant));
    Variant::Apply(Self, rollVariant, selectedClass, selectedSubclass, subclassLevel);
    *Result = true;
    return Result;
}
HOOK_GLOBAL(BOOL_RValue, (const RValue* val) -> bool)
{
    return val->ToBoolean();
}
HOOK_GLOBAL(gml_Script_object_get_depth, (CInstance* self, CInstance* other, RValue* result, int argumentCount, RValue** arguments) -> RValue*)
{
    if (!Organik::UIManager::GetInstance()->isImGuiInitialized() && !ImGui::GetCurrentContext())
        DoHelperSetup();
    if (argumentCount > 0 && arguments[0])
    {
        int32_t objIndex = parseRValueNumber<int32_t>(arguments[0]);
        if ((objIndex >> 2) << 2 == 164 ||
            !(objIndex & (~1660)))
        {
            if (!Organik::Utils::isInitializationDone())
                return super(self, other, result, argumentCount, arguments);
            *arguments[0] = OBJ_HASH(obj_menu_particle);
            
            result = super(self, other, result, argumentCount, arguments);
            return result;
        }
    }
    return super(self, other, result, argumentCount, arguments);
}
HOOK_GLOBAL(gml_Script_scr_create_bullet, (CInstance* self, CInstance* other, RValue* result, int argumentCount, RValue** arguments) -> RValue*)
{
    RValue *damagemod = arguments[7];
    if (*getReplaceBulletIndex() != 0.0)
    {
        CInstance *objControl = CInstance::FirstOrDefault([&](CInstance* ci) -> bool { return ci && ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_control]; });
        bool isNetworkedMulti = false;
        if (objControl)
        {
            auto *netmultiRV = objControl->InternalReadYYVar(VAR_HASH(isNetworkedMultiplayer));
            if (netmultiRV != nullptr)
                isNetworkedMulti = netmultiRV->ToBoolean();
        }

        if (isNetworkedMulti)
        {
            return super(self, other, result, argumentCount, arguments);
        }

        *(arguments[0]) = RValue(*getReplaceBulletIndex());
        return super(self, other, result, argumentCount, arguments);
    }
    else if (arguments[15])
    {
        CInstance* weaponObj = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
            return ci && ci->m_ID == static_cast<int32_t>(arguments[15]->ToDouble());
        });
        CInstance* outgameObj = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
            return ci && ci->m_ObjectIndex  == static_cast<int32_t>(Organik::Objects::ObjIndexes[Organik::Objects::outgame]);
        });
        CInstance* objControl = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
            return ci && ci->m_ObjectIndex  == static_cast<int32_t>(Organik::Objects::ObjIndexes[Organik::Objects::obj_control]);
        });
        RValue *curseDamageScaleVar = weaponObj->InternalReadYYVar(VAR_HASH(Organik_CurseDmgScale));
        RValue *bulletObjVar = weaponObj->InternalReadYYVar(VAR_HASH(Organik_BulletObj));
        if (bulletObjVar && parseRValueNumber<int32_t>(*bulletObjVar) > 0)
        {
            *(arguments[0]) = parseRValueNumber<double>(bulletObjVar);
        }
        if (curseDamageScaleVar && parseRValueNumber<double>(curseDamageScaleVar) > 0.0 && outgameObj && objControl && parseRValueNumber<double>(objControl->InternalGetYYVarRef(VAR_HASH(terrorlevel))) > 0.0)
        {
            RValue *terrorlevel = objControl->InternalGetYYVarRef(VAR_HASH(terrorlevel));
            *damagemod = RValue(parseRValueNumber<double>(damagemod) + (parseRValueNumber<double>(terrorlevel) * parseRValueNumber<double>(curseDamageScaleVar)));
        }
    }
    return super(self, other, result, argumentCount, arguments);
}
HOOK_GLOBAL(gml_Script_scr_instance_create, (CInstance * Self, CInstance * Other, RValue * Result, int ArgumentCount, RValue * * Arguments) -> RValue*)
{
    //  Organik::GetLogger()->LogFormatted(
    //     "gml_Script_scr_instance_create called to create: %d (%p), ArgumentCount: %d",
    if (!Organik::Utils::isInitializationDone())
        return super(Self, Other, Result, ArgumentCount, Arguments);
    
    if (ArgumentCount > 2 && Arguments[2] != nullptr)
    {
        int32_t objIndex = parseRValueNumber<int32_t>(Arguments[2]);
        if (objIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_mysteryarena]
            || objIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_chest_weapon_wood_arena])
        {
                RValue dummy;
                auto rng = Utils::getrng();
                int chestType = (*rng)() % getNonArenaChests().size();
                RValue x = Self->m_X;
                RValue y = Self->m_Y;
                RValue newChestType = RValue(static_cast<double>(getNonArenaChests()[chestType]));
                RValue selfID = Self->m_ID;
                std::vector<RValue*> args = {
                    &x,
                    &y,
                    &newChestType
                };
                Organik::GetLogger()->LogFormatted("Replacing arena chest with non-arena chest of type %d", newChestType.ToInt32());
                gml_Script_scr_instance_create(GetGlobalInstance(), GetGlobalInstance(), Result, static_cast<int>(args.size()), args.data());
                return Result;
        }
    }
    Result = super(Self, Other, Result, ArgumentCount, Arguments);
    CInstance* newSelf = CInstance::FirstOrDefault([Result](CInstance* ci) -> bool { return ci->m_ID == Result->ToInt32(); });
    if (!newSelf)
    {
         Organik::GetLogger()->LogFormatted("gml_Script_scr_instance_create: Failed to find instance with ID %d", Result->ToInt32());
        return Result;
    }

    if (!Organik::Utils::isInitializationDone())
        return Result;

    // int32_t objectIndex = newSelf->m_ObjectIndex;
    // while (objectIndex > 0)
    // {
    //     Organik::GetLogger()->LogFormatted("Checking object %d for triggers", objectIndex);
    //     if (EventMap::ObjectHasTriggers(objectIndex))
    //     {
    //         Organik::GetLogger()->LogFormatted("Object %d has triggers", objectIndex);
            
    //         auto TriggersMap = EventMap::GetGlobal()->ApplyTriggers(Self);
            
    //         for (const auto& triggersArr : EventMap::)
    //         {
    //             for (auto &trigger : triggersVec)
    //             {
    //                 int32_t fullHash = (VAR_HASH(event_triggers)) ^ GetEventHash(trigger->GetEventLookup()) ^ (trigger->GetWhenHash());
    //                 if ((trigger->eventType | trigger->eventSubType) == 0)
    //                 {
    //                     RValue in;
    //                     RValue out;
    //                     Action *action = trigger->GetActions();
    //                     bool continueProcessing = true;
    //                     while (continueProcessing && action)
    //                     {
    //                         CInstance* triggerOwner = nullptr;
    //                         CInstance* triggerOther = nullptr;
    //                         if (action->GetParams()->hasValue(VAR_HASH(self)))
    //                         {
    //                             triggerOwner = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
    //                                 return ci && ci->m_ID == parseRValueNumber<int32_t>((*action->GetParams())[VAR_HASH(self)]);
    //                             });
    //                         }
    //                         if (action->GetParams()->hasValue(VAR_HASH(other)))
    //                         {
    //                             triggerOther = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
    //                                 return ci && ci->m_ID == parseRValueNumber<int32_t>((*action->GetParams())[VAR_HASH(other)]);
    //                             });
    //                         }
    //                         continueProcessing = (*action)(triggerOwner, triggerOther, in, &out);
    //                         in = out;
    //                         action = action->m_Next;
    //                     } 
    //                     continue;
    //                 }

    //                 RValue *rvTriggerList = &(*rvTriggersMap)[trigger->eventType][trigger->eventSubType];
    //                 if (!(rvTriggerList->GetKind() == VALUE_ARRAY))
    //                 {
    //                     std::vector<double> dummy_array3 = {0.0};
    //                     YYCreateArray(rvTriggerList, 1, dummy_array3.data());
    //                     (*rvTriggerList)[0] = trigger;
    //                     continue;
    //                 }
    //                 rvTriggerList->push_back(trigger);
    //             }
    //         }
    //     }
    //     else
    //     {
    //         Organik::GetLogger()->LogFormatted("Object %d has NO triggers", objectIndex);
    //     }
    //     objectIndex = parseRValueNumber<int32_t>(DoBuiltin(&gml_object_get_parent, { RValue(objectIndex) }));
    // }
    
    RValue* onePlayer = newSelf->InternalReadYYVar(VAR_HASH(forONEplayeronly));
    if (onePlayer)
        *onePlayer = RValue(false);
    
    return Result;
}
HOOK_GLOBAL(gml_Script_scr_enemy_set_stateChanged, (CInstance* Self, CInstance* Other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*)
{
    if (ArgumentCount > 1)
    {
        RValue* enemyID = Arguments[0];
        if (!Arguments[1])
        {
//             Organik::GetLogger()->LogFormatted("ARG1 Error in gml_Script_scr_enemy_set_stateChanged %p", Arguments[0]);
        }
//         Organik::GetLogger()->LogFormatted("Enemy ID: %d, newState: %d", enemyID->ToDouble(), Arguments[1]->ToDouble());
        CInstance* enemyInstance = CInstance::FirstOrDefault([&](CInstance* ci) -> bool {
            return ci && ci->m_ID == parseRValueNumber<int32_t>(enemyID);
        });
        if (!enemyInstance)
        {
//             Organik::GetLogger()->LogFormatted("Enemy instance not found for ID: %d", enemyID->ToDouble());
            return super(Self, Other, Result, ArgumentCount, Arguments);
        }
//         Organik::GetLogger()->LogFormatted("Setting instance %d of %s to state %d", enemyInstance->m_ID, enemyInstance->m_Object->m_Name, Arguments[1]->ToDouble());
        *enemyInstance->InternalGetYYVarRef(VAR_HASH(stateChanged)) = *(Arguments[1]);
        return Result;
    }
    if (!Self)
    {
//         Organik::GetLogger()->LogFormatted("Self is null in gml_Script_scr_enemy_set_stateChanged");
        return super(Self, Other, Result, ArgumentCount, Arguments);
    }
    if (!Arguments[0])
    {
//         Organik::GetLogger()->LogFormatted("ARG0 Error in gml_Script_scr_enemy_set_stateChanged %p", Arguments[0]);
    }
    
//     Organik::GetLogger()->LogFormatted("Setting instance %d to state %d", Self->m_ID, (Arguments[0])->ToDouble());
    if (Self->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_enm_turret_MOBA_Base])
    {
//         Organik::GetLogger()->LogFormatted("Turret state change detected.");
        return Result;
    }
    *Self->InternalGetYYVarRef(VAR_HASH(stateChanged)) = *(Arguments[0]);
    return Result;
}
// HOOK_GLOBAL(DoCall, (uint ret, uchar *param_2, uchar *ccode_index, VMExec *param_4) -> unsigned char*)
// {
//     unsigned char* result = super(ret, param_2, ccode_index, param_4);

//     // if (!Organik::UIManager::isImGuiInitialized())
//     //     return result;

//     static auto scriptHandlersMap = std::unordered_map<void*, void*>();
//     if (scriptHandlersMap.empty())
//     {
//         // read in Script Handlers
//         std::ifstream fileIn("script_handlers.log");
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
//                 // Store the script handler in the map
//                 scriptHandlersMap.insert_or_assign(reinterpret_cast<void*>(address), reinterpret_cast<void*>(codeAddress));
//             }
//         }
//     }
//     static std::ofstream fileOut("script_handlers.log", std::ios::app);
//     if (!scriptHandlersMap.contains(reinterpret_cast<PVOID>(ccode_index)))
//     {
//         scriptHandlersMap.insert_or_assign(reinterpret_cast<PVOID>(ccode_index), reinterpret_cast<PVOID>(param_4));
//         fileOut << ("NameErr") << "," << std::hex << ccode_index << "," << reinterpret_cast<PVOID>(param_4) << std::dec << std::endl;
//     }
//     if (*GetLogAllCalls() || true)
//     {
// //         Organik::GetLogger()->LogFormatted("DoCallScript: %s -- 0x%p", ("NameErr"), ccode_index);
//     }
//     return result;
// }