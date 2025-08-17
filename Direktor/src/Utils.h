#pragma once
#include "Synthetik.h"
#include <GameMaker_Defs.h>
#include "zhl_internal.h"
#include "d3d11.h"
#include "zhl.h"
#include <xstring>
#include "Organik/UI/UIManager.h"
#include "Logging.h"
#include "DefinitionHelpers/VariableHelper.h"
#include "imgui/imgui.h"


inline static CInstance *GetGlobalInstance() 
{
    static CInstance* globalInstance = nullptr;
    if (!globalInstance) {
        Organik::GetLogger()->LogSimple("Getting global instance");
        if (!g_pGlobalInstance) {
            Organik::GetLogger()->LogSimple("Global instance requested before ZHL initialization.");
            return nullptr;
        }
        if (!(*g_pGlobalInstance)) {
            Organik::GetLogger()->LogSimple("Global instance requested before gamemaker initialization.");
            return nullptr;
        }
        globalInstance = *g_pGlobalInstance;
        Organik::GetLogger()->LogFormatted("Global instance: %p", globalInstance);
    }
    return globalInstance;
}

inline static HWND GetWindowHandle()
{
    static HWND windowHandle = nullptr;
    if (!windowHandle) {
        Organik::GetLogger()->LogSimple("Getting window handle");
        HWND temp = FindWindowA("YYGameMakerYY", NULL);
        if (temp && IsWindow(temp) && IsWindowVisible(temp)) {
            windowHandle = temp;
            Organik::GetLogger()->LogFormatted("Window handle found: %p", windowHandle);
        } else {
            Organik::GetLogger()->LogSimple("Window handle not found or invalid.");
            return nullptr;
        }
    }
    return windowHandle;
}

namespace Organik
{
    namespace Utils
    {
        std::string url_encode(const std::string &value);
        void bugWebhook(const std::string &errorMessage);
        std::string SendHTTPSPost(const std::string& _server, const std::string& _pae, const std::string& _params = "", const std::string& _body = "");
        static ID3D11Device** const g_D3DDevice = (ID3D11Device**) (0x04267DF4);
        static ID3D11DeviceContext** const g_D3DContext = (ID3D11DeviceContext**)(0x04267DF8);
        static bool isInitializationDone()
        {
            return ImGui::GetCurrentContext() != nullptr;
        }
        static volatile bool g_needUpdateActiveInstances = false;
        struct Settings 
        {
            static bool g_EnableMultiplayerCompat; // Enable compatibility with 26.1 multiplayer;
            static bool g_EnableInvincibility; // Enable invincibility in the game; 
        };

        static inline std::unordered_map<int32_t, std::string> GetVariableMap(CInstance* inst)
        {
            std::unordered_map<int32_t, std::string> variableMap = {};
            if (!inst || !inst->m_YYVarsMap) {
                Organik::GetLogger()->LogSimple("Invalid instance.");
                return variableMap;
            }
            CHashMapElement<int, RValue*> currentElement;
            for (int32_t i = 1; i < inst->m_YYVarsMap->m_CurrentSize; i++) {
                if ((inst->m_YYVarsMap->m_Elements[i].m_Hash & inst->m_YYVarsMap->m_CurrentMask) == HASH_EMPTY || (inst->m_YYVarsMap->m_Elements[i].m_Hash & HASH_DELETED) == HASH_DELETED)
                    continue;
                Organik::GetLogger()->LogFormatted("Getting variable %d from instance %p", i, inst);
                currentElement = inst->m_YYVarsMap->m_Elements[i];
                int32_t hash = currentElement.m_Hash & inst->m_YYVarsMap->m_CurrentMask;
                Organik::GetLogger()->LogFormatted("Variable %d hash: %d", i, hash);
                const char* name = Organik::Variables::HashToVariableMap[hash];
                Organik::GetLogger()->LogFormatted("Variable %d name: %s", i, name);
                if (name) {
                    variableMap[hash] = name;
                } else {
                    continue;
                }
            }
            return variableMap;
        }
    }
}
