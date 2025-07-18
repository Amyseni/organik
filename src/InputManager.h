#pragma once
#include "ModuleMain.h"
#include "YYToolkit/YYTK_Shared_Types.hpp"
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include "dinput.h"


using namespace YYTK;
namespace Organik
{
    namespace Input
    {

        CInstance *outgame = nullptr;
        CInstance *global = nullptr;
        struct Keybind 
        {
            enum : int32_t
            {
                TYPE_UINAV = 1<<0,
                TYPE_PLAYER = 1<<1,
                TYPE_WEAPON = 1<<2,
                TYPE_ABILITY = 1<<3,
                TYPE_NEWMENU = 1<<4,
                TYPE_IMGUI = 1<<5,
                TYPE_DEVCOM = 1<<6,
                
                MODE_ALWAYS = 1<<8,
                MODE_GAMEPLAY = 1<<9,
                MODE_PAUSED = 1<<10,
                MODE_OUTGAME = 1<<11,
            };

            enum Modifiers : int32_t
            {
                MOD_PROXIMITY = 1<<16,
                MOD_ALT_BIND = 1<<17,
                MOD_LOCALP2 = 1<<18
            };

            int32_t GetFlags();
        };
        struct InputManager
        {
        public:
            InputManager() = default;
            ~InputManager() = default;
            bool Init();

        private:
            IDirectInput8* g_pDI;
            IDirectInputDevice8* g_pInputP1;
            IDirectInputDevice8* g_pInputP2;
        };
    }
}