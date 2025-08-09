#include "Synthetik.h"
#include "zhl_internal.h"
#include "Utils.h"
#include "Organik/UI/UIManager.h"
#include "windows.h"
#include "Logging.h"
#include "wininet.h"
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
    const std::string& playerName = "Unknown Player";
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
HOOK_GLOBAL(Error_Show_Action, (char* message, bool mustCrash, bool manualError) -> void)
{
    Organik::GetLogger()->LogFormatted("Error_Show_Action called with message: %s, mustCrash: %s, manualError: %s", message, mustCrash, manualError);
    std::string formattedMessage = GetLogger()->ParseFormatting("Oops! All Exceptions!\nMustCrash? %s\nManualError? %s\nMessage: %s",
        mustCrash ? "yes" : "no",
        manualError ? "yes" : "no",
        message
    );
    Organik::Utils::bugWebhook(
        formattedMessage
    );
    MessageBoxA(NULL, formattedMessage.c_str(), manualError ? "Organik Error" : "Synthetik Error", MB_ICONERROR | MB_OK);
    if (mustCrash)
    {
        Organik::GetLogger()->Cleanup();
        Organik::UIManager::GetInstance()->Shutdown();
        ExitProcess(1);
    }
    super(message, mustCrash, manualError);
}