#pragma once

#ifndef LIBZHL_API
#include "Synthetik.h"
namespace Organik {
    namespace Utils {
        struct Settings {
#undef LIBZHL_API
#endif
static constexpr const uint32_t defaultChatColor = 0xa0a0a0;
static inline struct GUISettings
{
    GUISettings() :
        g_PlayerDisplayName("Player"),
        g_SystemColor(0xa02075),   // #a02075 system messages
        g_ChatColor(0x1cb1df),     // #1cb1df user's chat color
        g_AdminColor(0x126CB9),    // #126CB9 admins
        g_IconColors{ 
            0xa0a0a0,               // #a0a0a0 gray (default) icon color
            0x74c90c,               // #74c90c green icon color
            0xff8400,               // #ffa200 orange icon color
            0xc90a6c,               // #c90a6c purple icon color
            0x1cb1df,               // #1cb1df blue icon color
        },
        g_DefaultColor(0xFFFFFF)   // #FFFFFF default chat color
    {};
public:
    char g_PlayerDisplayName[NAME_CHAR_LIMIT];
    uint32_t g_SystemColor;
    uint32_t g_ChatColor;
    uint32_t g_AdminColor;
    uint32_t g_IconColors[5];
    uint32_t g_DefaultColor;

public:
    inline std::string GetPlayerDisplayName() { return g_PlayerDisplayName; }
    inline uint32_t* GetSystemColor() { return &g_SystemColor; }
    inline uint32_t* GetChatColor() { 
        #if !defined(PATREONBUILD)
        g_ChatColor = defaultChatColor;
        #endif
        #if defined(PATREONBUILD) 
        static_assert(false, "Patreon build defined") 
        #endif
        return &g_ChatColor;
    }
    inline uint32_t* GetAdminColor() { return &g_AdminColor; }
    inline uint32_t* GetIconColor(int32_t icon) { return &(g_IconColors[icon]); }
    inline uint32_t* GetDefaultColor() { return &g_DefaultColor; }
} *g_UISettings;
#ifndef LIBZHL_API
        }; // struct Settings
    } // namespace Utils
} // namespace Organik
#undef LIBZHL_API
#endif