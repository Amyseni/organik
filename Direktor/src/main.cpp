#ifndef _WIN32
#define _WIN32
#endif
#ifndef WIN32
#define WIN32
#endif
#ifndef __i386__
#define __i386__
#endif
#ifdef _AMD64_
#undef _AMD64_
#endif
#ifdef __i686__
#undef __i686__
#endif
#ifdef WOW64
#undef WOW64
#endif

#include "Synthetik.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "zhl_private.h"
#include "ConsoleWindow.h"

using namespace Organik;
using namespace ZHL;

HINSTANCE hModule_Dupe = nullptr;
HMODULE D3D11Dll = nullptr;
ID3D11Device** gD3DDevice;
ID3D11DeviceContext** gD3DContext;

// HRESULT __stdcall GamerGraphicsHook(IDXGIAdapter* _adapter, D3D_DRIVER_TYPE _driverType, HMODULE _software, UINT _flags, const D3D_FEATURE_LEVEL* p_featureLevels, UINT _featureLevels, UINT _sdkVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL * pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
// {
//     // HRESULT realRet = ((PFN_D3D11_CREATE_DEVICE)d3d11CreateDevice_Super)(_adapter, _driverType, _software, _flags, p_featureLevels, _featureLevels, _sdkVersion, ppDevice, pFeatureLevel, ppImmediateContext);
//     // if (SUCCEEDED(realRet))
//     // {
//     //     std::cout << "D3D11 device created successfully." << std::endl;
//     //     Organik::Utils::globalD3DDevice = *ppDevice;
//     //     Organik::Utils::globalD3DContext = *ppImmediateContext;
//     // }
//     // else
//     // {
//     //     std::cerr << "Failed to create D3D11 device: " << std::hex << realRet << std::dec << std::endl;
//     // }
//     return ((PFN_D3D11_CREATE_DEVICE)d3d11CreateDevice_Super)(_adapter, _driverType, _software, _flags, p_featureLevels, _featureLevels, _sdkVersion, ppDevice, pFeatureLevel, ppImmediateContext);
// }

/*
 *#Define CreateDevice's original function type/calling convention and tell ZHL how to hook it (basically)
 * argdata = an array describing the arguments, their types, and what registers they use (if any, but for __stdcall, they are all on the stack)
 * InternalSuper = the original function pointer to an unmodified D3D11CreateDevice function.
 * Hook = the trampoline funtction that gets called instead of the original, with any number of other hooks installed into it.
 * Super = a reference to the original function pointer set up in a way that will be easy to call from the hook
 * hookObj = the FunctionHook object that manages the hook and its installation.
 * __declspec(naked) super = naked ASM jump to the original function pointer for the hook 
 * 
 * and then do it all again for CreateDXGIFactory1 
*/
static short d3d11CreateDevice_argdata[] = {0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};

static void *d3d11CreateDevice_InternalSuper = 0; 
static auto __stdcall d3d11CreateDevice_Hook (IDXGIAdapter* _adapter, D3D_DRIVER_TYPE _driverType, HMODULE _software, UINT _flags, const D3D_FEATURE_LEVEL* p_featureLevels, UINT _featureLevels, UINT _sdkVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL * pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) -> HRESULT;
static auto __stdcall d3d11CreateDevice_Super (IDXGIAdapter* _adapter, D3D_DRIVER_TYPE _driverType, HMODULE _software, UINT _flags, const D3D_FEATURE_LEVEL* p_featureLevels, UINT _featureLevels, UINT _sdkVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL * pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) -> HRESULT;
static FunctionHook_private hookObj("D3D11CreateDevice", typeid(auto (*) (IDXGIAdapter* _adapter, D3D_DRIVER_TYPE _driverType, HMODULE _software, UINT _flags, const D3D_FEATURE_LEVEL* p_featureLevels, UINT _featureLevels, UINT _sdkVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL * pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) -> HRESULT), &d3d11CreateDevice_Hook, &d3d11CreateDevice_InternalSuper, 0); 
auto __declspec(naked) __stdcall d3d11CreateDevice_Super (IDXGIAdapter* _adapter, D3D_DRIVER_TYPE _driverType, HMODULE _software, UINT _flags, const D3D_FEATURE_LEVEL* p_featureLevels, UINT _featureLevels, UINT _sdkVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL * pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) -> HRESULT
{
    __asm {
        JUMP_INSTRUCTION(d3d11CreateDevice_InternalSuper)
    }; 
}

auto __stdcall d3d11CreateDevice_Hook (IDXGIAdapter* _adapter, D3D_DRIVER_TYPE _driverType, HMODULE _software, UINT _flags, const D3D_FEATURE_LEVEL* p_featureLevels, UINT _featureLevels, UINT _sdkVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL * pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) -> HRESULT
{
_flags |= D3D11_CREATE_DEVICE_DEBUG;
    
    HRESULT realRet = ((PFN_D3D11_CREATE_DEVICE)d3d11CreateDevice_Super)(_adapter, _driverType, _software, _flags, p_featureLevels, _featureLevels, _sdkVersion, ppDevice, pFeatureLevel, ppImmediateContext);
    if (SUCCEEDED(realRet))
    {
        std::cout << "D3D11 device created successfully." << std::endl 
                    << "ppDevice:" << std::hex << ppDevice << ": " << *ppDevice << std::endl
                    << "ppDeviceContext:" << std::hex << ppImmediateContext << ": " << *ppImmediateContext << std::dec << std::endl;
        
        
        std::cout << "Global D3D device and context set." << std::hex << Organik::Utils::g_D3DDevice << "," << Organik::Utils::g_D3DContext << std::dec << std::endl;
    }
    else
    {
        std::cerr << "Failed to create D3D11 device: " << std::hex << realRet << std::dec << std::endl;
    }
    return realRet;
}

static void *org_CreateDeviceOut = nullptr;
static auto fnDef = FunctionDefinition("D3D11CreateDevice", typeid(PFN_D3D11_CREATE_DEVICE), (PFN_D3D11_CREATE_DEVICE)0x03865822, d3d11CreateDevice_argdata, 10, 0, &org_CreateDeviceOut);
void gamerD3DetourThread()
{
    while (!(D3D11Dll = GetModuleHandleA("d3d11.dll")))
    {
        std::cout << "Waiting for d3d11.dll to be loaded..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(42));
    }

    std::cout << "d3d11.dll loaded: " << std::hex << D3D11Dll << std::dec << std::endl;
    fnDef.Load();
    std::cout << "Function definition loaded: "<< std::hex << (fnDef.GetAddress() ? fnDef.GetAddress() : (void*)0xDEADBEEF) << std::dec << std::endl;
    hookObj.Install();
    std::cout << "Gamer D3D11 hook installed @ " << std::hex << hookObj._hook << std::dec << std::endl;  
    std::cout << "GetProcAddress says the function is at." << std::hex << GetProcAddress(GetModuleHandleA("d3d11.dll"), "D3D11CreateDevice") << std::dec << std::endl;

    std::thread t(ZHL::Init);
    t.detach();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if (hModule_Dupe == nullptr)
		{
			// Prevents the module from being unloaded by the game (or, tries to anyways)
            GetModuleHandleEx(NULL, L"Direktor.dll", &hModule_Dupe);
            std::cout << "Direktor DLL loaded: " << std::to_string((unsigned int) hModule_Dupe) << std::endl;
            
            // gamer multithreading. way too early to initialize imgui
			std::thread t(gamerD3DetourThread);
            ConsoleWindow::Init();
			std::cout << "Created thread with ID: " << t.get_id() << std::endl;
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