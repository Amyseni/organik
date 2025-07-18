#pragma once
#include <unordered_map>
#include <string>
#include <vector>

namespace Organik
{
    namespace Config
    {
        enum KeybindAction : uint32_t
        {
            Left = 1<<0,
            Right = 1<<1, 
            Up = 1<<2,
            Down = 1<<3,
            Eject = 1<<4,
            Reload = 1<<5,
            Dash = 1<<6,

            Num_1 = 1<<7,
            Num_2 = 1<<8,
            Num_3 = 1<<9,
            Num_4 = 1<<10,
            Num_5 = 1<<11,
            Num_6 = 1<<12,
            Num_7 = 1<<13,
            Num_8 = 1<<14,
            Num_9 = 1<<15,
            Num_0 = 1<<16,

            Mod_UseItem = 1<<17,
            Mod_UseAbility = 1<<18,
            Mod_Terminal = 1<<29,
            Mod_Movement = 1<<30,
            Mod_UINav = 1<<31
        };
        struct KeybindConfig
        {
            KeybindConfig();
            ~KeybindConfig();

            bool Load();
            bool Save();

            void ResetToDefaults();


            // Map: 
            /*
                KeybindAction (represents a game action to check) ->
                {
                    uint32_t Hash - Hash of the keyboard shortcut (e.g. Ctrl+Q, Alt+Enter)
                    std::string DisplayStr - Human-readable string representation of Hash.
                }
            */
            std::unordered_map<KeybindAction, std::unordered_map<uint32_t, std::string>> Keybinds;
        };
        static KeybindConfig& GetKeybindConfig()
        {
            static KeybindConfig config;
            return config;
        }
    }
}

