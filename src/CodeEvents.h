#pragma once
#include <Aurie/shared.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include "Logging.h"
#include "ModuleMain.h"

namespace Organik
{
    typedef std::tuple<CInstance *, CInstance*, CCode *, RValue**, int> CallbackParams;
    void gml_Object_obj_research_button_Create_0_Before(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Create_0_After(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Mouse_11_Before(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Mouse_11_After(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Mouse_10_Before(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Mouse_10_After(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Mouse_7_Before(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Mouse_7_After(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Draw_0_Before(CallbackParams &Args) ;
    void gml_Object_obj_research_button_Draw_0_After(CallbackParams &Args) ;
    void gml_Object_obj_research_button_PreCreate_0_Before(CallbackParams &Args);
    void gml_Object_obj_research_button_PreCreate_0_After(CallbackParams &Args);
    void gml_Object_obj_research_button_PARENT_Create_0_Before(CallbackParams &Args);
    void gml_Object_obj_research_button_PARENT_Create_0_After(CallbackParams &Args);
    void gml_Object_obj_research_button_PARENT_Alarm_0_Before(CallbackParams &Args);
    void gml_Object_obj_research_button_PARENT_Alarm_0_After(CallbackParams &Args);
    void gml_Object_obj_research_button_PARENT_PreCreate_0_Before(CallbackParams &Args);
    void gml_Object_obj_research_button_PARENT_PreCreate_0_After(CallbackParams &Args);
    void gml_Object_rb_dailybonus_PreCreate_0_Before(CallbackParams &Args);
    void gml_Object_rb_dailybonus_PreCreate_0_After(CallbackParams &Args);
    void gml_Object_rb_dailybonus_Create_0_Before(CallbackParams &Args);
    void gml_Object_rb_dailybonus_Create_0_After(CallbackParams &Args);
    void MSClientCreate_0_Before(CallbackParams &Args);
    void MSClientCreate_0_After(CallbackParams &Args);
    void MSClientDestroy_1_Before(CallbackParams &Args);
    void MSClientDestroy_1_After(CallbackParams &Args);
    // void MSClientStep_2_Before(CallbackParams &Args);
    // void MSClientStep_2_After(CallbackParams &Args);
    // void MSClientStep_0_Before(CallbackParams &Args);
    // void MSClientStep_0_After(CallbackParams &Args);
    void MSClientOther_68_Before(CallbackParams &Args);
    void MSClientOther_68_After(CallbackParams &Args);
    void MSClientOther_25_Before(CallbackParams &Args);
    void MSClientOther_25_After(CallbackParams &Args);
    void MSClientOther_15_Before(CallbackParams &Args);
    void MSClientOther_15_After(CallbackParams &Args);
    void MSClientOther_13_Before(CallbackParams &Args);
    void MSClientOther_13_After(CallbackParams &Args);
    void MSClientOther_12_Before(CallbackParams &Args);
    void MSClientOther_12_After(CallbackParams &Args);
    void MSClientOther_10_Before(CallbackParams &Args);
    void MSClientOther_10_After(CallbackParams &Args);
    void MSClientOther_4_Before(CallbackParams &Args);
    void MSClientOther_4_After(CallbackParams &Args);
    void MSClientDraw_0_Before(CallbackParams &Args);
    void MSClientDraw_0_After(CallbackParams &Args);
    void MSClientPreCreate_0_Before(CallbackParams &Args);
    void MSClientPreCreate_0_After(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Create_0_Before(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Create_0_After(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Step_0_Before(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Step_0_After(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Mouse_54_Before(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Mouse_54_After(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Mouse_53_Before(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_Mouse_53_After(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_PreCreate_0_Before(CallbackParams &Args);
    void gml_Object_obj_cursor_mainmenu_PreCreate_0_After(CallbackParams &Args);
    void gml_Object_obj_mm_research_trigger_Step_0_Before(CallbackParams &Args);
    // void gml_Object_obj_mm_research_trigger_Step_0_After(CallbackParams &Args);
    void gml_Object_obj_mm_research_trigger_PreCreate_0_Before(CallbackParams &Args);
    void gml_Object_obj_mm_research_trigger_PreCreate_0_After(CallbackParams &Args);
    void gml_Object_obj_perk_hframe_Step_2_Before(CallbackParams &Args) ;
    void gml_Object_obj_perk_hframe_Step_2_After(CallbackParams &Args) ;

    void gml_Object_obj_player_Alarm_10_Before(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_10_After(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_5_Before(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_5_After(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_3_Before(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_3_After(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_1_Before(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_1_After(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_0_Before(CallbackParams &Args);
    void gml_Object_obj_player_Alarm_0_After(CallbackParams &Args);
    void gml_Object_outgame_Step_0_Before(CallbackParams &Args);
    void gml_Object_outgame_Draw_0_After(CallbackParams &Args);

}