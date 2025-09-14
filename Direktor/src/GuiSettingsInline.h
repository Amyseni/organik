#pragma once

#ifndef LIBZHL_API
#include "Synthetik.h"
namespace Organik {
    namespace Utils {
        struct Settings {
#undef LIBZHL_API
#endif

static inline struct GUISettings
{
    GUISettings() :
        g_PlayerDisplayName("Player"),
        g_SystemColor(0xAAAAAA),  //
        g_ServiceColor(0x00AAFF),  //
        g_ChatColor(0x1cb1df),  // #1cb1df user's chat color
        g_OwnerColor(0x8B0000),  // #8B0000 owners
        g_AdminColor(0x126CB9),  // #126CB9 channel admins
        g_OperatorColor(0xFF4500),  // #FF4500 channel operators
        g_VoicedColor(0x1E90FF),  // #1E90FF voiced users
        g_DefaultColor(0xFFFFFF)  // #FFFFFF default chat color
    {};
public:
    std::string g_PlayerDisplayName;
    uint32_t g_SystemColor;
    uint32_t g_ServiceColor;
    uint32_t g_ChatColor;
    uint32_t g_OwnerColor;
    uint32_t g_AdminColor;
    uint32_t g_OperatorColor;
    uint32_t g_VoicedColor;
    uint32_t g_DefaultColor;

public:
    inline std::string* GetPlayerDisplayName() { return &g_PlayerDisplayName; }
    inline uint32_t* GetSystemColor() { return &g_SystemColor; }
    inline uint32_t* GetServiceColor() { return &g_ServiceColor; }
    inline uint32_t* GetChatColor() { return &g_ChatColor; }
    inline uint32_t* GetOwnerColor() { return &g_OwnerColor; }
    inline uint32_t* GetAdminColor() { return &g_AdminColor; }
    inline uint32_t* GetOperatorColor() { return &g_OperatorColor; }
    inline uint32_t* GetVoicedColor() { return &g_VoicedColor; }
    inline uint32_t* GetDefaultColor() { return &g_DefaultColor; }
} *g_UISettings;
#ifndef LIBZHL_API
        }; // struct Settings
    } // namespace Utils
} // namespace Organik
#undef LIBZHL_API
#endif