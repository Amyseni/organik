#include "Synthetik.h"
#include "zhl_internal.h"
#include "Organik/UI/UIManager.h"
#include "windows.h"

#include "wininet.h"
#include "DefinitionHelpers/VariableHelper.h"
#pragma comment(lib, "wininet.lib")
using namespace Organik;
bool Organik::Utils::Settings::g_EnableMultiplayerCompat = false;
bool Organik::Utils::Settings::g_EnableInvincibility = false;

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

std::mt19937 gen( std::chrono::system_clock::now().time_since_epoch().count() ); // Standard mersenne_twister_engine
std::mt19937* Organik::Utils::getrng()
{
    return &gen;
} 
void Organik::Utils::GetCurrentRoom(CRoom*& room)
{
    if (Run_Room && *Run_Room)
    {
        room = *Run_Room;
    }
    else
    {
        room = nullptr;
    }
}

std::mutex g_SettingsMutex;
std::pair<std::unique_lock<std::mutex>, Utils::Settings::GUISettings*&> Utils::Settings::GetUISettings()
{
    std::unique_lock<std::mutex> lock(g_SettingsMutex);
    GUISettings *ret = nullptr;
    if (!(ret = g_UISettings)){
        ret = (
            UIManager::isImGuiInitialized() && ImGui::GetIO().UserData
        ) ? static_cast<GUISettings*>(ImGui::GetIO().UserData) 
          : new GUISettings();
        g_UISettings = ret;
    }
    return std::pair<std::unique_lock<std::mutex>, Utils::Settings::GUISettings*&>(std::move(lock), std::forward<Utils::Settings::GUISettings*&>(g_UISettings));

}

void Utils::GetPlayerName(char outName[NAME_CHAR_LIMIT])
{
    if (outName == nullptr)
    {
        Error_Show_Action("Player name buffer is null", true, true);
        return; // line above terminates the process, so this return is just for clarity
    }
    
    memset(outName, 0, NAME_CHAR_LIMIT);
    auto copyPNameSafe = [&outName](const char* name) -> void {
        if (!name) {
            Error_Show_Action("Player name buffer is null", true, true);
            return; // line above terminates the process, so this return is just for clarity
        }
        const char* ptr = &name[0];
        int index = 0;
        do {
            outName[index++] = *ptr;
        } while (*ptr && index < NAME_CHAR_LIMIT && ++ptr);
        DO_LIMIT_NAME(outName);
    };
    CInstance *global;
    if (!(global = GetGlobalInstance())) {
        copyPNameSafe(DEFAULT_NAME);
        return;
    }
}
int Organik::UIElement::createCount = 0;
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
                    const_cast<char*>(_body.c_str()), _body.length());
                
                if (isSent)
                {
                    char statusCode[10];
                    DWORD statusCodeSize = sizeof(statusCode);
                    DWORD index = 0;
                    
                    if (HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE, statusCode, &statusCodeSize, &index))
                    {
//                         GetLogger()->LogFormatted("HTTP Status: %s", statusCode);
                    }
                    
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
//                     GetLogger()->LogFormatted("Response length: %d", responseStr.length());
                }
                else
                {
                    DWORD error = GetLastError();
//                     GetLogger()->LogFormatted("HttpSendRequestA failed with error: %d", error);
                }
                
                ::InternetCloseHandle(hRequest);
            }
            ::InternetCloseHandle(hConnect);
        }
        ::InternetCloseHandle(hInternet);
    }
    
    return responseStr;
}
CInstance* getOutgame() {
    static CInstance* outgame = nullptr;
    if (!outgame) {
        outgame = CInstance::FirstOrDefault([](CInstance* inst) -> bool {
            return inst && inst->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::outgame];
        });
    }
    return outgame;
}
void Organik::Utils::bugWebhook(const std::string& errorMsg)
{
    const std::string& playerName = "Unknown Player";
    if (UIManager::isImGuiInitialized()) //this means engine init has finished, safe to look at outgame now
    {
        
    }
    const std::string& formattedErrorMsg = GetLogger()->ParseFormatting("Bug Report - %s\n\n%s", playerName.c_str(), errorMsg.c_str());
    std::cerr << GetLogger()->ParseFormatting("Report Message: %s -> %s", errorMsg.c_str(), formattedErrorMsg.c_str());
    const std::string& body = "content=" + Utils::url_encode(formattedErrorMsg);
    std::cerr << GetLogger()->ParseFormatting("Encoded: %s", body.c_str());
    std::string response = Utils::SendHTTPSPost(
        "discord.com",
        "/api/webhooks/1393620194072465459/qcn1RTy4yJTlhP0vCvj6ufCOrzdH1Z3EGKOLaRpnNyJtyyFRGmoL0R7W7gBGZK0dKQLd",
        "thread_id=1393617657944608838",
        body
    );

    std::cerr << GetLogger()->ParseFormatting("Response: %s", response.c_str());
}
#define DO_WEBHOOK false
HOOK_GLOBAL(Error_Show_Action, (char* message, bool mustCrash, bool manualError) -> void)
{
    std::string formattedMessage = GetLogger()->ParseFormatting("Oops! All Exceptions!\nMustCrash? %s\nManualError? %s\nMessage: %s",
        mustCrash ? "yes" : "no",
        manualError ? "yes" : "no",
        message
    );
    if ((*Settings::GetEnableBugWebhook())) { 
        Organik::Utils::bugWebhook(
            formattedMessage
        );
    }
    MessageBoxA(NULL, formattedMessage.c_str(), manualError ? "Organik Error" : "Synthetik Error", MB_ICONERROR | MB_OK);
    if (mustCrash)
    {
        Organik::GetLogger()->Cleanup();
        Organik::UIManager::GetInstance()->Shutdown();
        ExitProcess(1);
    }
    super(message, mustCrash, manualError);
}
void Error_Show_Action(bool mustCrash, bool manualError, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    constexpr size_t max_length = 4096;
    char buffer[max_length] = { 0 };
    size_t length = strnlen_s(fmt, max_length);

    if (length >= max_length)
        strncpy_s(buffer, "<string too long to print>", max_length);
    else
        vsprintf_s(buffer, fmt, args);

    va_end(args);
    Error_Show_Action(&buffer[0], mustCrash, manualError);
}
bool Utils::isInitializationDone()
{
    return ImGui::GetCurrentContext() != nullptr;
}