#include <Windows.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <TlHelp32.h>
#include <functional>
#include <string>
#include <vector>
#include "ConsoleWindow.h"

HMODULE mod;
HMODULE hModule_Dupe;
HMODULE* modref;
void doLoadLibraryW()
{
	if (!mod)
	{
		ConsoleWindow::Init();
		std::cout << "Created thread with ID: " << std::to_string(std::this_thread::get_id()._Get_underlying_id()) << std::endl;

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
			GetModuleHandleEx(NULL, L"LogHelper.dll", &hModule_Dupe);
			
			std::thread t(doLoadLibraryW);
			t.detach();
		}
	}

	return TRUE;
}