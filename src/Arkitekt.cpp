
#define ZHL_LOGGING 1
#include "Synthetik.h"
#include "detours.h"
#include "SigScan.h"
#include <unordered_map>
#include <cstdio>
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <io.h> 
#include <cstdarg>
#include "PALMemoryProtection.h"
#include "Logging.h"
#include <inttypes.h>
#include <filesystem>
#include <Windows.h>
#include "Organik/UI/UIManager.h"
// #include <Arkitekt/Assembler.win32.hpp>
#include "Arkitekt.h"
#define OUR_OWN_FUNCTIONS_CALLEE_DOES_CLEANUP 1
#define PTR_PRINT_F "0x%08" PRIxPTR
#define POINTER_BYTES 4
// #define USE_STACK_ALIGNMENT 1
// #define STACK_ALIGNMENT_SIZE 0x04

using namespace Arkitekt;
using namespace Organik;
//#define HOOK_LOG

std::vector<FunctionHook*>& GetInstalledHooks()
{
	static std::vector<FunctionHook*> ret{};
	return ret;
};
std::shared_mutex& GetInstallMapMutex()
{
	static std::shared_mutex ret;
	return ret;
};

std::multimap<int, std::function<auto ()->FunctionHook*>*, std::greater<int>>& GetHookMap()
{
	static std::multimap<int, std::function<auto ()->FunctionHook*>*, std::greater<int>> ret{};
	return ret;
};
std::multimap<int, std::function<auto ()->FunctionHook*>*, std::greater<int>>& GetDelayedInstallMap()
{
	static std::multimap<int, std::function<auto ()->FunctionHook*>*, std::greater<int>> ret{};
	return ret;
};
static std::thread* installerThread;
std::thread* GetInstallerThread()
{
	if (installerThread)
		return installerThread;
	installerThread = new std::thread([&]()
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(100ms);
		while (true)
		{
			bool needToInstall = false;


			{
				std::shared_lock<std::shared_mutex> mapReadLock(GetInstallMapMutex());
				needToInstall = GetHookMap().size() > 0;
			}

			if (needToInstall)
			{
				std::unique_lock<std::shared_mutex> mapInstallLock(GetInstallMapMutex());
				for (auto [priority, installfn] : GetHookMap())
				{
					GetInstalledHooks().emplace_back((*installfn)());
				}
				GetHookMap().clear();
			}

			needToInstall = false;

			if (ImGui::GetCurrentContext())
			{
				{
					std::shared_lock<std::shared_mutex> mapReadLock(GetInstallMapMutex());
					needToInstall = GetDelayedInstallMap().size() > 0;
				}
				if (needToInstall)
				{
					std::unique_lock<std::shared_mutex> mapInstallLock(GetInstallMapMutex());
					for (auto [priority, installfn] : GetDelayedInstallMap())
					{
						GetInstalledHooks().emplace_back((*installfn)());
					}
					GetDelayedInstallMap().clear();
				}
			}
			std::this_thread::sleep_for(100ms);
		}
	});
	installerThread->detach();
	return installerThread;
};

bool Arkitekt::Init()
{
	static bool initialized = false;
	if (initialized) return initialized;
	initialized = true;

	if (!Definition::Init())
	{
		// TODO: Maybe change this over to SDL_ShowSimpleMessageBox for all systems; however, we'll have to add libsdl to the build & maybe the sdl.dll runtime DLL on Windows
		// SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", FunctionDefinition::GetLastError(), NULL);

		MessageBoxA(0, FunctionDefinition::GetLastError(), "Error", MB_ICONERROR);
		ExitProcess(1);
	}

	Organik::GetLogger()->InitLogging();
	GetInstallerThread();
	// auto* hookTest = CreateWrapper<bool(__cdecl*)(bool), true>("GR_D3D_Finish_Frame", 0x0327DEC0u, "SYNTHETIK.exe");
	// Log("===HOOK CREATED=== %p", (void*)hookTest);
	return initialized;

}


template <size_t Size>
static const char* ConvertToUniqueName(char(&dst)[Size], const char* name, const char* type)
{
	char tmp[128];
	strncpy(tmp, type, 128);

	const char* p = tmp;
	if (p[0] == '.')
	{
		++p;
		if (p[0] == 'P' && p[1] == '8')
		{
			p += 2;
			while (p[0] && (p[0] != '@' || p[1] != '@')) ++p;
		}
	}

	snprintf(dst, Size, "%s%s", name, p);
	return dst;
}

//================================================================================
// Definition

static std::vector<Definition*>& Defs()
{
	static std::vector<Definition*> defs;
	return defs;
}

std::unordered_map<size_t, Definition*>& DefsByHash()
{
	static std::unordered_map<size_t, Definition*> DefsByHash;
	return DefsByHash;
}

static char g_defLastError[1024] = { 0 };

const char* Definition::GetLastError() { return g_defLastError; }


int Definition::Init()
{
	SigScan::Init();

	for (auto it = Defs().begin(); it != Defs().end(); ++it)
	{
		if (!(*it)->Load()) return 0;
	}
	return 1;
}

void Definition::Add(const char* name, size_t typeHash, Definition* def)
{
	Defs().push_back(def);
	DefsByHash().insert(std::pair<size_t, Definition*>(CHashMapCalculateHash(name) ^ typeHash, def));
}



//================================================================================
// VariableDefinition


int VariableDefinition::Load()
{
	SigScan sig(_sig);
	if (!sig.Scan())
	{
		std::cerr << "Failed to find value for variable" << _name << "address could not be found" << std::endl;
		return 0;
	}

	if (sig.GetMatchCount() == 0)
	{
		std::cerr << "Failed to find address for variable " << _name << " no capture in input signature" << std::endl;
		return 0;
	}

	const SigScan::Match& m = sig.GetMatch();
	if (_useOffset)
	{
		/*
		 * Instruction Pointer relative addressing
		 * Determine real address of variable match
		 * RIP + var = real addr
		 * During execution (E|R)IP would be at the next instruction so instead we add the length of the match
		 * NOTE: This means it is only possible to match a variable with offset computation that is at the END of the instruction bytes
		 * i.e., operands that can take two memory addresses, only the second memory address (end of the bytes of the instruction) can be matched.
		 */
		if (!m.address || !m.length)
		{
			std::cerr << "VariableDefinition::Load(" << _name << "): m.address is NULL." << std::endl;
			return 0;
		}
		uintptr_t valueVar = 0;
		memcpy(&valueVar, m.address, m.length);
		uintptr_t realAddr = (uintptr_t)m.address;
		realAddr += m.length;
		realAddr += valueVar;
		if (!_outVar)
		{
			std::cerr << "VariableDefinition::Load(" << _name << "): _outVar is NULL." << std::endl;
			return 0;
		}
		if (!(*(void**)_outVar))
		{
			std::cerr << "VariableDefinition::Load(" << _name << "): _outVar resolves to nullptr." << std::endl;
			return 0;
		}
		*(void**)_outVar = (void*)realAddr;
	}
	else if (_useValue)
		memcpy(_outVar, m.address, m.length);
	else
		*(void**)_outVar = (void*)m.address;

	std::cout << "Found value for " << _name << ": @" << _outVar << ", dist " << std::to_string(sig.GetDistance());

	return 1;
}

//================================================================================
// NoOpDefinition


int NoOpDefinition::Load()
{
	SigScan sig(_sig);
	if (!sig.Scan())
	{
		std::cerr << "Failed to find match for no-op region " << _name << " address could not be found" << std::endl;
		return 0;
	}

	if (sig.GetMatchCount() == 0)
	{
		std::cerr <<"Failed to find match for no-op region " << _name << " no capture in input signature" << std::endl;
		return 0;
	}

	const SigScan::Match& m = sig.GetMatch();

	uintptr_t ptrToCode = (uintptr_t)m.address;
	const size_t noopingSize = sizeof(uint8_t) * m.length;
	MEMPROT_SAVE_PROT(dwOldProtect);
	MEMPROT_PAGESIZE();
	MEMPROT_UNPROTECT(ptrToCode, noopingSize, dwOldProtect);
	for (int i = 0; i < m.length; i++)
	{
		*(uint8_t*)(ptrToCode++) = 0x90;
	}
	MEMPROT_REPROTECT(ptrToCode, noopingSize, dwOldProtect);

	std::cerr << "Found address for " << _name << ": " << std::to_string((uintptr_t)m.address) << ", wrote NOP's for " << std::to_string(m.length) << "bytes, dist " << std::to_string(sig.GetDistance()) << std::endl;

	return 1;
}

//================================================================================
// FunctionDefinition

int FunctionDefinition::Load()
{
	static uintptr_t moduleBase=(uintptr_t)GetModuleHandleA(nullptr);
	if (_address)
	{
		std::cout << "Function " << _name << " has predefined address: " << std::hex << _address << std::dec << std::endl;
		*_outFunc = _address;
		return 1;
	}
	SigScan sig = SigScan(_sig);

	if (!sig.Scan())
	{
		std::cerr << "Failed to find address for function " << _name << std::endl;
		return 0;
	}

	_address = sig.GetAddress<void*>();

	*_outFunc = _address;
	std::cout << "Found address for " << _name.data()  << " @ " << std::hex << _address << " after " << std::dec << sig.GetDistance() << " bytes" << std::endl;

	return 1;
}
