#include <Windows.h>
#include "ConsoleWindow.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <TlHelp32.h>
#include <functional>
#include <string>
#include <vector>

HMODULE mod;
HMODULE hModule_Dupe;
HMODULE* modref;
void doLoadLibraryW()
{
	if (!mod)
	{
		ConsoleWindow::Init();
		mod = LoadLibraryW(L"Direktor.dll");
	}
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if (hModule_Dupe == nullptr)
		{
			// Prevents the module from being unloaded by the game
			GetModuleHandleEx(NULL, L"dbghelp.dll", &hModule_Dupe);
			
			std::thread t(doLoadLibraryW);
			std::cout << "Created thread with ID: " << std::to_string(t.get_id()._Get_underlying_id()) << std::endl;
			t.detach();
		}
	}

	return TRUE;
}