#pragma once
#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <deque>
#include "Logging.h"
#define VERSIONNUM "v1.0.0"
#define MODNAME "Organik Mod Framework " VERSIONNUM

#define SOME_ENUM(DO) \
            DO(x)

#define CRASH_LOG_SIZE 1024 * 512 // 512KB of log history
#define PARSER_HISTORY_SIZE 1024 * 32 // 32KB of parser history
#define STACK_TRACE_LINES 1024 // 1024 lines of stack trace (from ExecuteIt)
#define LOG_LINE_LENGTH 256 // 256 characters per line in stack trace
#ifndef LIST_COUNTS
#define LIST_COUNTS
#define VARNAMES_COUNT (sizeof(Organik::Variables::VariableNamesArr) / 4)
#define OBJECTS_COUNT (sizeof(Organik::Objects::ObjectNamesArr) / 4)
#endif

static char crashLogBuffer[CRASH_LOG_SIZE];
static char *crashLogBufferPos = &crashLogBuffer[0];
static std::deque<std::string> parserHistory;
static std::deque<std::string> stackTraceLines;



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
#define vk_i                    RValue(0x49)
#define vk_k                    RValue(0x4B)
#define vk_6                    RValue(0x36)
#define vk_f1                   RValue(112)
#define vk_f2                   RValue(113)
#define vk_f3                   RValue(114)
#define vk_f4                   RValue(115)
#define vk_f5                   RValue(116)
#define vk_f6                   RValue(117)
#define vk_f7                   RValue(118)
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


extern RValue GMLVarIndexMapGMLHash[1001];

extern CInstance* globalInstance;
extern YYTKInterface* g_ModuleInterface;
extern YYRunnerInterface g_RunnerInterface;
extern Logger *g_LoggerInstance;

