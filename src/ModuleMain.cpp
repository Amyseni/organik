#define _CRT_SECURE_NO_WARNINGS
#include <Aurie/shared.hpp>
#include "ModuleMain.h"
#include "CodeEvents.h"
#include "YYToolkit/YYTK_Shared.hpp"
#include "FunctionWrapper/FunctionWrapper.hpp"
#include "CallbackManager/CallbackManagerInterface.h"
#include "Logging.h"

using namespace Aurie;
using namespace YYTK;
using namespace Organik;

int objOutgameIndex = -1;
int objStartGameIndex = -1;
int objAllGameIndex = -1;
int objConstructorIndex = -1;
int objControlIndex = -1;
int objRoomCtrlIndex = -1;
int objEnemyCtrlIndex = -1;
int objIngameCtrlIndex = -1;
int objPlayerMainmenuIndex = -1;
int objLocalPlayerIndex = -1;
int objStatsIndex = -1;
int objStatisticsIndex = -1;
int objClassUpgradeIndex = -1;
int objLogmenuIndex = -1;
int objHelpmenuIndex = -1;
int objCursorIndex = -1;
int objCursorMainmenuIndex = -1;
int objMainmenuIndex = -1;
int objMmTalentselectionMenuIndex = -1;
int objMmClassesMenuIndex = -1;
int objMmWeaponmenuIndex = -1;
int objMmItemselectMenuIndex = -1;
int objMmDifficultymenuIndex = -1;
int objMmReadylobbyIndex = -1;
int objMmCreditsdisplayIndex = -1;
int objMmUnlocksMenuIndex = -1;
int objMmGroundIndex = -1;
int objMmWeaponlockerIndex = -1;
int objMmComputerIndex = -1;
int objMmKitselectIndex = -1;
int objMmProgressIndex = -1;
int objScrollingLogIndex = -1;
int objTooltipIndex = -1;
int objTooltipbigIndex = -1;
int objAllItemsIndex = -1;
int objItemSlotIndex = -1;
int objItemDropIndex = -1;
int objItemstatsIndex = -1;
int objLevelManagerIndex = -1;
int objTeleporterIndex = -1;
int objBindingsIndex = -1;
int objDamagezoneIndex = -1;
int objBossSpiderIndex = -1;
int objArtefactMysterybonusIndex = -1;
int objAndroidconsoleIndex = -1;
int objNetGlobalChatIndex = -1;
int objOverlaysAboveuiIndex = -1;
int objLUTStartIndex = -1;

Organik::Logger* g_LoggerInstance;
YYTKInterface* g_ModuleInterface = nullptr;
CallbackManagerInterface* callbackManagerInterfacePtr = nullptr;
YYRunnerInterface g_RunnerInterface;
CInstance **g_globalInstance;
HWND hWnd;
CInstance* globalInstance = nullptr;
int frameCounter = 0;
bool debug = false;
typedef void (*fnCallback)(CallbackParams&);

void FrameCallback(FWFrame& FrameContext)
{
	UNREFERENCED_PARAMETER(FrameContext);
	frameCounter++;

	if (!(frameCounter % 30 == 0))
		return;

	int scriptIndex;
	RValue version = g_ModuleInterface->CallBuiltin("gamemaker_version", {});
	Organik::GetLogger()->LogFormatted("GameMaker version: %s", version.GetKindName().c_str());
	if (g_ModuleInterface->GetNamedRoutineIndex(
		"gml_Script_debug",
		&scriptIndex
	) == AURIE_SUCCESS)
	{
		if (!debug) {
			debug = true;
			g_ModuleInterface->CallBuiltin("show_debug_overlay", {RValue(true)});
			
		}
			
	};
}
struct CallbackInfo {
	char *functionName;
	fnCallback *callbackBefore;
	fnCallback *callbackAfter;
	CallbackInfo(char *_fnName, fnCallback *_before, fnCallback *_after) : functionName(_fnName), callbackBefore(_before), callbackAfter(_after) 
	{	
	}
};
EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);
	
	AurieStatus last_status = AURIE_SUCCESS;
	bool loging_initialized = Logger::InitLogging();
	if (!loging_initialized)
	{
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

	last_status = g_ModuleInterface->CreateCallback(
		Module,
		EVENT_FRAME,
		FrameCallback,
		0
	);

	std::map<const char *, std::vector<fnCallback>> callbacks
	{
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
		
		{ "gml_Object_obj_net_MS_client_Create_0", std::vector<fnCallback> { &Organik::MSClientCreate_0_Before, &Organik::MSClientCreate_0_After}},
		{ "gml_Object_obj_net_MS_client_Destroy_1", std::vector<fnCallback> { &Organik::MSClientDestroy_1_Before, &Organik::MSClientDestroy_1_After}},
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
		{ "gml_Object_obj_net_MS_client_PreCreate_0", std::vector<fnCallback> { &Organik::MSClientPreCreate_0_Before, &Organik::MSClientPreCreate_0_After}},
		{ "gml_Object_obj_cursor_mainmenu_Create_0", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Create_0_Before, &Organik::gml_Object_obj_cursor_mainmenu_Create_0_After}},
		{ "gml_Object_obj_cursor_mainmenu_Step_0", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Step_0_Before, &Organik::gml_Object_obj_cursor_mainmenu_Step_0_After }},
		{ "gml_Object_obj_cursor_mainmenu_Mouse_54", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Mouse_54_Before, &Organik::gml_Object_obj_cursor_mainmenu_Mouse_54_After}},
		{ "gml_Object_obj_cursor_mainmenu_Mouse_53", std::vector<fnCallback> {&Organik::gml_Object_obj_cursor_mainmenu_Mouse_53_Before, &Organik::gml_Object_obj_cursor_mainmenu_Mouse_53_After}},
		// { "gml_Object_obj_cursor_mainmenu_PreCreate_0", std::vector<fnCallback*> {&Organik::gml_Object_obj_cursor_mainmenu_PreCreate_0_Before, &Organik::gml_Object_obj_cursor_mainmenu_PreCreate_0_After}},
		//{ "gml_Object_obj_mm_research_trigger_Step_0", std::vector<fnCallback*> {&Organik::gml_Object_obj_mm_research_trigger_Step_0_Before, &Organik::gml_Object_obj_mm_research_trigger_Step_0_After}},
		{ "gml_Object_obj_mm_research_trigger_PreCreate_0", std::vector<fnCallback> {&Organik::gml_Object_obj_mm_research_trigger_PreCreate_0_Before, &Organik::gml_Object_obj_mm_research_trigger_PreCreate_0_After}},
		{ "gml_Object_outgame_Step_0", std::vector<fnCallback> {&Organik::gml_Object_outgame_Step_0_Before, nullptr}},
		{ "gml_Object_outgame_Draw_0", std::vector<fnCallback> {nullptr, &Organik::gml_Object_outgame_Draw_0_After}}
	};
	if (!AurieSuccess(last_status))
	{
		g_ModuleInterface->Print(CM_RED, "Failed to register frame callback");
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

	return AURIE_SUCCESS;
}
