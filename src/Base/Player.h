#pragma once
#include <string>
#include "../VariableHelper.h"
#include "../ScriptHelper.h"
#include "../InstanceHelper.h"
#include "Entity.h"
#include "YYToolkit/YYTK_Shared.hpp"

namespace Organik
{
    namespace Entities
    {
        struct EntityStats;
        struct EntityInstance;
        struct EntityPosition;
        struct Perk;
    }
}

namespace Organik
{
    namespace Entities
    {
        enum OrganikClass : uint32_t
        {
            MEKANIK,
            COLLEKTOR,
            KASTER,
            TYPE37,
            PHALANX,
            DEMON,
            HUNTER,
            GH0ST,
            GODTOUCHED,
        };
        struct EntityStats;
        struct EntityInstance;
        struct EntityPosition;

        using Vec2 = YYTK::Vec2;
        using RValue = YYTK::RValue;
        struct PlayerPosition : EntityPosition
        {

        };
        // struct Perk : public LingeringEffect
        // {
        //     EntityStats* m_Bonuses;
        //     OrganikClass perkClass;
        //     typedef [this] auto (*ApplyEffectFn)(EntityInstance* self, EntityStats* applyTo, const RValue* value) -> bool;
        //     [this](bool(defaultApply)(EntityInstance* self, EntityStats* applyTo, const RValue* value)) 
        //     {
        //         if (applyTo == nullptr || self == nullptr) {
        //             return false;
        //         }
        //         // Apply the bonuses to the target stats
        //         for (const auto& [hash, statPair] : applyTo->m_Stats) {
        //             if (statPair.second != nullptr) {
        //                 *statPair.second = *value; // Assuming value is a valid RValue
        //             }
        //         }
        //         return true;
        //     };
        //     std::function<bool> applySelfToTarget;
        //     Perk(
        //         const char* name,
        //         OrganikClass perkClass,
        //         uint32_t Flags = Perk::Multiplicative |
        //                         Perk::Mod_Buff |
        //                         Perk::Source_Core |
        //                         Perk::Trigger_StatCalc,
        //         uint32_t triggerTiming = Perk::Timing_Before,
        //         std::function<bool> sendHelp = nullptr, 
        //         ...) : LingeringEffect(name, Flags, triggerTiming)
        //     {
        //         va_list args;
        //         va_start(args, sendHelp);
        //         this->perkClass = perkClass;
        //         if (sendHelp != nullptr) {
        //             applySelfToTarget = sendHelp;
        //         } else {   
        //             applySelfToTarget = std::function<bool>(&defaultTrain);
        //         }
        //     }
        // };
        class Player : public EntityInstance
        {

        };
    };
};