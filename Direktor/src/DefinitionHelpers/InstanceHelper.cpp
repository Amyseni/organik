#include "Synthetik.h"
#include <unordered_map>
#include "InstanceHelper.h"
#include "VariableHelper.h"
#include "DefinitionHelpers/BuiltinHelper.h"
#include "room_list.h"
#include "ScriptHelper.h"
#define MAKE_SCRIPTIDS(script) Script_Find_Id(const_cast<char*>(#script)),
#define MAKE_HASHES(var) Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(#var)),
#define MAKE_ASSETINDEXES(object) DoBuiltin(&gml_asset_get_index, {RValue(#object)}).m_i32,
#define MAKE_VARMAP(var) {Code_Variable_FindAlloc_Slot_From_Name(GetGlobalInstance(), const_cast<char*>(#var)), #var},
std::vector<int32_t> Organik::Variables::Hashes;
std::vector<int32_t> Organik::Objects::ObjIndexes;
std::vector<int32_t> Organik::Rooms::AssetIndexes;
std::vector<int32_t> Organik::Scripts::Indexes;
std::unordered_map<int32_t, const char*> Organik::Variables::HashToVariableMap;
std::unordered_map<int32_t, CScript*> Organik::Scripts::scriptPointerMap;
std::unordered_map<const char*, int32_t> Organik::Variables::varToIndexMap;

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
constexpr int variableCount = sizeof(Organik::Variables::VariableNamesArr) / sizeof(char* );
constexpr int objectCount = sizeof(Organik::Objects::ObjectNamesArr) / sizeof(char* );
constexpr int roomCount = sizeof(Organik::Rooms::RoomNamesArr) / sizeof(char* );
constexpr int scriptCount = sizeof(Organik::Scripts::ScriptNamesArr) / sizeof(char* );
void Organik::DoHelperSetup()
{
    Organik::Variables::HashToVariableMap.reserve(variableCount);
    Organik::Variables::varToIndexMap.reserve(variableCount);
    Organik::Scripts::scriptPointerMap.reserve(scriptCount);
    // std::cout << "Setting up helper definitions..." << std::endl;
    for (int i=0;i<variableCount;i++)
    {
        int32_t hash = Code_Variable_FindAlloc_Slot_From_Name(
            GetGlobalInstance(), 
            const_cast<char*>(
                Organik::Variables::VariableNamesArr[i]
            ));

        Organik::Variables::Hashes.push_back(hash);
        Organik::Variables::varToIndexMap[Organik::Variables::VariableNamesArr[i]] = i;
        Organik::Variables::HashToVariableMap.insert_or_assign(hash, reinterpret_cast<const char*>(Organik::Variables::VariableNamesArr[i]));
    }
   Organik::GetLogger()->LogSimple("Setting up object indexes...");
    for (int i=0;i<objectCount;i++)
    {
        Organik::Objects::ObjIndexes.push_back(
            DoBuiltin(&gml_asset_get_index, {RValue(Organik::Objects::ObjectNamesArr[i])}).m_i32
        );
    }
    Organik::GetLogger()->LogSimple("Object indexes initialized.");
    Organik::GetLogger()->LogSimple("Setting up room indexes...");

    for (int i=0;i<roomCount;i++)
    {
        Organik::Rooms::AssetIndexes.push_back(
            DoBuiltin(&gml_asset_get_index, {RValue(Organik::Rooms::RoomNamesArr[i])}).m_i32
        );
    }
    Organik::GetLogger()->LogSimple("Room indexes initialized.");
    Organik::GetLogger()->LogSimple("Setting up script indexes...");

    for (int i=0;i<scriptCount;i++)
    {
        Organik::Scripts::Indexes.push_back(
            Script_Find_Id(const_cast<char*>(Organik::Scripts::ScriptNamesArr[i]))-100000
        );
        Organik::Scripts::scriptPointerMap.insert_or_assign(
            Organik::Scripts::Indexes[i],
            ScriptFromId(Organik::Scripts::Indexes[i])
        );
    }
    Organik::GetLogger()->LogSimple("Script indexes initialized.");
    Organik::GetLogger()->LogSimple("Helper definitions set up successfully.");
   
}