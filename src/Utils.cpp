#include "Utils.h"
#include <string>
#include <vector>
#include <sstream>
#include "ModuleMain.h"
#include "Logging.h"
#include "YYToolkit/YYTK_Shared.hpp"



using namespace YYTK;
using namespace Aurie;
using namespace Organik;

static YYTK::CInstance* g_GlobalInstance = nullptr;

YYTK::CInstance* Organik::Utils::GetGlobalInstance()
{
    if (g_GlobalInstance == nullptr)
    {
        Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&g_GlobalInstance);
        if (last_status != Aurie::AURIE_SUCCESS || !g_GlobalInstance)
        {
            GetLogger()->LogSimple("Error: Global instance not found");
            return nullptr;
        }
    }
    return g_GlobalInstance;
}

std::vector<std::string> Organik::Utils::splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string Organik::Utils::joinStrings(const std::vector<std::string>& words, const std::string& delimiter)
{
    std::string result = "";

    for (size_t i = 0; i < words.size(); ++i) {
        result += words[i];
        if (i < words.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}
std::string Organik::Utils::joinRValStrings(const std::vector<YYTK::RValue>& words, const std::string& delimiter)
{
    std::string result = "";

    for (size_t i = 0; i < words.size(); ++i) {
        result += words[i].ToString();
        if (i < words.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}
CInstance *Organik::Utils::builtinInstanceFind(int assetIndex, int number)
{
    auto args = std::vector<RValue>(
        {
            RValue(assetIndex), 
            RValue(number)
        }
    );
    if (!Organik::Utils::getInstanceFind())
    {
        GetLogger()->LogFormatted("Couldn't resolve builtin: %s", __FUNCTION__);
        return nullptr;
    }
    RValue result;
    Organik::Utils::getInstanceFind()(result, Organik::Utils::GetGlobalInstance(), Organik::Utils::GetGlobalInstance(), static_cast<int>(args.size()), args.data());
    if (result.ToInt32() > 0)
    {
        return Organik::Utils::GetInstanceFromID(result.ToInt32());
    }
}
int Organik::Utils::builtinInstanceNumber(int assetIndex)
{
    auto args = std::vector<RValue>(
        {
            RValue(assetIndex)
        }
    );
    if (!Organik::Utils::getInstanceNumber())
    {
        GetLogger()->LogSimple("Couldn't resolve builtin");
        return -1;
    }
    RValue result;
    Organik::Utils::getInstanceNumber()(result, Organik::Utils::GetGlobalInstance(), Organik::Utils::GetGlobalInstance(), static_cast<int>(args.size()), args.data());
    return result.ToInt32();
}
int Organik::Utils::builtinAssetGetIndex(const std::string &assetName)
{
    auto args = std::vector<RValue>(
        {
            RValue(assetName.c_str())
        }
    );
    if (!Organik::Utils::getAssetGetIndex())
    {
        GetLogger()->LogSimple("Couldn't resolve builtin");
        return ref_noone.ToInt32();
    }
    RValue result;
    Organik::Utils::getAssetGetIndex()(result, Organik::Utils::GetGlobalInstance(), Organik::Utils::GetGlobalInstance(), static_cast<int>(args.size()), args.data());
    return result.ToInt32();
}

std::vector<YYTK::CInstance*> Organik::Utils::findInstances(const std::string& objectName)
{
    int objectIndex = builtinAssetGetIndex(objectName);
    return Organik::Utils::findInstances(objectIndex);
}

std::vector<YYTK::CInstance*> Organik::Utils::findInstances(int objectIndex)
{
    std::vector<CInstance*> instances;
    int instance_count = builtinInstanceNumber(objectIndex);
    if (instance_count <= 0)
    {
        GetLogger()->LogFormatted("No instances of object #%d found.", objectIndex);
        return instances;
    }
    GetLogger()->LogFormatted("Found %d instances of object #%d", instance_count, objectIndex);
    instances.reserve(instance_count);
    for (int i=0; i< instance_count; ++i) 
    {
        CInstance* instance = builtinInstanceFind(objectIndex, i);
        if (instance == nullptr) 
        { // shouldn't be possible, according to GM docs. Maybe race condition?
          // TOCTOU, most likely. If the instance was deactivated after the call to instance_number.
            GetLogger()->LogFormatted("Error: Instance #%d of object #%d not found", i, objectIndex);
            continue;
        }
        instances.push_back(instance);
    }
    return instances;
}
CInstance *Organik::Utils::spawnObjectHere(const std::string &objectName, int x, int y)
{
    RValue mouseX = RValue(0);
    RValue mouseY = RValue(0);
    g_ModuleInterface->GetBuiltin("mouse_x", g_GlobalInstance, NULL_INDEX, mouseX);
    g_ModuleInterface->GetBuiltin("mouse_y", g_GlobalInstance, NULL_INDEX, mouseY);
    int layerID = g_ModuleInterface->CallBuiltin("layer_get_id", {RValue("Instances")}).ToInt32();
    int objectIndex = Organik::Utils::builtinAssetGetIndex(objectName);
    Organik::GetLogger()->LogFormatted("Spawning object #%d at (%d, %d) on layer %d", objectIndex, mouseX.ToInt32(), mouseY.ToInt32(), layerID);
    RValue newInstance = g_ModuleInterface->CallBuiltin("instance_create_layer", 
    {
        RValue(mouseX.ToInt32()), 
        RValue(mouseY.ToInt32()), 
        RValue(layerID),
        RValue(objectIndex),
        {}
    });
    if (newInstance.ToInt32() > 0)
    {
        Organik::Utils::knownInstanceIDs(newInstance.ToInt32());
        Organik::GetLogger()->LogFormatted("Spawned new instance with ID: %d", newInstance.ToInt32());
        return Organik::Utils::GetInstanceFromID(newInstance.ToInt32());
    }
    return nullptr;
}

std::vector<int> Organik::Utils::knownInstanceIDs(int newInstance)
{
    static std::vector<int> knownIDs;
    if (newInstance != -1) 
    {
        Organik::GetLogger()->LogFormatted("Adding new instance ID: %d", newInstance);
        if (std::find(knownIDs.begin(), knownIDs.end(), newInstance) == knownIDs.end()) 
        {
            knownIDs.push_back(newInstance);
        }
    }
    return knownIDs;
}
void Organik::Utils::builtinRoomGoto(const std::string &roomName)
{
    int roomIndex = Organik::Utils::builtinAssetGetIndex(roomName);
    auto args = std::vector<RValue>(
        {
            RValue(roomIndex)
        }
    );
    if (!Organik::Utils::getRoomGoto())
    {
        GetLogger()->LogFormatted("Couldn't resolve builtin: %s", __FUNCTION__);
        return;
    }
    RValue result;
    Organik::Utils::getRoomGoto()(result, Organik::Utils::GetGlobalInstance(), Organik::Utils::GetGlobalInstance(), static_cast<int>(args.size()), args.data());
}
// void Organik::Utils::roomGoto(const std::string &roomName)
// {
//     RValue roomNameR = RValue(roomName);
//     RValue roomIndex = g_ModuleInterface->CallBuiltin("asset_get_index", {roomNameR});
//     if (roomIndex.ToInt32() == -1) {
//         GetLogger()->LogFormatted("Error: Room '%s' not found", roomName);
//         return;
//     }
//     RValue newRoomID = roomIndex;
//     GetLogger()->LogFormatted("Found room #: %d", roomIndex.ToInt32());
//     if (!g_ModuleInterface->CallBuiltin("room_exists", {roomIndex}).ToBoolean()) {
//         GetLogger()->LogFormatted("Error: Room with index %s does not exist.", roomName.c_str());
//         return;
//     }
//     GetLogger()->LogFormatted("Going to room with index: %d", newRoomID.ToInt32());
//     g_ModuleInterface->CallBuiltin("room_goto", {newRoomID});
// }

std::vector<YYTK::CInstance*> Organik::Utils::getRoomInstances()
{
    RValue instances = g_ModuleInterface->CallBuiltin("layer_get_all_elements", {RValue("Instances")});
    std::vector<YYTK::RValue> instances_vect = instances.ToVector();

    std::vector<YYTK::CInstance*> filtered_instances;
    for (auto& instance : instances_vect) {
        if (CInstance::FromInstanceID(instance.ToInt32()) != nullptr) {
            filtered_instances.push_back(CInstance::FromInstanceID(instance.ToInt32()));
        }
    }
    return filtered_instances;
}

std::vector<std::string> Organik::Utils::getInstanceVariableNames(YYTK::CInstance* instance)
{
    if (!instance) {
        GetLogger()->LogSimple("Error: Instance is null");
        return {};
    }
    
    std::vector<RValue> variableNamesR = g_ModuleInterface->CallBuiltin("variable_instance_get_names", {RValue(instance->GetMembers().m_ID)}).ToVector();
    std::vector<std::string> variableNames;

    for (const auto& varName : variableNamesR) {
        variableNames.push_back(varName.ToString());
    }
    
    
    return variableNames;
}
YYTK::CInstance* Organik::Utils::GetInstanceFromID(int32_t id)
{
    // GetLogger()->LogFormatted("Getting instance from ID: %d", id);
    if (!YYGML_FindInstance) 
    {
        GetLogger()->LogSimple("Resolving YYGML_FindInstance");
        std::wstring game_name;
		Aurie::AurieStatus last_status = AurieStatus::AURIE_SUCCESS;
        last_status = MdGetImageFilename(
			g_ArInitialImage,
			game_name
		);
        YYGML_FindInstance = (fn_FromInstanceID) MmSigscanModule  // YYGML_FindInstance(int id)
        (
			game_name.c_str(),
			UTEXT("\x51\x8B\x54\x24\x00\x53\x55\x56"),
            "xxxx?xxx"
        );
        GetLogger()->LogFormatted("FindInstance = 0x%p", YYGML_FindInstance);
    }
    Organik::Utils::knownInstanceIDs(id);
    return YYGML_FindInstance(id);
}
CRoom* Organik::Utils::Room_Data(int RoomID)
{
    if (g_Rooms == nullptr || g_RoomItems < 1) {
        GetLogger()->LogSimple("Error: g_Rooms or g_RoomItems is null");
        return nullptr;
    }
    if (RoomID < 0 || RoomID >= g_RoomItems) {
        GetLogger()->LogFormatted("Error: Invalid RoomID %d", RoomID);
        return nullptr;
    }
    return (g_Rooms)[RoomID];
}

PVOID Organik::Utils::GetActivateFunction()
{
    if (!Organik::Utils::CInstance_Activate) 
    {
        GetLogger()->LogSimple("Resolving CInstance_Activate");
        std::wstring game_name;
		Aurie::AurieStatus last_status = AurieStatus::AURIE_SUCCESS;
        last_status = MdGetImageFilename(
			g_ArInitialImage,
			game_name
		);
        Organik::Utils::CInstance_Activate = (PVOID) MmSigscanModule  // YYGML_FindInstance(int id)
        (
			game_name.c_str(),
			UTEXT("\x56\x8B\xF1\xF7\x86\x00\x00\x00\x00\x00\x00\x08\x00\x75"),
            "xxxxx????xxxxx"
        );
        GetLogger()->LogFormatted("CInstance_Activate = 0x%p", Organik::Utils::CInstance_Activate);
    }
    return Organik::Utils::CInstance_Deactivate;
}
// HOOK_METHOD(YYTK::CInstance, Activate, () -> void)
// {
//     GetLogger()->LogFormatted("CInstance::Activate called on instance ID: %d", this->GetMembers().m_ID);
//     super();
// }
// HOOK_METHOD(YYTK::CInstance, Deactivate, () -> void)
// {
//     GetLogger()->LogFormatted("CInstance::Deactivate called on instance ID: %d", this->GetMembers().m_ID);
//     super();
// }
// HOOK_METHOD(YYTK::CInstance, SetDeactivated, (bool active) -> void)
// {
//     GetLogger()->LogFormatted("CInstance::SetDeactivated (%s) called on instance ID: %d", active ? "true" : "false", this->GetMembers().m_ID);
//     super(active);
// }
