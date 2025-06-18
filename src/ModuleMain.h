#pragma once
#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Logging.h"
#define VERSIONNUM "v1.0.0"
#define MODNAME "Organik Mod Framework " VERSIONNUM

#define SOME_ENUM(DO) \
            DO(x)

        
using namespace Organik;

namespace Organik 
{
    struct vector2 {
        int x;
        int y;
        vector2(int x, int y) : x(x), y(y) {}
        vector2() : x(0), y(0) {}
        vector2 operator+(const vector2& other) const {
            return vector2(x + other.x, y + other.y);
        }
        vector2 operator-(const vector2& other) const {
            return vector2(x - other.x, y - other.y);
        }
        vector2 operator*(int scalar) const {
            return vector2(x * scalar, y * scalar);
        }
        vector2 operator/(int scalar) const {
            return vector2(x / scalar, y / scalar);
        }
        bool operator==(const vector2& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    struct vector4 {
        int x;
        int y;
        int z;
        int w;
        vector4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
        vector4() : x(0), y(0), z(0), w(0) {}
        vector4 operator+(const vector4& other) const {
            return vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }
        vector4 operator-(const vector4& other) const {
            return vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }
    };
    
    typedef vector4 bbox;
    typedef vector2 point;
    typedef vector2 pos2d;

}


extern RValue GMLVarIndexMapGMLHash[1001];

extern CInstance* globalInstance;
extern YYTKInterface* g_ModuleInterface;
extern YYRunnerInterface g_RunnerInterface;
extern Logger *g_LoggerInstance;

extern int objOutgameIndex;         // outgame
extern int objStartGameIndex;       // obj_start_game
extern int objAllGameIndex;         // obj_allgame
extern int objConstructorIndex;     // obj_constructor
extern int objControlIndex;         // obj_control
extern int objRoomCtrlIndex;        // obj_room_ctrl
extern int objEnemyCtrlIndex;       // obj_enemy_ctrl       
extern int objIngameCtrlIndex;      // obj_ingame_ctrl
extern int objPlayerMainmenuIndex;  // obj_player_mainmenu
extern int objLocalPlayerIndex;     // obj_localPlayer
extern int objStatsIndex;           // obj_stats
extern int objStatisticsIndex;      // obj_statistics
extern int objClassUpgradeIndex;    // obj_class_upgrade
extern int objLogmenuIndex;         // obj_logmenu
extern int objHelpmenuIndex;        // obj_helpmenu
extern int objCursorIndex;          // obj_cursor
extern int objCursorMainmenuIndex;  // obj_cursor_mainmenu
extern int objMainmenuIndex;        // obj_mainmenu
extern int objMmTalentselectionMenuIndex; // obj_mm_talentselection_menu
extern int objMmClassesMenuIndex;   // obj_mm_classes_menu
extern int objMmWeaponmenuIndex;    // obj_mm_weaponmenu
extern int objMmItemselectMenuIndex;// obj_mm_itemselect_menu
extern int objMmDifficultymenuIndex;// obj_mm_difficultymenu
extern int objMmReadylobbyIndex;    // obj_mm_readylobby
extern int objMmCreditsdisplayIndex;// obj_mm_creditsdisplay
extern int objMmUnlocksMenuIndex;   // obj_mm_unlocks_menu
extern int objMmGroundIndex;        // obj_mm_ground
extern int objMmWeaponlockerIndex;  // obj_mm_weaponlocker
extern int objMmComputerIndex;      // obj_mm_computer
extern int objMmKitselectIndex;     // obj_mm_kitselect
extern int objMmProgressIndex;      // obj_mm_progress
extern int objScrollingLogIndex;    // obj_scrolling_log
extern int objTooltipIndex;         // obj_tooltip
extern int objTooltipbigIndex;      // obj_tooltipbig
extern int objAllItemsIndex;        // obj_all_items
extern int objItemSlotIndex;        // obj_item_slot
extern int objItemDropIndex;        // obj_item_drop
extern int objItemstatsIndex;       // obj_itemstats
extern int objLevelManagerIndex;    // obj_levelManager
extern int objTeleporterIndex;      // obj_teleporter
extern int objBindingsIndex;        // obj_bindings
extern int objDamagezoneIndex;      // obj_damagezone
extern int objBossSpiderIndex;      // obj_boss_spider
extern int objArtefactMysterybonusIndex; // obj_artefact_mysterybonus
extern int objAndroidconsoleIndex;  // obj_androidconsole
extern int objNetGlobalChatIndex;   // obj_net_global_chat
extern int objOverlaysAboveuiIndex; // obj_overlays_aboveui
extern int objLUTStartIndex;        // obj_LUT_start