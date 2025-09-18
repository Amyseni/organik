#pragma once
#include "Synthetik.h"
#include <variant>
#include <functional>
#include <concepts>
#define doLocalizationScript(argVar, argVector, outPtr, strText) \
    YYCreateString(&argVar, strText); \
    argVector = { &argVar }; \
    gml_Script_scr_localization(instance, instance, outPtr, static_cast<int>(argVector.size()), argVector.data());

#define doLocalizationScriptAlt(argVar, arg2Var, argVector, outPtr, strText, strReplace) \
    YYCreateString(&argVar, strText); \
    YYCreateString(&arg2Var, strReplace); \
    argVector = { &argVar, &arg2Var }; \
    gml_Script_scr_localization(instance, instance, outPtr, static_cast<int>(argVector.size()), argVector.data());
#define ORGANIK_VARIANT 0x08000000
inline uint32_t CalculateEventHash(uint64_t eventCodeFull)
{
    // look, I don't make the rules okay? this is just how it works
    return static_cast<uint32_t>(
        (
            (eventCodeFull * 0x9E3779B97F4A7C55u) 
            >> (32)
        )) + 1;
}
inline uint32_t CalculateEventHash(uint32_t eventCode, uint32_t eventSubCode)
{
    return CalculateEventHash(
          (static_cast<uint64_t>(eventCode) << 32) 
        | (static_cast<uint64_t>(eventSubCode)));
}
namespace Organik 
{
    namespace Variants
    {
        
    }
    namespace Rarities
    {
        enum : int32_t
        {
            COMMON = 0,
            UNCOMMON = 2,
            RARE = 3,
            EPIC = 4,
            ULTRA = 5,
            LEGENDARY = 8
        };
        const int32_t RarityColor[] = {
            0xC0C0C0, // #C0C0C0
            0x855F4C, // #855F4C
            0xFF8400, // #FF8400
            0xFD5D58, // #FD5D58
            0xFC5076, // #FC5076
            0x2323E3, // #2323E3
            0x003CFF, // #003CFF
            0x00C0FF, // #00C0FF
            0x917347, // #917347
        };
    }
}
RValue* GetEventCodeReverseMap();
std::unordered_map<int32_t, CInstance*>* GetGlobalInstances();
enum EffectApplyMode 
{
    ADDITIVE,
    MULTIPLICATIVE,
    SET
};
template <typename T>
requires std::is_arithmetic_v<T>
T parseRValueNumber(RValue* rValue)
{
    if (!rValue)
    {
        return T(0.0);
    } 
    T ret = T(0.0);
    switch (rValue->m_Kind)
    {
        case VALUE_INT32:
            ret = static_cast<T>(rValue->ToInt32());
            break;
        case VALUE_INT64:
            ret = static_cast<T>(rValue->ToInt64());
            break;
        case VALUE_PTR:
            ret = static_cast<T>((uintptr_t)rValue->ToPointer());
            break;
        case VALUE_REAL:
            ret = static_cast<T>(rValue->ToDouble());
            break;
    }
    return ret;
}
template <typename T>
requires std::is_arithmetic_v<T>
T parseRValueNumber(RValue rValue)
{
    if (!rValue) 
    {
        return static_cast<T>(0.0);
    }

    T ret = static_cast<T>(0.0);
    switch (rValue.m_Kind)
    {
        case VALUE_INT32:
            ret = static_cast<T>(rValue.ToInt32());
            break;
        case VALUE_INT64:
            ret = static_cast<T>(rValue.ToInt64());
            break;
        case VALUE_PTR:
            Error_Show_Action("WARNING! parseRValueNumber: called with VALUE_PTR, this is probably a bug.", false, true);
            ret = static_cast<T>((uintptr_t)rValue.ToPointer());
            break;
        case VALUE_REAL:
            ret = static_cast<T>(rValue.ToDouble());
            break;
    }
    return ret;
}
const std::vector<int32>& getNonArenaChests();
#define ACTIONS_DO(THING) \
    THING(call_gml) \
    THING(call_event) \
    THING(call_script) \
    THING(call_action) \
    THING(assign) \
    THING(math) \
    THING(cond) \
    THING(buffer_read) \
    THING(buffer_write) \
    THING(cache_read) \
    THING(cache_write) \
    THING(instances_iter) \
    THING(struct_vars_iter) \
    THING(list_iter) \
    THING(do_return) \
    THING(draw) \
    THING(stat_sets) \
    THING(stat_increases) \
    THING(stat_multipliers) \

#define flag_param_list 1<<31
#define MAKE_STRING(thing) #thing,
#define MAKE_MAP(thing) {Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(#thing)), ActionType::##thing},
#define MAKE_ENUM(thing) thing,
#define MAKE_FUNCTION(thing) void __thiscall do##thing(CInstance* self, CInstance* other,std::unordered_map<int32_t, ObjectVariableRef> params, RValue* result );
#define MAKE_FUNCTIONPRED(thing) void __thiscall do##thing##Pred(std::function<bool(CInstance*, CInstance*, std::unordered_map<int32_t, ObjectVariableRef>)> pred, CInstance* self, CInstance* other, std::unordered_map<int32_t, ObjectVariableRef> params);

enum ActionType
{
    ACTIONS_DO(MAKE_ENUM)
};
/// @brief It's a const constant. Don't overthink it.
static const char* const ActionNames[] = {
    ACTIONS_DO(MAKE_STRING)
};

#define g__outgame Organik::Objects::outgame
#define g__obj_stats Organik::Objects::obj_stats
#define g__obj_localPlayer Organik::Objects::obj_localPlayer
#define g__obj_statistics Organik::Objects::obj_statistics
#define g__obj_control Organik::Objects::obj_control
#define g__obj_allgame Organik::Objects::obj_allgame
#define g__obj_ingame_ctrl Organik::Objects::obj_ingame_ctrl
#define g__obj_room_ctrl Organik::Objects::obj_room_ctrl
#define g__obj_enemy_ctrl Organik::Objects::obj_enemy_ctrl

#pragma once
template<int32_t ObjectEnum>
requires (ObjectEnum > -1)
CInstance* GetGlobalObject()
{
	auto cObject = Object_Data(Organik::Objects::ObjIndexes[ObjectEnum]);
	return cObject ? (cObject->m_Instances.m_First) : nullptr;
}
#define __gObj(name) GetGlobalObject<g__##name>()
#define __gOutgame __gObj(outgame)
#define __gStats __gObj(obj_stats)
#define __gLocalPlayer __gObj(obj_localPlayer)
#define __gStatistics __gObj(obj_statistics)
#define __gControl __gObj(obj_control)
#define __gAllgame __gObj(obj_allgame)
#define __gIngameCtrl __gObj(obj_ingame_ctrl)
#define __gRoomCtrl __gObj(obj_room_ctrl)
#define __gEnemyCtrl __gObj(obj_enemy_ctrl)

void DrawObjectSelector(std::function<void(int32_t)> callback);
double* getReplaceBulletIndex();
bool* GetLogAllCalls();
std::vector<int32_t>* getPossibleBullets();

// std::unordered_map<int32_t, RValue>* GetEventReverseMap();


CInstance* GetActionGlobalSelf();
CInstance* GetActionGlobalOther();