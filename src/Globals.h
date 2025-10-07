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
void ChatAddHooks();
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
        const inline LIBZHL_API int32_t RarityColor[] = {
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
std::unordered_map<int32_t, CInstance*>* GetGlobalInstances();

const std::vector<int32>& getNonArenaChests();

#define MAKE_STRING(thing) #thing,
#define MAKE_MAP(thing) {Code_Variable_FindAlloc_Slot_From_Name(nullptr, const_cast<char*>(#thing)), ActionType::##thing},
#define MAKE_ENUM(thing) thing,

#pragma once
template<int32_t ObjectEnum>
requires (ObjectEnum > -1)
CInstance* GetGlobalObject()
{
	auto cObject = Object_Data(Organik::Objects::ObjIndexes[ObjectEnum]);
	return (cObject 
        ? (cObject->m_Instances.m_Count
            ? (cObject->m_Instances.m_First
                ? (cObject->m_Instances.m_First->m_Object 
                    ? (cObject->m_Instances.m_First->m_Object) 
                    : nullptr)
                : nullptr)
            : nullptr)
        : nullptr);
}


void DrawObjectSelector(std::function<void(int32_t)> callback);
double* getReplaceBulletIndex();
bool* GetLogAllCalls();
std::vector<int32_t>* getPossibleBullets();

// std::unordered_map<int32_t, RValue>* GetEventReverseMap();


CInstance* GetActionGlobalSelf();
CInstance* GetActionGlobalOther();