#define _CRT_SECURE_NO_WARNINGS
#include <Aurie/shared.hpp>
#include "ModuleMain.h"
#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include <FunctionWrapper/FunctionWrapper.hpp>
#include "CallbackManager/CallbackManagerInterface.h"

using namespace Aurie;
using namespace YYTK;

YYTKInterface* g_ModuleInterface = nullptr;
CallbackManagerInterface* callbackManagerInterfacePtr = nullptr;
YYRunnerInterface g_RunnerInterface;
std::ofstream outFile;

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
	outFile = std::ofstream("organik.log", std::ofstream::trunc);
	// Gets a handle to the interface exposed by YYTK
	// You can keep this pointer for future use, as it will not change unless YYTK is unloaded.
	last_status = ObGetInterface(
		"YYTK_Main",
		(AurieInterfaceBase*&)(g_ModuleInterface)
	);

	// If we can't get the interface, we fail loading.
	if (!AurieSuccess(last_status))
	{
		outFile << "Failed to get g_ModuleInterface. How did this happen?" << std::endl;
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}
	else
	{
		outFile << "YYTK_Main found!" << std::endl;
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
		outFile << "Failed to get global instance: " << AurieStatusToString(last_status) << std::endl;
	}
	else
	{
		std::map<std::string, RValue*> result;
		outFile << "Successfully retrieved global instance at 0x" << (void*) globalInstance << std::endl;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_outgame_Create_0", OutgameCreateBefore, OutgameCreateAfter))) // callback before and after gml_Object_outgame_Create_0
	{
		outFile << "Failed to register callbacks for " << "gml_Object_outgame_Create_0" << std::endl;
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;

	}
	// callback before and after gml_Object_obj_enm_base_parent_Create_0
	// before and after gml_Object_obj_cursor_Create_0
	// before and after gml_Object_obj_buff_timed_Create_0
	// before gml_Object_obj_cursor_camera_Draw_0
	// before gml_Object_obj_buff_statboosts_Create_0
	// before gml_Object_obj_upgrademenu_Create_0
	// before gml_Object_obj_lastkill_Create_0
	// before and after gml_Object_obj_lastkill_Draw_0
	// before and after gml_Object_obj_main_ui_S1_PreCreate_0
	// before and after gml_Object_obj_main_ui_weapon_PreCreate_0
	// before and after gml_Object_obj_roll_classitem_Create_0
	// before and after gml_Object_obj_selection_menu_Create_0
	// before and after "gml_Object_obj_selection_menu_perk_Create_0
	// before and after gml_Object_obj_androidconsole_Create_0
	// before and after gml_Object_obj_selection_menu_stat_Create_0
	// before gml_Object_obj_escapesettings_Create_0
	// before gml_Object_obj_dialog_ui_Create_0
	// before gml_Object_obj_pausemenu_Create_0
	// before gml_Object_obj_reloading_PreCreate_0
	// before and after gml_Object_obj_reloading_Create_0
	// before and after gml_Object_obj_reloading_Step_2
	// before and after gml_Object_obj_itemstats_Create_0
	// before and after gml_Object_obj_itemstats_Draw_0
	// before and after gml_Object_obj_statistics_Create_0
	// before and after gml_Object_obj_overlays_aboveui_Create_0
	// after gml_Object_obj_main_ui_S1_Draw_0
	// after gml_Object_obj_main_ui_weapon_Draw_0
	// after gml_Object_obj_selection_menu_Draw_0
	// after gml_Object_obj_escapesettings_Draw_0
	// after gml_Object_obj_androidconsole_Draw_0
	// after gml_Object_obj_dialog_ui_Draw_0
	// after gml_Object_obj_pausemenu_Draw_0
	// after gml_Object_obj_reloading_Draw_0
	// after gml_Object_obj_overlays_aboveui_Draw_0
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