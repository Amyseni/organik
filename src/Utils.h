#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "YYToolkit/YYTK_Shared.hpp"
#include "ModuleMain.h"

using namespace YYTK;
using namespace Aurie;
#define INLINE_GETTER(varname, myType, init) \
    static inline myType get##varname() \
    { \
        static myType varname = init; \
        return varname; \
    }
#define INLINE_BUILTIN_GETTER(varname, fnName) \
    INLINE_GETTER(varname, YYTK::TRoutine, \
        nullptr; \
        if (!(##varname)) { \
            Organik::GetLogger()->LogSimple("Resolving " #fnName " builtin"); \
            g_ModuleInterface->GetNamedRoutinePointer(#fnName, reinterpret_cast<PVOID*>(&##varname)); \
        } \
    )

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
        CInstance *builtinInstanceFind(int assetIndex, int number);
        int builtinInstanceNumber(int assetIndex);
        int builtinAssetGetIndex(const std::string &assetName);
        void builtinRoomGoto(const std::string &roomName);
        CInstance* spawnObjectHere(const std::string &objectName, int x = -1, int y = -1);
        std::vector<int> knownInstanceIDs(int newInstance = -1);
        INLINE_GETTER(OutgameIndex, int, builtinAssetGetIndex("outgame"))
        INLINE_GETTER(Obj_PlayerIndex, int, builtinAssetGetIndex("obj_player"))
        INLINE_GETTER(Obj_Chat_ParentIndex, int, builtinAssetGetIndex("obj_chat_parent"))
        INLINE_GETTER(Obj_StatsIndex, int, builtinAssetGetIndex("obj_stats"))
        INLINE_GETTER(Obj_Item_ParentIndex, int, builtinAssetGetIndex("obj_item_parent"))
        INLINE_GETTER(Obj_Perk_ParentIndex, int, builtinAssetGetIndex("obj_perk_parent"))
        INLINE_GETTER(Obj_Weapon_ParentIndex, int, builtinAssetGetIndex("obj_weapon_PARENT"))
        INLINE_GETTER(Obj_Pickup_ParentIndex, int, builtinAssetGetIndex("obj_pickup_parent"))
        INLINE_GETTER(Obj_Item_SlotIndex, int, builtinAssetGetIndex("obj_item_slot"))
        INLINE_BUILTIN_GETTER(LayerGetAll, layer_get_all)
        INLINE_BUILTIN_GETTER(InstanceNumber, instance_number)
        INLINE_BUILTIN_GETTER(InstanceFind, instance_find)
        INLINE_BUILTIN_GETTER(RoomGoto, room_goto)
        INLINE_BUILTIN_GETTER(AssetGetIndex, asset_get_index)
        
        CRoom* Room_Data(int RoomID);
        
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
        YYTK::CInstance *GetInstanceFromID(int32_t id);
        
        
        PVOID GetActivateFunction();
        PVOID GetDeactivateFunction();
        PVOID GetSetDeactivatedFunction();
    }
}