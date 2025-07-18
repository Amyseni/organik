#include "Utils.h"
#include <string>
#include <vector>
#include <sstream>
#include "ModuleMain.h"
#include "UI/UIManager.h"   
#include "UI/OrganikConsole.h"
#include "Logging.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include <cctype>
#include <iomanip>
#include "windows.h"
#include "wininet.h"
#include "InstanceHelper.h"
#include "VariableHelper.h"
#pragma comment(lib, "wininet.lib")

using namespace YYTK;
using namespace Aurie;
using namespace Organik;
namespace Organik {
    struct OrganikConsole;
}
static YYTK::CInstance* g_GlobalInstance = nullptr;

YYTK::CInstance* Organik::Utils::GetGlobalInstance()
{
    if (g_GlobalInstance == nullptr)
    {
        Aurie::AurieStatus last_status = g_ModuleInterface->GetGlobalInstance(&g_GlobalInstance);
        if (last_status != Aurie::AURIE_SUCCESS || !g_GlobalInstance)
        {
            GetLogger()->TryLogConsole("Error: Global instance not found");
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
        GetLogger()->TryLogConsole("Couldn't resolve builtin: %s", __FUNCTION__);
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
        GetLogger()->TryLogConsole("Couldn't resolve builtin: %s", __FUNCTION__);
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
        GetLogger()->TryLogConsole("Couldn't resolve builtin: %s", __FUNCTION__);
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
CInstance* Organik::Utils::FirstInstanceOrNullptr(int objectIndex)
{
    int instance_count = builtinInstanceNumber(objectIndex);
    if (instance_count > 0)
        return builtinInstanceFind(objectIndex, 0);
    return nullptr;
}
std::vector<YYTK::CInstance*> Organik::Utils::findInstances(int objectIndex)
{
    std::vector<CInstance*> instances;
    int instance_count = builtinInstanceNumber(objectIndex);
    if (instance_count <= 0)
    {
        return instances;
    }
    instances.reserve(instance_count);
    for (int i=0; i< instance_count; ++i) 
    {
        CInstance* instance = builtinInstanceFind(objectIndex, i);
        if (instance == nullptr) 
        {
            GetLogger()->TryLogConsole("Error: Instance #%d of object #%d not found", i, objectIndex);
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
    GetLogger()->TryLogConsole("Spawning object #%d at (%d, %d) on layer %d", objectIndex, mouseX.ToInt32(), mouseY.ToInt32(), layerID);
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
        GetLogger()->TryLogConsole("Spawned new instance with ID: %d", newInstance.ToInt32());
        return Organik::Utils::GetInstanceFromID(newInstance.ToInt32());
    }
    return nullptr;
}

std::vector<int> Organik::Utils::knownInstanceIDs(int newInstance)
{
    static std::vector<int> knownIDs;
    if (newInstance != -1) 
    {
        if (std::find(knownIDs.begin(), knownIDs.end(), newInstance) == knownIDs.end()) 
        {
            GetLogger()->TryLogConsole("Adding new instance ID: %d", newInstance);
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
        GetLogger()->TryLogConsole("Couldn't resolve builtin: %s", __FUNCTION__);
        return;
    }
    RValue result;
    Organik::Utils::getRoomGoto()(result, Organik::Utils::GetGlobalInstance(), Organik::Utils::GetGlobalInstance(), static_cast<int>(args.size()), args.data());
}

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

// https://stackoverflow.com/a/17708801
std::string Organik::Utils::url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

std::string Organik::Utils::SendHTTPSPost(
    const std::string& _server, 
    const std::string& _page, 
    const std::string& _params,
    const std::string& _body)
{
    std::string responseStr;
    
    // initialize WinInet
    HINTERNET hInternet = ::InternetOpen(TEXT("Organik HTTP Client"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet != NULL) 
    {
        // open HTTPS session
        HINTERNET hConnect = ::InternetConnectA(hInternet, _server.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect != NULL) 
        {
            std::string request = _page + 
                    (_params.empty() ? "" : ("?" + _params));

            // open request with HTTPS flags
            HINTERNET hRequest = ::HttpOpenRequestA(hConnect, "POST", request.c_str(), NULL, NULL, NULL, 
                INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0); 

            if (hRequest != NULL) 
            {   
                // Set headers for form data
                std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
                
                // send request
                BOOL isSent = ::HttpSendRequestA(hRequest, headers.c_str(), headers.length(), 
                    const_cast<char*>(_body.c_str()), _body.length()); // Fix 3: Don't subtract 1 from length
                
                if (isSent)
                {
                    // Check HTTP status code first
                    char statusCode[10];
                    DWORD statusCodeSize = sizeof(statusCode);
                    DWORD index = 0;
                    
                    if (HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE, statusCode, &statusCodeSize, &index))
                    {
                        GetLogger()->LogFormatted("HTTP Status: %s", statusCode);
                    }
                    
                    // Read response in chunks and accumulate
                    char szData[1024];
                    DWORD dwByteRead;
                    
                    while (true)
                    {
                        BOOL isRead = ::InternetReadFile(hRequest, szData, sizeof(szData) - 1, &dwByteRead);
                        
                        if (isRead == FALSE || dwByteRead == 0)
                            break;

                        szData[dwByteRead] = '\0';
                        responseStr += std::string(szData, dwByteRead);
                    }
                    GetLogger()->LogFormatted("Response length: %d", responseStr.length());
                }
                else
                {
                    DWORD error = GetLastError();
                    GetLogger()->LogFormatted("HttpSendRequestA failed with error: %d", error);
                }
                
                ::InternetCloseHandle(hRequest);
            }
            ::InternetCloseHandle(hConnect);
        }
        ::InternetCloseHandle(hInternet);
    }
    
    return responseStr;
}
void Organik::Utils::bugWebhook(const std::string& errorMsg)
{
    const std::string& playerName = Utils::GetPlayerName();
    const std::string& formattedErrorMsg = GetLogger()->ParseFormatting("Bug Report - %s\n\n%s", playerName.c_str(), errorMsg.c_str());
    GetLogger()->LogFormatted("Report Message: %s -> %s", errorMsg.c_str(), formattedErrorMsg.c_str());
    const std::string& body = "content=" + Utils::url_encode(formattedErrorMsg);
    GetLogger()->LogFormatted("Encoded: %s", errorMsg.c_str(), formattedErrorMsg.c_str());
    std::string response = Utils::SendHTTPSPost(
        "discord.com",
        "/api/webhooks/1393620194072465459/qcn1RTy4yJTlhP0vCvj6ufCOrzdH1Z3EGKOLaRpnNyJtyyFRGmoL0R7W7gBGZK0dKQLd",
        "thread_id=1393617657944608838",
        body
    );

    GetLogger()->LogFormatted("Response: %s", response.c_str());
}
std::vector<std::string> Organik::Utils::getInstanceVariableNames(YYTK::CInstance* instance)
{
    if (!instance) {
        GetLogger()->TryLogConsole("Error: Instance is null in function: %s", __FUNCTION__);
        return {};
    }
    
    std::vector<RValue> variableNamesR = g_ModuleInterface->CallBuiltin("variable_instance_get_names", {RValue(instance->m_ID)}).ToVector();
    std::vector<std::string> variableNames;

    for (const auto& varName : variableNamesR) {
        variableNames.push_back(varName.ToString());
    }
    
    
    return variableNames;
}
YYTK::CInstance* Organik::Utils::GetInstanceFromID(int32_t id)
{
    if (!YYGML_FindInstance) 
    {
        GetLogger()->TryLogConsole("Resolving YYGML_FindInstance");
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
        
        GetLogger()->TryLogConsole("FindInstance = 0x%p", YYGML_FindInstance);
    }
    Organik::Utils::knownInstanceIDs(id);
    return YYGML_FindInstance(id);
}
CRoom* Organik::Utils::Room_Data(int RoomID)
{
    
    if (getRooms() == nullptr || getRoomItems() == nullptr) {
        GetLogger()->TryLogConsole("Error: g_Rooms or g_RoomItems is null");
        return nullptr;
    }
    if (RoomID < 0 || RoomID >= *getRoomItems()) {
        GetLogger()->TryLogConsole("Error: Invalid RoomID %d", RoomID);
        return nullptr;
    }
    return (*getRooms())[RoomID];
}

PVOID Organik::Utils::GetActivateFunction()
{
    if (!Organik::Utils::CInstance_Activate) 
    {
        GetLogger()->TryLogConsole("Resolving CInstance_Activate");
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
        
        GetLogger()->TryLogConsole("CInstance_Activate = 0x%p", Organik::Utils::CInstance_Activate);
    }
    return Organik::Utils::CInstance_Activate;
}
std::string Organik::Utils::GetPlayerName()
{
    std::vector<CInstance*> instanceVect = Organik::Utils::findInstances(Organik::Utils::getOutgameIndex());

    if (!instanceVect.empty())
    {
        CInstance* outgameInstance = instanceVect[0];
        if (outgameInstance->ToRValue().ContainsValue("o_playername"))
        {
            RValue* g_oPlayerName = outgameInstance->GetRefMember("o_playername");
            if (g_oPlayerName && g_oPlayerName->ToString().length())
            {
                return g_oPlayerName->ToString();
            }
            else
            {
                GetLogger()->TryLogConsole("Warning: Player name is empty or not set.");
                return "Unknown Player";
            }
        }
    }
    return "Unknown Player";
}