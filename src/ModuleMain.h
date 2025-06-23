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

/* KEY CODE CONSTANTS */
#define vk_enter                RValue(13)
#define vk_escape               RValue(27)
#define vk_left                 RValue(37)
#define vk_up                   RValue(38)
#define vk_right                RValue(39)
#define vk_down                 RValue(40)
#define vk_space                RValue(32)
#define vk_backspace            RValue(8)
#define vk_tab                  RValue(9)
#define vk_shift                RValue(16)
#define vk_control              RValue(17)
#define vk_alt                  RValue(18)
#define vk_delete               RValue(46)
#define vk_f1                   RValue(112)
#define vk_f12                  RValue(123)

/* MOUSE CONSTANTS*/
#define mb_left                 RValue(1)
#define mb_right                RValue(2)
#define mb_middle               RValue(3)

/* REFERENCE KEYWORDS*/
#define ref_self                RValue(-1)
#define ref_other               RValue(-2)
#define ref_all                 RValue(-3)
#define ref_noone               RValue(-4)
#define ref_global              RValue(-5)
#define ref_undefined           RValue(5)
#define pointer_invalid         RValue(-1)
#define pointer_null            RValue(0)

/* EVENTS */
#define ev_create               RValue(0)
#define ev_destroy              RValue(1)
#define ev_step                 RValue(3)
#define ev_collision            RValue(4)
#define ev_keyboard             RValue(5)
#define ev_mouse                RValue(6)
#define ev_other                RValue(7)
#define ev_draw                 RValue(8)      // Main draw event type

/* SUB-EVENTS: DRAW */
#define ev_draw_normal          RValue(0)      // Normal draw (sub-event 0)
#define ev_draw_gui             RValue(64)     // Draw GUI (sub-event 64)
#define ev_draw_pre             RValue(76)     // Pre-draw (sub-event 76)
#define ev_draw_post            RValue(77)     // Post-draw (sub-event 77)

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