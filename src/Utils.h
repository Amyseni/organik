#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "YYToolkit/YYTK_Shared.hpp"

using namespace YYTK;
using namespace Aurie;


namespace Organik {
    namespace Utils {
        YYTK::CInstance* GetGlobalInstance();
        std::vector<std::string> splitString(const std::string& s, char delimiter);
        std::string joinStrings(const std::vector<std::string>& words, const std::string& delimiter = " ");
        std::string joinRValStrings(const std::vector<YYTK::RValue>& words, const std::string& delimiter = " ");
        std::vector<YYTK::RValue> findInstances(int objectID);
        int assetGetIndex(const std::string& assetName);
        void roomGoto(const std::string& roomName);
        std::vector<YYTK::CInstance*> getRoomInstances();
        std::vector<std::string> getInstanceVariableNames(YYTK::CInstance* instance);
        CRoom* Room_Data(int RoomID);
        
        typedef YYTK::CInstance* (*fn_FromInstanceID)(int32_t);
        typedef void (__thiscall YYTK::CInstance::*fn_Activate)(void);
        typedef void (__thiscall YYTK::CInstance::*fn_Deactivate)(void);
        typedef void (__thiscall YYTK::CInstance::*fn_SetDeactivated)(bool deactivated);
        typedef YYTK::CInstance* (*fn_FromInstanceID)(int32_t);
        static PVOID CInstance_Activate = nullptr;
        static PVOID CInstance_Deactivate = nullptr;
        static PVOID CInstance_SetDeactivated = nullptr;
        static fn_FromInstanceID YYGML_FindInstance = nullptr;
        YYTK::CInstance *GetInstanceFromID(int32_t id);
        
        
        PVOID GetActivateFunction();
        PVOID GetDeactivateFunction();
        PVOID GetSetDeactivatedFunction();
    }
}