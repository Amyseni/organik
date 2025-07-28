

#include <Windows.h>
#include <iostream>
#include <string>
#include "zhl.h"
#include "zhl_internal.h"
#include <windef.h>
#include "Logging.h"
#include <thread>
#include <fstream>
#include <TlHelp32.h>
#include <functional>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

using namespace Organik;
using namespace ZHL;

HINSTANCE hModule_Dupe = nullptr;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if(ul_reason_for_call == DLL_THREAD_ATTACH)
	{
		if (hModule_Dupe == nullptr)
		{
			// Prevents the module from being unloaded by the game
            GetModuleHandleEx(NULL, L"Direktor.dll", &hModule_Dupe);
            std::cout << "Direktor DLL loaded: " << std::to_string((unsigned int) hModule_Dupe) << std::endl;

			std::thread t(ZHL::Init);
			std::cout << "Created thread with ID: " << std::to_string(t.get_id()._Get_underlying_id()) << std::endl;
			t.detach();
		}
	}
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        if (hModule_Dupe != nullptr)
        {
            std::cout << "Direktor DLL unloaded: " << std::to_string((unsigned int) hModule_Dupe) << std::endl;
            hModule_Dupe = nullptr;
        }
        
        // Perform any necessary cleanup here
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
    return TRUE;
}