#define _CRT_SECURE_NO_WARNINGS
#include <Aurie/shared.hpp>
#include "ModuleMain.h"
#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include <FunctionWrapper/FunctionWrapper.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include "Logging.h"

using namespace Aurie;
using namespace YYTK;

using namespace Organik;
Logger *g_LoggerInstance = nullptr;
YYTKInterface* g_ModuleInterface = nullptr;
CallbackManagerInterface* callbackManagerInterfacePtr = nullptr;
YYRunnerInterface g_RunnerInterface;

HWND hWnd;

CInstance* globalInstance = nullptr;

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
void FrameCallback(FWFrame& FrameContext)
{
	UNREFERENCED_PARAMETER(FrameContext);

	static uint32_t frame_counter = 0;

	if (frame_counter % 30 == 0)
		// g_ModuleInterface->PrintWarning("[Example Plugin] - 30 frames have passed! Framecount: %u", frame_counter);

	frame_counter++;
}

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
	
	last_status = g_ModuleInterface->GetGlobalInstance(&globalInstance);
	
	if (!AurieSuccess(last_status))
	{
		GetLogger()->LogFormatted("Failed to get global instance: %s", AurieStatusToString(last_status));
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}
	else
	{
		std::map<std::string, RValue*> result;
		GetLogger()->LogFormatted("Successfully retrieved global instance at %0p", (void*) globalInstance);
	
	}
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_outgame_Create_0", OutgameCreateBefore, OutgameCreateAfter))) // callback before and after gml_Object_outgame_Create_0
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_outgame_Create_0");
	}	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_outgame_Create_0", OutgameCreateBefore, OutgameCreateAfter))) // callback before and after gml_Object_outgame_Create_0
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_outgame_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_outgame_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// callback before and after gml_Object_obj_enm_base_parent_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_enm_base_parent_Create_0", EnmBaseParentCreateBefore, EnmBaseParentCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_enm_base_parent_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_enm_base_parent_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_cursor_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_cursor_Create_0", CursorCreateBefore, CursorCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_cursor_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_cursor_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_buff_timed_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_buff_timed_Create_0", BuffTimedCreateBefore, BuffTimedCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_buff_timed_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_buff_timed_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}


	// before gml_Object_obj_cursor_camera_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_cursor_camera_Draw_0", CursorCameraDrawBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_cursor_camera_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_cursor_camera_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_buff_statboosts_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_buff_statboosts_Create_0", BuffStatboostsCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_buff_statboosts_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_buff_statboosts_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_upgrademenu_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_upgrademenu_Create_0", UpgradeMenuCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_upgrademenu_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_upgrademenu_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_lastkill_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_lastkill_Create_0", LastKillCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_lastkill_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_lastkill_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_lastkill_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_lastkill_Draw_0", LastKillDrawBefore, LastKillDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_lastkill_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_lastkill_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_main_ui_S1_PreCreate_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_main_ui_S1_PreCreate_0", MainUiS1PreCreateBefore, MainUiS1PreCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_main_ui_S1_PreCreate_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_main_ui_S1_PreCreate_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_main_ui_weapon_PreCreate_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_main_ui_weapon_PreCreate_0", MainUiWeaponPreCreateBefore, MainUiWeaponPreCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_main_ui_weapon_PreCreate_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_main_ui_weapon_PreCreate_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_roll_classitem_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_roll_classitem_Create_0", RollClassItemCreateBefore, RollClassItemCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_roll_classitem_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_roll_classitem_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_selection_menu_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_selection_menu_Create_0", SelectionMenuCreateBefore, SelectionMenuCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_selection_menu_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_selection_menu_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_selection_menu_perk_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_selection_menu_perk_Create_0", SelectionMenuPerkCreateBefore, SelectionMenuPerkCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_selection_menu_perk_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_selection_menu_perk_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_androidconsole_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_androidconsole_Create_0", AndroidConsoleCreateBefore, AndroidConsoleCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_androidconsole_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_androidconsole_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_selection_menu_stat_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_selection_menu_stat_Create_0", SelectionMenuStatCreateBefore, SelectionMenuStatCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_selection_menu_stat_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_selection_menu_stat_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_escapesettings_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_escapesettings_Create_0", EscapeSettingsCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_escapesettings_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_escapesettings_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_dialog_ui_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_dialog_ui_Create_0", DialogUiCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_dialog_ui_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_dialog_ui_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_pausemenu_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_pausemenu_Create_0", PauseMenuCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_pausemenu_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_pausemenu_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before gml_Object_obj_reloading_PreCreate_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_reloading_PreCreate_0", ReloadingPreCreateBefore, nullptr)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_reloading_PreCreate_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_reloading_PreCreate_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_reloading_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_reloading_Create_0", ReloadingCreateBefore, ReloadingCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_reloading_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_reloading_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_reloading_Step_2
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_reloading_Step_2", ReloadingStep2Before, ReloadingStep2After)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_reloading_Step_2");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_reloading_Step_2");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_itemstats_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_itemstats_Create_0", ItemStatsCreateBefore, ItemStatsCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_itemstats_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_itemstats_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_itemstats_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_itemstats_Draw_0", ItemStatsDrawBefore, ItemStatsDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_itemstats_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_itemstats_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_statistics_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_statistics_Create_0", StatisticsCreateBefore, StatisticsCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_statistics_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_statistics_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// before and after gml_Object_obj_overlays_aboveui_Create_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_overlays_aboveui_Create_0", OverlaysAboveUiCreateBefore, OverlaysAboveUiCreateAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_overlays_aboveui_Create_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_overlays_aboveui_Create_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_main_ui_S1_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_main_ui_S1_Draw_0", nullptr, MainUiS1DrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_main_ui_S1_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_main_ui_S1_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_main_ui_weapon_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_main_ui_weapon_Draw_0", nullptr, MainUiWeaponDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_main_ui_weapon_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_main_ui_weapon_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_selection_menu_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_selection_menu_Draw_0", nullptr, SelectionMenuDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_selection_menu_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_selection_menu_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_escapesettings_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_escapesettings_Draw_0", nullptr, EscapeSettingsDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_escapesettings_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_escapesettings_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_androidconsole_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_androidconsole_Draw_0", nullptr, AndroidConsoleDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_androidconsole_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_androidconsole_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_dialog_ui_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_dialog_ui_Draw_0", nullptr, DialogUiDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_dialog_ui_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_dialog_ui_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_pausemenu_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_pausemenu_Draw_0", nullptr, PauseMenuDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_pausemenu_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_pausemenu_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_reloading_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_reloading_Draw_0", nullptr, ReloadingDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_reloading_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_reloading_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	// after gml_Object_obj_overlays_aboveui_Draw_0
	if (AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_overlays_aboveui_Draw_0", nullptr, OverlaysAboveUiDrawAfter)))
	{
		GetLogger()->LogFormatted("Registered callback for %s", "gml_Object_obj_overlays_aboveui_Draw_0");
	}
	else
	{
		GetLogger()->LogFormatted("Failed to register callbacks for %s", "gml_Object_obj_overlays_aboveui_Draw_0");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
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