#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "YYToolkit/YYTK_Shared.hpp"
#include "Aurie/shared.hpp"
#include "ModuleMain.h"
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include <random>

using namespace YYTK;
using namespace Aurie;

/*
    honestly, this is where the magic happens.
    it looks so ugly though


    I'm not sorry
*/

#define INLINE_GETTER_CONST(varname, myType, init) \
    static inline myType get##varname() \
    { \
        static myType varname = init; \
        return varname; \
    }
#define INLINE_BUILTIN_FN_GETTER(varname, fnName) \
    static inline YYTK::TRoutine get##varname() {\
        static YYTK::TRoutine varname; \
        if (varname == nullptr) { \
            Organik::GetLogger()->LogSimple("Resolving " #fnName " builtin"); \
            g_ModuleInterface->GetNamedRoutinePointer(#fnName, reinterpret_cast<PVOID*>(&##varname)); \
        } \
        if (!varname) \
        { \
            MessageBoxA(nullptr, "Failed to resolve " #fnName " builtin. Probably the game will crash now.", "Error", MB_ICONERROR); \
            static TRoutine empty = [](RValue & result, YYTK::CInstance *self, YYTK::CInstance *other, int argc, RValue *args) -> void { \
                result = ref_noone; \
                return; \
            }; \
            return empty; \
        } \
        return varname; \
    }
#define INLINE_BUILTIN_VAR_GETTER(varname, fnName) \
    static inline RValue& fnName() { \
        static RValue *varname; \
        static int varname##Index; \
        static RVariableRoutine *varname##Routine; \
        if (varname == nullptr || varname##Routine == nullptr) { \
            Organik::GetLogger()->LogSimple("Resolving " #varname " builtin"); \
            g_ModuleInterface->GetBuiltinVariableIndex(#varname,&varname##Index); \
            g_ModuleInterface->GetBuiltinVariableInformation(varname##Index, &varname##Routine); \
        } \
        if (!varname && !varname##Routine) \
        { \
            MessageBoxA(nullptr, "Failed to resolve " #varname " builtin. Probably the game will crash now.", "Error", MB_ICONERROR); \
            return nullptr; \
        } \
        if (varname##Routine) \
            if(!varname##Routine->m_GetVariable(Organik::Utils::getGlobalInstance(), varname##Index, varname)) \
                if (!varname) \
                    return nullptr; \
        return varname; \
    }
#define INLINE_SCRIPT_GETTER(varname, fnName) \
    static inline YYTK::CScript* get##varname() {\
        static YYTK::CScript* varname; \
        if (varname == nullptr) { \
            Organik::GetLogger()->LogSimple("Resolving " #fnName " builtin"); \
            g_ModuleInterface->GetNamedRoutinePointer(#fnName, reinterpret_cast<PVOID*>(&##varname)); \
        } \
        if (!varname) \
        { \
            MessageBoxA(nullptr, "Failed to resolve " #fnName " builtin. Probably the game will crash now.", "Error", MB_ICONERROR); \
            return nullptr; \
        } \
        return varname; \
    }
#define INLINE_GLOBALSTATIC_GETTER(varname, type_name, sig, pmask) \
    static inline type_name *get##varname() \
    { \
        static type_name *varname; \
        if (varname != nullptr) \
            return varname; \
        static std::wstring game_exe = Organik::Utils::GetGameExe(); \
        Organik::GetLogger()->LogSimple("Resolving " #varname " global pointer"); \
        PVOID foundPtr = (PVOID) Aurie::MmSigscanModule(game_exe.c_str(), sig, pmask); \
        if (foundPtr != nullptr) \
        {\
            varname = reinterpret_cast<type_name*>(foundPtr); \
            return varname; \
        }\
        return nullptr; \
    }
#define INLINE_FUNCTION_GETTER(fnName, retType, sig, mask, ...) \
    typedef retType (*fn_get##fnName)(__VA_ARGS__); \
    static inline fn_get##fnName get##fnName() \
    { \
        static retType (*fn)(__VA_ARGS__); \
        if (fn != nullptr) \
            return fn; \
        Organik::GetLogger()->LogSimple("Resolving " #fnName " function pointer"); \
        PVOID foundPtr = reinterpret_cast<PVOID>(Aurie::MmSigscanModule(Organik::Utils::GetGameExe().c_str(), UTEXT(#sig), #mask)); \
        if (foundPtr != nullptr) \
        { \
            fn = reinterpret_cast<fn_get##fnName>(foundPtr); \
            return fn; \
        } \
        Organik::Utils::Crash("Failed to resolve " #fnName " function pointer."); \
    }
    
namespace Organik {
    namespace Utils {
        YYTK::CInstance* GetGlobalInstance();
        std::vector<std::string> splitString(const std::string& s, char delimiter);
        std::string joinStrings(const std::vector<std::string>& words, const std::string& delimiter = " ");
        std::string joinRValStrings(const std::vector<YYTK::RValue>& words, const std::string& delimiter = " ");
        // int assetGetIndex(const std::string& assetName);
        // void roomGoto(const std::string& roomName);
        
        std::vector<YYTK::CInstance*> getRoomInstances();
        std::vector<std::string> getInstanceVariableNames(YYTK::CInstance* instance);
        std::vector<YYTK::CInstance*> findInstances(const std::string& objectName);
        std::vector<YYTK::CInstance*> findInstances(int objectID);
        YYTK::CInstance *builtinInstanceFind(int assetIndex, int number);
        int builtinInstanceNumber(int assetIndex);
        int builtinAssetGetIndex(const std::string &assetName);
        void builtinRoomGoto(const std::string &roomName);
        YYTK::CInstance* spawnObjectHere(const std::string &objectName, int x = -1, int y = -1);
        std::vector<int> knownInstanceIDs(int newInstance = -1);
        std::string url_encode(const std::string &value);
        void bugWebhook(const std::string &errorMessage);
        static inline std::wstring CharPToWstring(const char* _charP)
        {
            return std::wstring(_charP, _charP + strlen(_charP));
        }
        std::string SendHTTPSPost(const std::string& _server, const std::string& _pae, const std::string& _params = "", const std::string& _body = "");
        static void Crash(const std::string& message)
        {
            g_ModuleInterface->GetRunnerInterface().YYError(message.c_str());
        }
        static inline std::wstring GetGameExe()
        {
            static std::wstring game_exe;
            if (game_exe.empty()) {
                if (!AurieSuccess(MdGetImageFilename(g_ArInitialImage, game_exe))) {
                    Utils::Crash("Error: Could not get game executable name");
                }
            }
            return game_exe;
        }
        static inline std::mt19937 getrng()
        {
            static std::random_device rd; // ARE EN GEE
            static std::mt19937 gen(rd()); // Standard mersenne_twister_engine
            return gen;
        } 
        
        INLINE_GETTER_CONST(OutgameIndex, int, builtinAssetGetIndex("outgame"))
        INLINE_GETTER_CONST(Obj_PlayerIndex, int, builtinAssetGetIndex("obj_player"))
        INLINE_GETTER_CONST(Obj_AllgameIndex, int, builtinAssetGetIndex("obj_allgame"))
        INLINE_GETTER_CONST(Obj_Chat_ParentIndex, int, builtinAssetGetIndex("obj_chat_parent"))
        INLINE_GETTER_CONST(Obj_StatsIndex, int, builtinAssetGetIndex("obj_stats"))
        INLINE_GETTER_CONST(Obj_Item_ParentIndex, int, builtinAssetGetIndex("obj_item_parent"))
        INLINE_GETTER_CONST(Obj_Perk_ParentIndex, int, builtinAssetGetIndex("obj_perk_parent"))
        INLINE_GETTER_CONST(Obj_Weapon_ParentIndex, int, builtinAssetGetIndex("obj_weapon_PARENT"))
        INLINE_GETTER_CONST(Obj_Pickup_ParentIndex, int, builtinAssetGetIndex("obj_pickup_parent"))
        INLINE_GETTER_CONST(Obj_Item_SlotIndex, int, builtinAssetGetIndex("obj_item_slot"))
        INLINE_BUILTIN_FN_GETTER(LayerGetAll, layer_get_all)
        INLINE_BUILTIN_FN_GETTER(InstanceNumber, instance_number)
        INLINE_BUILTIN_FN_GETTER(InstanceFind, instance_find)
        INLINE_BUILTIN_FN_GETTER(RoomGoto, room_goto)
        INLINE_BUILTIN_FN_GETTER(AssetGetIndex, asset_get_index)
        INLINE_BUILTIN_FN_GETTER(InstanceDestroy, instance_destroy)
        INLINE_BUILTIN_FN_GETTER(InstanceExists, instance_exists)
        INLINE_BUILTIN_FN_GETTER(BufferWrite, buffer_write)
        INLINE_BUILTIN_FN_GETTER(BufferCreate, buffer_create)
        
        INLINE_GLOBALSTATIC_GETTER(Run_Room, YYTK::CRoom*, UTEXT("\x00\x00\x00\x00\x85\xF6\x74\x00\x8B\xB6\x00\x00\x00\x00\x85\xF6\x74\x00\x8B\x86"), "????xxx?xx????xxx?xx")
        INLINE_GLOBALSTATIC_GETTER(CurrentRoom, int, UTEXT("\x00\x00\x00\x00\x83\xC4\x04\x85\xC9\x78"), "????xxxxxx")
        INLINE_GLOBALSTATIC_GETTER(RoomItems, int, UTEXT("\x00\x00\x00\x00\x3B\xDE\x0F\x8D\x00\x00\x00\x00\xA1"), "????xxx????x")
        INLINE_GLOBALSTATIC_GETTER(Rooms, YYTK::CRoom**, UTEXT("\x00\x00\x00\x00\x83\x3C\x00\x00\x0F\x84\x00\x00\x00\x00\x56\x68"), "????xx?xxx????xx")
        INLINE_GLOBALSTATIC_GETTER(GlobalInstance, YYTK::CInstance*, UTEXT("\x00\x00\x00\x00\x53\xC7\x46\x00\x00\x00\x00\x00\xC7\x06\x00\x00\x00\x00"), "????xxx?????xxxxxx")
        INLINE_GLOBALSTATIC_GETTER(Scripts, CScript**, UTEXT("\x00\x00\x00\x00\x8B\x6C\x24\x00\x2B\xD8\x8D\x3C"), "????xxx?xxxx")
        INLINE_GLOBALSTATIC_GETTER(ScriptsCount, int, UTEXT("\x00\x00\x00\x00\x83\xEE\x01\x57"), "????xxxx")
        
        INLINE_FUNCTION_GETTER(
            Variable_GetValue_Direct, 
            bool, 
            \x8B\x44\x24\x00\x53\x8B\x5C\x24\x00\xC6\x44\x24\x00\x00, 
            xxx?xxxx?xxx?x,
            YYObjectBase* inst, int var_ind, int array_ind, RValue *val, bool fPrepareArray, bool fPartOfPop
        )
        INLINE_FUNCTION_GETTER(
            Function_Replace, 
            void,
            \x53\x8B\x1D\x00\x00\x00\x00\x56\x33\xF6\x83\xEB\x01, 
            xxx????xxxxxx,
            YYObjectBase* inst, int var_ind, int array_ind, RValue *val, bool fPrepareArray, bool fPartOfPop
        )

        YYTK::CRoom* Room_Data(int RoomID);
        
        typedef YYTK::CInstance* (*fn_FromInstanceID)(int32_t);
        typedef void (__thiscall YYTK::CInstance::*fn_Activate)(void);
        typedef void (__thiscall YYTK::CInstance::*fn_Deactivate)(void);
        typedef void (__thiscall YYTK::CInstance::*fn_SetDeactivated)(bool deactivated);
        typedef YYTK::CInstance* (*fn_FromInstanceID)(int32_t);
        static PVOID CInstance_Activate = nullptr;
        static PVOID CInstance_Deactivate = nullptr;
        static PVOID CInstance_SetDeactivated = nullptr;
        static YYTK::TRoutine instanceFind = nullptr;
        static fn_FromInstanceID YYGML_FindInstance = nullptr;
        static bool g_EnableMultiplayerCompat = false;
        static bool g_EnableDevcom = false;
        static bool g_EnableInvincibility = false;

        /* ================================================== */
        /*           PATCHES, FIXES, FEATURE TESTS            */
        /* ================================================== */
        
        static bool g_MinDMGFix         = true;
        static bool g_HGSlowdownFix     = true;
        static bool g_SSSUnlocksS       = true;
        static bool g_NewVariantTests   = true; /*WIP*/
        static bool g_ReplaceRNG        = true; /*WIP*/
        static bool g_ForceHandleInput  = false;
        static bool g_DeepMultiCompat   = false;
        static bool g_NewWeaponTesting  = false;
        static bool g_NewPlayerStats    = false;
        static bool g_NewItemTesting    = false;
        static bool g_NewCollisions     = false;

        /* ================================================== */
        

        static void Crash(const std::string& message);
        YYTK::CInstance* FirstInstanceOrNullptr(int objectIndex);

        YYTK::CInstance *GetInstanceFromID(int32_t id);
        static inline std::string GetPlayerName();
        static inline YYTK::CRoom *GetCurrentRoom()
        {
            static YYTK::CInstance* myGlobal;
            if (!myGlobal)
                g_ModuleInterface->GetGlobalInstance(&myGlobal);
            if (myGlobal == nullptr)
                Organik::Utils::Crash("GetCurrentRoom: Global instance is null");

            YYTK::CRoom* currentRoom = nullptr;
            g_ModuleInterface->GetCurrentRoomData(currentRoom);
            return currentRoom;
        }
        static inline const std::string& VarHashToName(int32_t hash)
        {
            if (Organik::Variables::HashToVariableMap.contains(hash))
                return Organik::Variables::HashToVariableMap[hash];
            static std::string emptyString = "";
            GetLogger()->TryLogConsole("Error: Variable hash %d not found in map", hash);
            return emptyString; 
        }

        static inline int32_t GetVarHash(const std::string& name)
        {
            static YYTKInterface* _interface;
            if (!_interface)
                _interface = YYTK::GetInterface();
            int32_t hash = -1;
            _interface->GetVariableSlot(GetGlobalInstance(), name.c_str(), hash);
            return hash;
        }
        
        /**
         * @brief basically, fast inverse sqrt but for exp(x) (e^x).
         * So we can go fast while calculating how fast we can go.
         * The core identity: e^x = 2^(x * log2(e))
         */
        inline double fast_exp(double x) {

            // We calculate v = x * log2(e) and then find 2^v.
            const double log2_e = 1.4426950408889634;
            double v = x * log2_e;

            // basically, i is bit magic and f is the remainder we have at home
            const int64_t i = static_cast<int64_t>(v);
            const double f = v - i;
            
            // always remember to lie to the compiler about types :)
            union
            {
                int64_t i;
                double d;
            } u;

            u.i = (i + 1023) << 52; // this is the line that actually does the magic. No, I don't really get it either.

            // the magic numbers are pre-computed fractions
            // this is just a polynomial from high school geometry class, but way more fucked up
            const double pow2_f_approx = 0.99992522 + f * (0.69583354 + f * (0.22606716 + f * 0.078125));

            // what if we shifted our bits together under the fast inverse square root?
            return u.d * pow2_f_approx;
        }
        
        PVOID GetActivateFunction();
        PVOID GetDeactivateFunction();
        PVOID GetSetDeactivatedFunction();
    }
}