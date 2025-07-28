#include "Synthetik.h"
#include <unordered_map>
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include "room_list.h"
#include "ScriptHelper.h"

std::vector<int32_t> Organik::Variables::Hashes;
std::vector<int32_t> Organik::Objects::ObjIndexes;
std::vector<int32_t> Organik::Rooms::AssetIndexes;
std::vector<int32_t> Organik::Scripts::Indexes;
std::unordered_map<int32_t, const char*> Organik::Variables::HashToVariableMap;
std::unordered_map<int32_t, CScript*> Organik::Scripts::ScriptPointers;

HOOK_STATIC(Graphics, OpenWindow, (HWND g_hwnd, int width, int height, int debugMode) -> void)
{
    Organik::Variables::Hashes = std::vector<int32_t>(sizeof(Organik::Variables::VariableNamesArr) / 4);
    Organik::Objects::ObjIndexes = std::vector<int32_t>(sizeof(Organik::Objects::ObjectNamesArr) / 4);
    Organik::Rooms::AssetIndexes = std::vector<int32_t>(sizeof(Organik::Rooms::RoomNamesArr) / 4);
    Organik::Scripts::Indexes = std::vector<int32_t>(sizeof(Organik::Scripts::ScriptNamesArr) / 4);
    Organik::Variables::HashToVariableMap = {};
    Organik::Scripts::ScriptPointers;
    constexpr int numScripts = sizeof(Organik::Scripts::ScriptNamesArr) / 4;
    constexpr int numVariables = sizeof(Organik::Variables::VariableNamesArr) / 4;
    Organik::Scripts::ScriptPointers.reserve(numScripts);
    Organik::Variables::HashToVariableMap.reserve(numVariables);
    super(g_hwnd, width, height, 0);
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
    int32_t hash = -1;
    hash = Code_Variable_FindAlloc_Slot_From_Name(g_pGlobalInstance->m_prototype, const_cast<char*>(name));
    return hash;
}
