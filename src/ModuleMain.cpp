#define _CRT_SECURE_NO_WARNINGS
#include <Aurie/shared.hpp>
#include "ModuleMain.h"
#include "CodeEvents.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include "FunctionWrapper/FunctionWrapper.hpp"
#include "CallbackManager/CallbackManagerInterface.h"
#include "UI/UIManager.h"
#include "UI/BuiltinCommands.h"
#include "Logging.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "dsound.h"
#include "imgui/imgui_impl_dx11.h"
#include "Utils.h"

using namespace Aurie;
using namespace YYTK;
using namespace Organik;
#ifndef ORGANIK_VARIABLES
#define ORGANIK_VARIABLES
namespace Organik
{
    struct Variables
    {
        static std::unordered_map<const char*, int> varToEnumMap;
        static std::vector<int32_t> Hashes;
        static const char* const VariableNamesArr[];
        static std::unordered_map<int32_t, const char*> HashToVariableMap;
        static int32_t GetHash(const char* name);
	};
}
#endif
#ifndef ORGANIK_OBJECTS
#define ORGANIK_OBJECTS
namespace Organik
{
    struct Objects 
    {
        static const char* const ObjectNamesArr[];
        static std::vector<int32_t> ObjIndexes;
    };
}
#endif
#ifndef ORGANIK_ROOMS
#define ORGANIK_ROOMS
namespace Organik
{
    struct Rooms
    {
        static std::vector<int32_t> AssetIndexes;
        static const char* const RoomNamesArr[];
    };
}
#endif
#ifndef ORGANIK_SCRIPTS
#define ORGANIK_SCRIPTS
namespace Organik
{
    struct Scripts
    {
        static std::vector<int32_t> Indexes;
        static std::unordered_map<int32_t, CScript*> ScriptPointers;
        static const char* const ScriptNamesArr[];
    };
}
#endif

Organik::Logger* g_LoggerInstance;
YYTKInterface* g_ModuleInterface = nullptr;
CallbackManagerInterface* callbackManagerInterfacePtr = nullptr;
YYRunnerInterface g_RunnerInterface;
CInstance **g_globalInstance;
HWND hWnd;
ID3D11Device* d3d11Device = nullptr;
ID3D11DeviceContext* d3d11Context = nullptr;
HINSTANCE g_directInputInstance = nullptr;
CInstance* globalInstance = nullptr;


int frameCounter = 0;
bool debug = false;
typedef void (*fnCallback)(CodeEventArgs&);
RValue& emptyRVal() {
	static std::string emptyStr("");
	static RValue emptyRValInstance;
	emptyRValInstance = RValue(std::string(""));
	return emptyRValInstance;
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void WndProcCallback(FWWndProc& WndProcContext)
{
    auto args = WndProcContext.Arguments();
    HWND hWnd = std::get<0>(args);
    UINT msg = std::get<1>(args);
    WPARAM wParam = std::get<2>(args);
    LPARAM lParam = std::get<3>(args);
    
    // Let ImGui handle the input first
    LRESULT imgui_result = ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    
    // Check if ImGui wants to capture this input
    ImGuiIO& io = ImGui::GetIO();
    bool imgui_wants_input = false;
    
    switch (msg) {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            imgui_wants_input = io.WantCaptureKeyboard;
            break;
        case WM_LBUTTONDOWN:
			
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEMOVE:
			if (ImGui::IsAnyItemHovered() || UIManager::GetInstance()->isAnyItemHovered())
			{
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
				   ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange;
				imgui_wants_input = true;
				ImGui::GetIO().MouseDrawCursor = true;
			}
			else 
			{
				if (!(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
					ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange;
				ImGui::GetIO().MouseDrawCursor = false;
				imgui_wants_input = io.WantCaptureMouse;
			}
            break;
    }
    
    if (imgui_wants_input || imgui_result) {
        // ImGui handled it, don't pass to game
        WndProcContext.Override(true);
        return;
    }
    
    // Let the game handle it
    WndProcContext.Call();
}

void __cdecl HkYYError(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::string errorMsg = Organik::GetLogger()->ParseFormatting(fmt, args);
	Utils::bugWebhook(errorMsg);
	va_end(args);
}

void FrameCallback(FWFrame& FrameContext)
{
    frameCounter++;
    // Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
    
    // Add ImGui frame management here BEFORE calling the original frame
    // ImGui_ImplDX11_NewFrame();
    // ImGui_ImplWin32_NewFrame();
    // ImGui::NewFrame();
    

	// // Your UI rendering code here
	// Organik::UIManager::GetConsole()->Draw("Organik Console", &Organik::UIManager::GetInstance()->showConsole);
    
    // // Render ImGui
    // ImGui::Render();
    // ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    FrameContext.Call();
    
}
struct CallbackInfo {
	char *functionName;
	fnCallback *callbackBefore;
	fnCallback *callbackAfter;
	CallbackInfo(char *_fnName, fnCallback *_before, fnCallback *_after) : functionName(_fnName), callbackBefore(_before), callbackAfter(_after) 
	{	
	}
};
template <typename T>
T GetHookTrampoline(const char* Name)
{
	return reinterpret_cast<T>(MmGetHookTrampoline(g_ArSelfModule, Name));
}
EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	AurieStatus last_status = AURIE_SUCCESS;
	bool loging_initialized = Logger::InitLogging();
	if (!loging_initialized)
	{
		GetLogger()->LogFormatted("%s %d", __FUNCTION__, __LINE__);
		std::cerr << "Failed to initialize logging. Please check your logging configuration." << std::endl;
		return AURIE_MODULE_INITIALIZATION_FAILED;
	}
	// Gets a handle to the interface exposed by YYTK
	// You can keep this pointer for future use, as it will not change unless YYTK is unloaded.
	last_status = ObGetInterface(
		"YYTK_Main",
		(AurieInterfaceBase*&)(g_ModuleInterface)
	);

	// If we can't get the interface, we fail loading.
	if (!AurieSuccess(last_status))
	{
		GetLogger()->LogSimple("Failed to get g_ModuleInterface. How did this happen?");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}
	else
	{
		GetLogger()->LogSimple("YYTK_Main found!");
	}
	
	last_status = ObGetInterface("callbackManager", (AurieInterfaceBase*&)callbackManagerInterfacePtr);
	if (!AurieSuccess(last_status))
	{
		printf("Failed to get callback manager interface. Make sure that CallbackManagerMod is located in the mods/Aurie directory.\n");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}
	
	CInstance *g_globalInstance = nullptr;
	last_status = g_ModuleInterface->GetGlobalInstance(&g_globalInstance);
	if (!AurieSuccess(last_status))
	{
		GetLogger()->LogSimple("Failed to get global instance.");
		return AURIE_OBJECT_NOT_FOUND;
	}
	else
	{
		GetLogger()->LogSimple("Global instance found!");
	}

	RValue os_info_dsmap = g_ModuleInterface->CallBuiltin("os_get_info", {});
	int os_info_id = os_info_dsmap.ToInt32();
	GetLogger()->LogFormatted("os_get_info returned a ds_map with ID %s", std::to_string(os_info_id).c_str());
	RValue d3d_device = g_ModuleInterface->CallBuiltin("ds_map_find_value", {RValue(os_info_id), RValue("video_d3d11_device")});
	GetLogger()->LogSimple(d3d_device.GetKindName().c_str());
	GetLogger()->LogFormatted("ds_map_find_value returned a value with ID %p", d3d_device.ToPointer());
	RValue d3d_context = g_ModuleInterface->CallBuiltin("ds_map_find_value", {RValue(os_info_id), RValue("video_d3d11_context")});
	GetLogger()->LogFormatted("ds_map_find_value returned a value with ID %p", d3d_context.ToPointer());
	HWND window_handle = FindWindowA(nullptr, "Synthetik");
	GetLogger()->LogFormatted("windows_handle returned a value with ID %p", window_handle);	
	hWnd =	window_handle;
	d3d11Device = (ID3D11Device*) d3d_device.ToPointer();
	d3d11Context = (ID3D11DeviceContext*) d3d_context.ToPointer();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(d3d11Device, d3d11Context);

	last_status = g_ModuleInterface->CreateCallback(
		Module,
		EVENT_FRAME,
		FrameCallback,
		0
	);
	if (!AurieSuccess(last_status))
	{
		g_ModuleInterface->Print(CM_RED, "Failed to register frame callback");
	}
	last_status = g_ModuleInterface->CreateCallback(
		Module,
		EVENT_WNDPROC,
		WndProcCallback,
		0
	);
	if (!AurieSuccess(last_status))
	{
		g_ModuleInterface->Print(CM_RED, "Failed to register wndproc callback");
	}
	GetLogger()->LogFormatted("outgameIndex: %d, obj_statsIndex: %d, obj_playerIndex: %d",
					Organik::Utils::getOutgameIndex(), Organik::Utils::getObj_StatsIndex(), Organik::Utils::getObj_PlayerIndex());
	std::map<const char *, std::vector<fnCallback>> callbacks
	{
		{ "gml_Object_obj_stats_Create_0", std::vector<fnCallback>{&Organik::gml_Object_obj_stats_Create_0_Before, &Organik::gml_Object_obj_stats_Create_0_After}},
		{ "gml_Object_obj_player_Create_0", std::vector<fnCallback>{&Organik::gml_Object_obj_player_Create_0_Before, &Organik::gml_Object_obj_player_Create_0_After}},
		{ "gml_Object_obj_research_button_Create_0", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_Create_0_Before, &Organik::gml_Object_obj_research_button_Create_0_After}},
		{ "gml_Object_obj_research_button_Mouse_11", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_Mouse_11_Before, &Organik::gml_Object_obj_research_button_Mouse_11_After}},
		{ "gml_Object_obj_research_button_Mouse_10", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_Mouse_10_Before, &Organik::gml_Object_obj_research_button_Mouse_10_After}},
		{ "gml_Object_obj_research_button_Mouse_7", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_Mouse_7_Before, &Organik::gml_Object_obj_research_button_Mouse_7_After}},
		{ "gml_Object_obj_research_button_Draw_0", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_Draw_0_Before, &Organik::gml_Object_obj_research_button_Draw_0_After}},
		{ "gml_Object_obj_research_button_PreCreate_0", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_PreCreate_0_Before, &Organik::gml_Object_obj_research_button_PreCreate_0_After}},
		{ "gml_Object_obj_research_button_PARENT_Create_0", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_PARENT_Create_0_Before, &Organik::gml_Object_obj_research_button_PARENT_Create_0_After}},
		{ "gml_Object_obj_research_button_PARENT_Alarm_0", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_PARENT_Alarm_0_Before, &Organik::gml_Object_obj_research_button_PARENT_Alarm_0_After}},
		{ "gml_Object_rb_dailybonus_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_rb_dailybonus_PreCreate_0_Before, &Organik::gml_Object_rb_dailybonus_PreCreate_0_After}},
		{ "gml_Object_rb_dailybonus_Create_0", std::vector<fnCallback> {&Organik::gml_Object_rb_dailybonus_Create_0_Before, &Organik::gml_Object_rb_dailybonus_Create_0_After}},
		{ "gml_Object_obj_research_button_PARENT_PreCreate_0", std::vector<fnCallback>{&Organik::gml_Object_obj_research_button_PARENT_PreCreate_0_Before, &Organik::gml_Object_obj_research_button_PARENT_PreCreate_0_After}},
		
		// { "gml_Object_obj_net_MS_client_Create_0", std::vector<fnCallback> { &Organik::MSClientCreate_0_Before, &Organik::MSClientCreate_0_After}},
		// { "gml_Object_obj_net_MS_client_Destroy_1", std::vector<fnCallback> { &Organik::MSClientDestroy_1_Before, &Organik::MSClientDestroy_1_After}},
		/* { "gml_Object_obj_net_MS_client_Step_2", std::vector<fnCallback*> { &Organik::MSClientStep_2_Before, &Organik::MSClientStep_2_After}}, */
		/* { "gml_Object_obj_net_MS_client_Step_0", std::vector<fnCallback*> { &Organik::MSClientStep_0_Before, &Organik::MSClientStep_0_After}}, */
		// { "gml_Object_obj_net_MS_client_Other_68", std::vector<fnCallback> { &Organik::MSClientOther_68_Before, &Organik::MSClientOther_68_After}},
		// { "gml_Object_obj_net_MS_client_Other_25", std::vector<fnCallback*> { &Organik::MSClientOther_25_Before, &Organik::MSClientOther_25_After}},
		// { "gml_Object_obj_net_MS_client_Other_15", std::vector<fnCallback*> { &Organik::MSClientOther_15_Before, &Organik::MSClientOther_15_After}},
		// { "gml_Object_obj_net_MS_client_Other_13", std::vector<fnCallback*> { &Organik::MSClientOther_13_Before, &Organik::MSClientOther_13_After}},
		// { "gml_Object_obj_net_MS_client_Other_12", std::vector<fnCallback*> { &Organik::MSClientOther_12_Before, &Organik::MSClientOther_12_After}},
		// { "gml_Object_obj_net_MS_client_Other_10", std::vector<fnCallback*> { &Organik::MSClientOther_10_Before, &Organik::MSClientOther_10_After}},
		// { "gml_Object_obj_net_MS_client_Other_4", std::vector<fnCallback*> { &Organik::MSClientOther_4_Before, &Organik::MSClientOther_4_After}},
		/*{ "gml_Object_obj_net_MS_client_Draw_0", std::vector<fnCallback*> { &Organik::MSClientDraw_0_Before, &Organik::MSClientDraw_0_After }},  */
		{ "gml_Object_obj_player_Alarm_10", std::vector<fnCallback> {&Organik::gml_Object_obj_player_Alarm_10_Before, &Organik::gml_Object_obj_player_Alarm_10_After}},
		{ "gml_Object_obj_player_Alarm_5", std::vector<fnCallback> {&Organik::gml_Object_obj_player_Alarm_5_Before, &Organik::gml_Object_obj_player_Alarm_5_After}},
		{ "gml_Object_obj_player_Alarm_3", std::vector<fnCallback> {&Organik::gml_Object_obj_player_Alarm_3_Before, &Organik::gml_Object_obj_player_Alarm_3_After}},
		{ "gml_Object_obj_player_Alarm_1", std::vector<fnCallback> {&Organik::gml_Object_obj_player_Alarm_1_Before, &Organik::gml_Object_obj_player_Alarm_1_After}},
		{ "gml_Object_obj_player_Alarm_0", std::vector<fnCallback> {&Organik::gml_Object_obj_player_Alarm_0_Before, &Organik::gml_Object_obj_player_Alarm_0_After}},
		// { "gml_Object_obj_net_MS_client_PreCreate_0", std::vector<fnCallback> { &Organik::MSClientPreCreate_0_Before, &Organik::MSClientPreCreate_0_After}},
		{ "gml_Object_obj_cursor_mainmenu_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Create_0_Before, &Organik::gml_Object_obj_cursor_mainmenu_Create_0_After}},
		{ "gml_Object_obj_cursor_mainmenu_Step_0", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Step_0_Before, &Organik::gml_Object_obj_cursor_mainmenu_Step_0_After }},
		{ "gml_Object_obj_cursor_mainmenu_Mouse_54", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Mouse_54_Before, &Organik::gml_Object_obj_cursor_mainmenu_Mouse_54_After}},
		{ "gml_Object_obj_cursor_mainmenu_Mouse_53", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Mouse_53_Before, &Organik::gml_Object_obj_cursor_mainmenu_Mouse_53_After}},
		// { "gml_Object_obj_cursor_mainmenu_PreCreate_0", std::vector<fnCallback*> {&Organik::gml_Object_obj_cursor_mainmenu_PreCreate_0_Before, &Organik::gml_Object_obj_cursor_mainmenu_PreCreate_0_After}},
		//{ "gml_Object_obj_mm_research_trigger_Step_0", std::vector<fnCallback*> {&Organik::gml_Object_obj_mm_research_trigger_Step_0_Before, &Organik::gml_Object_obj_mm_research_trigger_Step_0_After}},
		{ "gml_Object_obj_mm_research_trigger_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_obj_mm_research_trigger_PreCreate_0_Before, &Organik::gml_Object_obj_mm_research_trigger_PreCreate_0_After}},
		{ "gml_Object_outgame_Step_0", std::vector<fnCallback> {&Organik::gml_Object_outgame_Step_0_Before, nullptr}},
		{ "gml_Object_outgame_Draw_0", std::vector<fnCallback> {nullptr, &Organik::gml_Object_outgame_Draw_0_After}},
		{ "gml_Object_outgame_Alarm_4", std::vector<fnCallback> {&Organik::gml_Object_outgame_Alarm_4_Before, &Organik::gml_Object_outgame_Alarm_4_After}},
		{ "gml_Object_outgame_Alarm_5", std::vector<fnCallback> {&Organik::gml_Object_outgame_Alarm_5_Before, &Organik::gml_Object_outgame_Alarm_5_After}},
		{ "gml_Object_outgame_Alarm_6", std::vector<fnCallback> {&Organik::gml_Object_outgame_Alarm_6_Before, &Organik::gml_Object_outgame_Alarm_6_After}},
		{ "gml_Object_outgame_Alarm_7", std::vector<fnCallback> {&Organik::gml_Object_outgame_Alarm_7_Before, &Organik::gml_Object_outgame_Alarm_7_After}},
		{ "gml_Object_outgame_Other_10", std::vector<fnCallback> {&Organik::gml_Object_outgame_Other_10_Before, &Organik::gml_Object_outgame_Other_10_After}},
		{ "gml_Object_outgame_KeyPress_32", std::vector<fnCallback> {&Organik::gml_Object_outgame_KeyPress_32_Before, &Organik::gml_Object_outgame_KeyPress_32_After}},
		{ "gml_Object_outgame_KeyPress_35", std::vector<fnCallback> {&Organik::gml_Object_outgame_KeyPress_35_Before, &Organik::gml_Object_outgame_KeyPress_35_After}},
		{ "gml_Object_outgame_KeyPress_46", std::vector<fnCallback> {&Organik::gml_Object_outgame_KeyPress_46_Before, &Organik::gml_Object_outgame_KeyPress_46_After}},
		{ "gml_Object_obj_all_items_Step_0", std::vector<fnCallback> {&Organik::gml_Object_obj_all_items_Step_0_Before, &Organik::gml_Object_obj_all_items_Step_0_After } },
		{ "gml_Object_obj_item_drop_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_item_drop_Create_0_Before, &Organik::gml_Object_obj_item_drop_Create_0_After } },
		{ "gml_Object_itemSystem_Other_12", std::vector<fnCallback> {&Organik::gml_Object_itemSystem_Other_12_Before, &Organik::gml_Object_itemSystem_Other_12_After } },
		{ "gml_Object_itemSystem_Other_11", std::vector<fnCallback> {&Organik::gml_Object_itemSystem_Other_11_Before, &Organik::gml_Object_itemSystem_Other_11_After } },
		{ "gml_Object_obj_item_parent_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Create_0_Before, &Organik::gml_Object_obj_item_parent_Create_0_After } },
		{ "gml_Object_obj_item_parent_Destroy_0", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Destroy_0_Before, &Organik::gml_Object_obj_item_parent_Destroy_0_After } },
		{ "gml_Object_obj_item_parent_Alarm_11", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Alarm_11_Before, &Organik::gml_Object_obj_item_parent_Alarm_11_After } },
		{ "gml_Object_obj_item_parent_Alarm_2", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Alarm_2_Before, &Organik::gml_Object_obj_item_parent_Alarm_2_After } },
		{ "gml_Object_obj_item_parent_Alarm_0", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Alarm_0_Before, &Organik::gml_Object_obj_item_parent_Alarm_0_After } },
		{ "gml_Object_obj_item_parent_Step_2", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Step_2_Before, &Organik::gml_Object_obj_item_parent_Step_2_After } },
		{ "gml_Object_obj_item_parent_Step_0", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Step_0_Before, &Organik::gml_Object_obj_item_parent_Step_0_After } },
		{ "gml_Object_obj_item_parent_Keyboard_115", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Keyboard_115_Before, &Organik::gml_Object_obj_item_parent_Keyboard_115_After } },
		{ "gml_Object_obj_item_parent_Other_22", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Other_22_Before, &Organik::gml_Object_obj_item_parent_Other_22_After } },
		{ "gml_Object_obj_item_parent_Other_14", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Other_14_Before, &Organik::gml_Object_obj_item_parent_Other_14_After } },
		{ "gml_Object_obj_item_parent_Other_4", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_Other_4_Before, &Organik::gml_Object_obj_item_parent_Other_4_After } },
		{ "gml_Object_obj_item_parent_KeyPress_17", std::vector<fnCallback> {&Organik::gml_Object_obj_item_parent_KeyPress_17_Before, &Organik::gml_Object_obj_item_parent_KeyPress_17_After } },
		{ "gml_Object_obj_debuff_cheat_mod_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_debuff_cheat_mod_Create_0_Before, &Organik::gml_Object_obj_debuff_cheat_mod_Create_0_After } },
		{ "gml_Object_obj_debuff_cheat_mod_Destroy_0", std::vector<fnCallback> {&Organik::gml_Object_obj_debuff_cheat_mod_Destroy_0_Before, &Organik::gml_Object_obj_debuff_cheat_mod_Destroy_0_After } },
		{ "gml_Object_obj_debuff_cheat_mod_Alarm_1", std::vector<fnCallback> {&Organik::gml_Object_obj_debuff_cheat_mod_Alarm_1_Before, &Organik::gml_Object_obj_debuff_cheat_mod_Alarm_1_After } },
		{ "gml_Object_obj_all_items_KeyPress_114", std::vector<fnCallback> {&Organik::gml_Object_obj_all_items_KeyPress_114_Before, &Organik::gml_Object_obj_all_items_KeyPress_114_After } },
		{ "gml_Object_obj_all_items_KeyPress_115", std::vector<fnCallback> {&Organik::gml_Object_obj_all_items_KeyPress_115_Before, &Organik::gml_Object_obj_all_items_KeyPress_115_After } },
		{ "gml_Object_itemSystem_Create_0", std::vector<fnCallback> {&Organik::gml_Object_itemSystem_Create_0_Before, &Organik::gml_Object_itemSystem_Create_0_After } },
		{ "gml_Object_itemSystem_Step_0", std::vector<fnCallback> {&Organik::gml_Object_itemSystem_Step_0_Before, &Organik::gml_Object_itemSystem_Step_0_After } },
		{ "gml_Object_itemSystem_Step_2", std::vector<fnCallback> {&Organik::gml_Object_itemSystem_Step_2_Before, &Organik::gml_Object_itemSystem_Step_2_After } },
		{ "gml_Object_perkSystem_Alarm_0", std::vector<fnCallback> {&Organik::gml_Object_perkSystem_Alarm_0_Before, &Organik::gml_Object_perkSystem_Alarm_0_After } },
		{ "gml_Object_perkSystem_Step_0", std::vector<fnCallback> {&Organik::gml_Object_perkSystem_Step_0_Before, &Organik::gml_Object_perkSystem_Step_0_After } },
		{ "gml_Object_T_perk_PARENT_Create_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_PARENT_Create_0_Before, &Organik::gml_Object_T_perk_PARENT_Create_0_After } },
		{ "gml_Object_T_perk_PARENT_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_PARENT_PreCreate_0_Before, &Organik::gml_Object_T_perk_PARENT_PreCreate_0_After } },
		{ "gml_Object_T_perk_Riotguard_PARENT_Create_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_Riotguard_PARENT_Create_0_Before, &Organik::gml_Object_T_perk_Riotguard_PARENT_Create_0_After } },
		{ "gml_Object_T_perk_Riotguard_PARENT_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_Riotguard_PARENT_PreCreate_0_Before, &Organik::gml_Object_T_perk_Riotguard_PARENT_PreCreate_0_After } },
		{ "gml_Object_T_perk_SWAT_PARENT_Create_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_SWAT_PARENT_Create_0_Before, &Organik::gml_Object_T_perk_SWAT_PARENT_Create_0_After } },
		{ "gml_Object_T_perk_SWAT_PARENT_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_SWAT_PARENT_PreCreate_0_Before, &Organik::gml_Object_T_perk_SWAT_PARENT_PreCreate_0_After } },
		{ "gml_Object_T_perk_Breacher_PARENT_Create_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_Breacher_PARENT_Create_0_Before, &Organik::gml_Object_T_perk_Breacher_PARENT_Create_0_After } },
		{ "gml_Object_T_perk_Breacher_PARENT_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_T_perk_Breacher_PARENT_PreCreate_0_Before, &Organik::gml_Object_T_perk_Breacher_PARENT_PreCreate_0_After } },
		{ "gml_Object_obj_artefact_testmode_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_artefact_testmode_Create_0_Before, &Organik::gml_Object_obj_artefact_testmode_Create_0_After } },
		{ "gml_Object_obj_artefact_testmode_Destroy_0", std::vector<fnCallback> {&Organik::gml_Object_obj_artefact_testmode_Destroy_0_Before, &Organik::gml_Object_obj_artefact_testmode_Destroy_0_After } },
		{ "gml_Object_obj_artefact_testmode_Alarm_1", std::vector<fnCallback> {&Organik::gml_Object_obj_artefact_testmode_Alarm_1_Before, &Organik::gml_Object_obj_artefact_testmode_Alarm_1_After } },
		{ "gml_Object_obj_artefact_testmode_Other_5", std::vector<fnCallback> {&Organik::gml_Object_obj_artefact_testmode_Other_5_Before, &Organik::gml_Object_obj_artefact_testmode_Other_5_After } },
		{ "gml_Object_obj_artefact_testmode_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_obj_artefact_testmode_PreCreate_0_Before, &Organik::gml_Object_obj_artefact_testmode_PreCreate_0_After } },
		{ "gml_Object_obj_buffs_debuffs_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Create_0_Before, &Organik::gml_Object_obj_buffs_debuffs_Create_0_After } },
		{ "gml_Object_obj_buffs_debuffs_Destroy_0", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Destroy_0_Before, &Organik::gml_Object_obj_buffs_debuffs_Destroy_0_After } },
		{ "gml_Object_obj_buffs_debuffs_Alarm_4", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Alarm_4_Before, &Organik::gml_Object_obj_buffs_debuffs_Alarm_4_After } },
		{ "gml_Object_obj_buffs_debuffs_Alarm_3", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Alarm_3_Before, &Organik::gml_Object_obj_buffs_debuffs_Alarm_3_After } },
		{ "gml_Object_obj_buffs_debuffs_Alarm_2", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Alarm_2_Before, &Organik::gml_Object_obj_buffs_debuffs_Alarm_2_After } },
		{ "gml_Object_obj_buffs_debuffs_Step_2", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Step_2_Before, &Organik::gml_Object_obj_buffs_debuffs_Step_2_After } },
		{ "gml_Object_obj_buffs_debuffs_Step_0", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Step_0_Before, &Organik::gml_Object_obj_buffs_debuffs_Step_0_After } },
		{ "gml_Object_obj_buffs_debuffs_Draw_0", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_Draw_0_Before, &Organik::gml_Object_obj_buffs_debuffs_Draw_0_After } },
		{ "gml_Object_obj_buffs_debuffs_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_obj_buffs_debuffs_PreCreate_0_Before, &Organik::gml_Object_obj_buffs_debuffs_PreCreate_0_After } },
		{ "gml_Object_obj_perk_parent_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_perk_parent_Create_0_Before, &Organik::gml_Object_obj_perk_parent_Create_0_After } },
		{ "gml_Object_obj_perk_parent_Alarm_10", std::vector<fnCallback> {&Organik::gml_Object_obj_perk_parent_Alarm_10_Before, &Organik::gml_Object_obj_perk_parent_Alarm_10_After } },
		{ "gml_Object_obj_perk_parent_Other_4", std::vector<fnCallback> {&Organik::gml_Object_obj_perk_parent_Other_4_Before, &Organik::gml_Object_obj_perk_parent_Other_4_After } },
		{ "gml_Object_obj_perk_parent_Draw_72", std::vector<fnCallback> {&Organik::gml_Object_obj_perk_parent_Draw_72_Before, &Organik::gml_Object_obj_perk_parent_Draw_72_After } },
		{ "gml_Object_obj_perk_parent_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_obj_perk_parent_PreCreate_0_Before, &Organik::gml_Object_obj_perk_parent_PreCreate_0_After } },
		{ "gml_Object_obj_allgame_Step_2", std::vector<fnCallback> {&Organik::gml_Object_obj_allgame_Step_2_Before, &Organik::gml_Object_obj_allgame_Step_2_After } },
		{ "gml_Object_obj_allgame_Draw_0", std::vector<fnCallback> {nullptr, &Organik::gml_Object_obj_allgame_Draw_0_After } },
		{ "gml_Object_obj_stats_Other_10", std::vector<fnCallback> {nullptr, &Organik::gml_Object_obj_stats_Other_10_After } },
		{ "gml_Object_obj_buff_firsttime_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_obj_buff_firsttime_PreCreate_0_Before, &Organik::gml_Object_obj_buff_firsttime_PreCreate_0_After } },
		{ "gml_Object_obj_allgame_Step_0", std::vector<fnCallback> {&Organik::gml_Object_obj_allgame_Step_0_Before, nullptr } },
		{ "gml_Object_obj_LUT_end_Draw_75", std::vector<fnCallback> {nullptr, &Organik::gml_Object_obj_LUT_end_Draw_75_After } },
		// { "gml_Object_obj_net_global_chat_Step_2", std::vector<fnCallback> {&Organik::gml_Object_obj_net_global_chat_Step_2_Before, &Organik::gml_Object_obj_net_global_chat_Step_2_After} },
		// { "gml_Object_obj_net_global_chat_Step_1", std::vector<fnCallback> {&Organik::gml_Object_obj_net_global_chat_Step_1_Before, &Organik::gml_Object_obj_net_global_chat_Step_1_After} },
		// { "gml_Object_obj_net_global_chat_Other_4", std::vector<fnCallback> {&Organik::gml_Object_obj_net_global_chat_Other_4_Before, &Organik::gml_Object_obj_net_global_chat_Other_4_After} },
		// { "gml_Object_obj_chat_parent_Step_2", std::vector<fnCallback> {&Organik::gml_Object_obj_chat_parent_Step_2_Before, &Organik::gml_Object_obj_chat_parent_Step_2_After} },
		// { "gml_Object_obj_chat_parent_Step_1", std::vector<fnCallback> {&Organik::gml_Object_obj_chat_parent_Step_1_Before, &Organik::gml_Object_obj_chat_parent_Step_1_After} },
		// { "gml_Object_obj_chat_parent_Other_4", std::vector<fnCallback> {&Organik::gml_Object_obj_chat_parent_Other_4_Before, &Organik::gml_Object_obj_chat_parent_Other_4_After} },
		// { "gml_Object_obj_chat_parent_Other_10", std::vector<fnCallback> {&Organik::gml_Object_obj_chat_parent_Other_10_Before, &Organik::gml_Object_obj_chat_parent_Other_10_After} },
		// { "gml_Object_obj_chat_parent_Other_11", std::vector<fnCallback> {&Organik::gml_Object_obj_chat_parent_Other_11_Before, &Organik::gml_Object_obj_chat_parent_Other_11_After} },
	};
	for (size_t i = 0; i < Organik::Variables::Hashes.size(); i++)
	{
		int32_t hash;
		g_ModuleInterface->GetVariableSlot(Utils::getGlobalInstance(), Organik::Variables::VariableNamesArr[i], hash);
		Organik::Variables::Hashes[i] = hash;
		Organik::Variables::HashToVariableMap[hash] = Organik::Variables::VariableNamesArr[i];
		GetLogger()->LogFormatted("Variable %s has hash %d", Organik::Variables::VariableNamesArr[i], hash);
	}
	for (size_t i = 0; i < Organik::Objects::ObjIndexes.size(); i++)
	{
		int32_t assetIndex = Utils::builtinAssetGetIndex(Organik::Objects::ObjectNamesArr[i]);
		if (assetIndex < 0)
			GetLogger()->LogFormatted("Lookup index failed for %s", Organik::Objects::ObjectNamesArr[i]);
		else
		{
			Organik::Objects::ObjIndexes[i] = assetIndex;
			GetLogger()->LogFormatted("Object %s has index %d", Organik::Objects::ObjectNamesArr[i], assetIndex);
		}
	}
	for (size_t i = 0; i < Organik::Rooms::AssetIndexes.size(); i++)
	{
		int32_t assetIndex = Utils::builtinAssetGetIndex(Organik::Rooms::RoomNamesArr[i]);
		if (assetIndex < 0)
			GetLogger()->LogFormatted("Lookup index failed for %s", Organik::Rooms::RoomNamesArr[i]);
		else
		{
			Organik::Rooms::AssetIndexes[i] = assetIndex;
			GetLogger()->LogFormatted("Room %s has index %d", Organik::Rooms::RoomNamesArr[i], assetIndex);
		}
	}
	for (size_t i = 0; i < Organik::Scripts::Indexes.size(); i++)
	{
		int index = GetInterface()->GetRunnerInterface().Script_Find_Id(Organik::Scripts::ScriptNamesArr[i]) - 100000;
		if (index < 0)
		{
			GetLogger()->LogFormatted("Lookup index failed for %s", Organik::Scripts::ScriptNamesArr[i]);
			continue;
		}
		else
		{
			Organik::Scripts::Indexes[i] = index;
			GetLogger()->LogFormatted("Script %s has index %d", Organik::Scripts::ScriptNamesArr[i], index);
			CScript* scriptPtr = nullptr;
			if (!Aurie::AurieSuccess(GetInterface()->GetScriptData(index, scriptPtr)))
			{
				MessageBoxA(nullptr, "Failed to get script data for %s", "error", MB_OK | MB_ICONERROR);
				continue;
			}
			GetLogger()->LogFormatted("Found script: %p - %d locals", scriptPtr, (scriptPtr && scriptPtr->m_Code) ? scriptPtr->m_Code->m_LocalsCount : -1);
			Organik::Scripts::ScriptPointers[index] = scriptPtr;
		}
	}

	for (const auto& callbackPair : callbacks)
	{
		const char* eventName = callbackPair.first;
		fnCallback beforeCallback = callbackPair.second[0];
		fnCallback afterCallback = callbackPair.second[1];
		last_status = callbackManagerInterfacePtr->RegisterCodeEventCallback(
			MODNAME,  // Module name
			eventName,  // Code event name (e.g., "gml_Object_obj_net_MS_client_Create_0")
			beforeCallback,
			afterCallback
		);
		
		if (!AurieSuccess(last_status))
		{
			GetLogger()->LogFormatted("Failed to register callback for event: %s", eventName);
			return last_status;
		}
		GetLogger()->LogFormatted("Successfully registered callback for event: %s", eventName);
	}
	return AURIE_SUCCESS;
}

EXPORTED AurieStatus ModuleUnload(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(Module);
	UNREFERENCED_PARAMETER(ModulePath);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return AURIE_SUCCESS;
}
