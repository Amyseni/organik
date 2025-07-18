#include <unordered_map>
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include "room_list.h"
#include "ScriptHelper.h"

namespace YYTK
{
    struct CScript;
}

const char* const Organik::Objects::ObjectNamesArr[] = 
{
    OBJECTS_DO(MAKE_STRINGS)
};
const char* const Organik::Variables::VariableNamesArr[] = 
{
    VARIABLES_DO(MAKE_STRINGS)
};
const char* const Organik::Rooms::RoomNamesArr[] = 
{
    ROOMS_DO(MAKE_STRINGS)
};
const char* const Organik::Scripts::ScriptNamesArr[] = 
{
    SCRIPTS_DO(MAKE_STRINGS)
};
int32_t Organik::Variables::GetHash(const char* name)
{
    static CInstance *_globalInst;
    static YYTKInterface* _g_interface;
    if (!_g_interface)
        _g_interface = YYTK::GetInterface();
    if (!_globalInst)
        _globalInst = Utils::GetGlobalInstance();
    int32_t hash = -1;
    YYTK::GetInterface()->GetVariableSlot(Utils::getGlobalInstance(), name, hash);
    return hash;
}
std::vector<int32_t> Organik::Variables::Hashes(sizeof(Organik::Variables::VariableNamesArr) / 4);
std::vector<int32_t> Organik::Objects::ObjIndexes(sizeof(Organik::Objects::ObjectNamesArr) / 4);
std::vector<int32_t> Organik::Rooms::AssetIndexes(sizeof(Organik::Rooms::RoomNamesArr) / 4);
std::vector<int32_t> Organik::Scripts::Indexes(sizeof(Organik::Scripts::ScriptNamesArr) / 4);
std::unordered_map<int32_t, const char*> Organik::Variables::HashToVariableMap = {};
std::unordered_map<int32_t, YYTK::CScript*> Organik::Scripts::ScriptPointers;
static int dummyInit = []()->int {
    constexpr int numScripts = sizeof(Organik::Scripts::ScriptNamesArr) / 4;
    constexpr int numVariables = sizeof(Organik::Variables::VariableNamesArr) / 4;
    Organik::Scripts::ScriptPointers.reserve(numScripts);
    Organik::Variables::HashToVariableMap.reserve(numVariables);
    return numScripts;
}();