#pragma once
#include <string>
#include "../VariableHelper.h"
#include "../ScriptHelper.h"
#include "../InstanceHelper.h"
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
        struct StatModifier
        {
            enum : uint32_t
            {
                Flat = 1<<0,
                Additive = 1<<1,
                Multiplicative = 1<<2,
                Mod_Buff = 1<<6,
                Mod_Debuff = 1<<7
            };

            uint32_t _hash;
            
            bool permanent;
            double totalDuration;
            double lifetime;
            uint32_t targetHash;
            int32_t targetObj;
            uint32_t stacks;
            uint32_t maxStacks;
            double value;
            
            const std::string& displayName;
            const std::string& description;
            const std::string& statName;
        };
        
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
        
        struct LingeringEffect
        {
            const std::string& DisplayName;
            
            enum : uint32_t
            {
                None = 0,

                Source_Core = 1<<10,                // triggered by class core talents/abilities.
                Source_Talent = 1<<11,              // talents and perks grouped together
                Source_Ability = 1<<12,             // abilities and items grouped together 
                Source_Difficulty = 1<<13,          // triggered by a difficulty modifier. (incl. user-defined modifiers)
                
                Trigger_Movement = 1<<14,           // Player character movement event. Does not trigger on input not causing movement.
                Trigger_Event = 1<<15,              // CEvent::Execute trigger
                
                Trigger_StatCalc = 1<<16,           // Stats are about to be calculated for this game step.
                Trigger_ReloadCallback = 1<<17,     // Active reload callback (paases status (e.g. success, fail, perfect) in datavals)
                Trigger_InputCallback = 1<<19,      // Key, Mouse, Gamepad, etc. event
                
                Trigger_GMLCallback = 1<<20,        // GML Callback 
                Trigger_InstanceCallback = 1<<21,   // Instance Callback. For in-engine object events.
                Trigger_EntityCallback = 1<<22,     // Entity Callbacks. For Organik-managed instance/object events.
                Trigger_RenderCallback = 1<<23,     // D3D Frame callback (::Present)
                
                Trigger_RoomCallback = 1<<24,       // Room start/end callbacks
                Trigger_FloorCallback = 1<<25,      // Floor callbacks (before/after boss fights, etc.)
                Trigger_LevelGenCallback = 1<<26,   // Level gen callback. Called on EVERY TILE OR OBJECT as they are generated.
                
                Trigger_CombatCallback = 1<<27,     // Combat start/end
                Trigger_OnHitEnemy = 1<<28,         // Enemy hit (bullet, item, etc.) (by player, by NPC, etc.)
                Trigger_OnGetHit = 1<<29,           // Player hit (by bullet, explosion, level hazard, etc.)
                
                Trigger_Custom = 1<<30,             // Custom trigger. Allows a custom (boolean) statement to be checked every frame.
                                                    // Applied when the statement returns a truthy value
            } Flags;

            enum : uint32_t
            {
                Timing_Before = 1<<0,           // e.g. Create_0_Before, Keyboard_117_Before, etc.
                Timing_After = 1<<1,            // e.g. Destroy_0_After, gml_Script_scr_goto_room_Before, etc.
                Timing_BeforeAndAfter = 1<<2,
                Timing_FixedTimer = 1<<3,       

            } TriggerTiming;

            bool showInHUD = true;
            
            LingeringEffect()
                : DisplayName("Unnamed Effect"), Flags(None), TriggerTiming(Timing_BeforeAndAfter) {};
            LingeringEffect(char* name, uint32_t flags, uint32_t triggerTiming = Timing_BeforeAndAfter)
                : DisplayName(name), Flags(None), TriggerTiming(Timing_BeforeAndAfter) {};

            
        };

        typedef [Vec2*] bool (LingeringEffect::*ApplyEffectFn)(const RValue* value);
        struct EntityPosition
        {
            Vec2 coords;
            Vec2 targetCoords; // aim x and y coordinates (mouse position for players, look target for NPCs, 
            const float maxSpeed; // represents combined x and y speed

            double movementTime = 0.0; // time spent moving in seconds;
            /**
             * @brief Calculates the player's speed based on the time spent moving.
             * Uses the formula: y = LIMIT * (1 - exp(-K_FACTOR * x))
             * with a high-performance approximation for exp().
             *
             * @param time The total time the character has spent moving (input x).
             * @return double The resulting speed (output y).
             */
            inline double getVelocity(double time) 
            {
                // This K_FACTOR determines how quickly the curve reaches the limit.
                // A value of ~2.6 gives a very snappy response, reaching ~92% of max at x=1.5.
                const double K_FACTOR = 2.608;

                // Ensure input is non-negative to prevent exp() from becoming huge.
                double non_negative_time = max(0.0, time);
                
                return maxSpeed * (1.0 - Organik::Utils::fast_exp(-K_FACTOR * non_negative_time));
            }

            auto (EntityPosition::*movement)(const double deltaTime, float lMult, float rMult, float uMult, float dMult) -> Vec2* = 0;

            auto (EntityPosition::*accellerate)(const double deltaTime, float direction) -> float = 0;
            
            EntityPosition(float x = 0.0f, float y = 0.0f, float direction = 0.0f, float maxSpeed = 7.95f, auto (EntityPosition::*accel)(const double deltaTime, float direction) -> float)
                : coords(x, y), maxSpeed(maxSpeed),
                    accellerate(accel)
            {

            }
        };

        struct EntityStats
        {
            using vars = Organik::Variables;
            std::unordered_map<uint32_t, std::pair<const std::string&, RValue*>> m_Stats;
            EntityStats() {

                /* == CURRENT HP == */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::hpCur], std::make_pair(vars::VariableNamesArr[vars::currenthp], &RValue(1075.00))
                }));
                /* == MAX HP == */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::hpMax], 
                    std::make_pair(vars::VariableNamesArr[vars::hpMax], &RValue(1075.00))
                }));
                /* == SHIELD == */
                /* == CURRENT SHIELD == */
                /* == POSITION == */

                /* TARGET POSITION (relative to self entity) */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::targetPosition], 
                    std::make_pair(vars::VariableNamesArr[vars::targetPosition], &RValue({0.0f, 0.0f}))
                }));
                /* ARMOR PEN */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::armorpen], std::make_pair(vars::VariableNamesArr[vars::armorpen], &RValue(0.0))
                }));
                /* ARMOR */
                /* CRIT CHANCE */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::crit], std::make_pair(vars::VariableNamesArr[vars::crit], &RValue(0.03))
                }));
                /* CRIT DAMAGE */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::critdmg], std::make_pair(vars::VariableNamesArr[vars::critdmg], &RValue(2.0))
                }));

                /* MOVESPEED */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::movementSpeed], std::make_pair(vars::VariableNamesArr[vars::movementSpeed], &RValue(0.0))
                }));
                /* DODGE CHANCE */
                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::dodgechance], std::make_pair(vars::VariableNamesArr[vars::dodgechance], &RValue(0.0))
                }));

                m_Stats.insert(std::pair<uint32_t, std::pair<const std::string&, RValue*>>({
                    vars::Hashes[vars::perks], std::make_pair(vars::VariableNamesArr[vars::perks], &RValue(std::vector<Organik::Entities::Perk*>().data()))
                }));
            }
            static EntityStats* getDefault()
            {
                return &EntityStats();
            }
        };
        class EntityInstance
        {
        public:
            EntityInstance(Vec2* position = &Vec2(0.0f,0.0f), EntityStats* stats = EntityStats::getDefault())
                : m_Stats(stats)
            {
                
            };
            virtual ~EntityInstance() = default;

            // Getters and Setters
            virtual std::string GetName() const = 0; // Get the name of the carryable item
            virtual void SetName(const std::string& name) = 0; // Set the name of the carryable item
            
            virtual EntityStats* GetStats() const = 0; // Get the stats 
            virtual EntityPosition* GetMovement() const = 0; // Get the movement component

        private:
            EntityStats* m_Stats = nullptr;
            EntityPosition* m_Position = nullptr;
        };
    }
}