#pragma once
#include "Windows.h"
#include "Synthetik.h"

HINSTANCE hModule_Dupe = nullptr;
PFN_D3D11_CREATE_DEVICE fnd3d11CreateDevice = nullptr;
HMODULE D3D11Dll = nullptr;
void* p_D3D11CreateOrg = nullptr;
void* p_D3D11CreateInternalSuper = nullptr;


void gamerD3DetourThread();
__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);