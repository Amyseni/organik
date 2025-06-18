#include "CodeEvents.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include <Windows.h> // For VirtualQuery
#include "CallbackManager/CallbackManagerInterface.h"
#include "ModuleMain.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Logging.h"
#include "UI/UIManager.h"
#include "UI/Button.h"
#include "UI/TextArea.h"
#include <ranges>


void superRerollCallback(Organik::Button* self)
{
    if (GetLogger())
    {
        GetLogger()->LogFormatted("Super Reroll Button Clicked: %s", self->GetLabel());
    }
    // Example of modifying the game state
    // g_ModuleInterface->CallBuiltin("some_function", {RValue(42)});
}

Button &btn1 =




























*new Organik::Button(
                "hi im button",
                20, 20, 250, 40,
                    {{Organik::LEFT, &superRerollCallback}});
using namespace Organik;
// Each CodeEvent is a game engine call to ExecuteIt on a CInstance *self object (equivalent to someObject->someFunction)
// ALL Code event callbacks should match the following function signature
/* 
    void MyHookFunction(
        std::tuple<
            CInstance *self, // effectively *this
            CInstance *other, // may be nullptr
            CCode *code, // the code object that is being executed
            int argc, // number of arguments passed to the function
            RValue *argv // array of arguments passed to the function  
        >& Args
    )
    {
        // Note that this function cannot return a value, as it has athe return type of void.
        // However, since we have access to self, other, and all args, we can modify the global state of the game engine.
        // Additionally, we can modify argv array being passed to the function, or
        // if necessary, detour the CCode function further (though this can have unwanted side effects)
    }
*/
/* stepCount++;
        if (stepCount % 100 == 0)
        {
            if (GetLogger())
            {
                GetLogger()->LogFormatted("MainMenu Cursor Step. Step Count: %d", stepCount);
            }
            stepCount = 0;
        }
        else 
        {
            return;
        }
        CInstance *global;
        // LogHelper(Args, __FUNCTION__);
        if (!GetInterface()->GetGlobalInstance(&global) == AurieStatus::AURIE_SUCCESS)
        {
            if (GetLogger())
            {
                GetLogger()->LogSimple("Failed to get global instance in gml_Object_obj_cursor_mainmenu_Step_0_Before");
            }
            return;
        }
        RValue count = g_ModuleInterface->CallBuiltin(
            "variable_instance_names_count",
            {global->ToRValue()}
        );
        int buf = count.ToInt32();
        std::vector<RValue*> vec = (g_ModuleInterface->CallBuiltin(
            "variable_instance_get_names",
            {global->ToRValue()}
        )).ToRefVector();

        for (auto [i, name] : vec | std::views::enumerate)
        {
            std::string nameStr = name->ToString();
            if (GetLogger())
            {
                GetLogger()->LogFormatted("MainMenu Cursor Step. Child Object %d: %s", i, nameStr.c_str());
            }
        } */


namespace Organik
{
    int stepCount = 0;
    void LogHelper(CallbackParams &Args, const char* caller)
    {
        if (GetLogger())
        {
            CInstance *self = std::get<0>(Args);
            CInstance *other = std::get<1>(Args);
            CCode *CCode = std::get<2>(Args);
            RValue** argv = std::get<3>(Args);
            int argc = std::get<4>(Args);
            RValue name = "";
            RValue buff = RValue(10);
            
            GetLogger()->LogFormatted("%s called", caller);
            GetLogger()->LogFormatted("Self: %p, other: %p, CCode: %p, argv: %p, argc: %d", 
                self,
                other,
                CCode,
                (void*)argv,
                argc
            );
            GetLogger()->LogFormatted("deref per pointer", caller);
            GetLogger()->LogFormatted("Self: %p, other: %p, CCode: %p, argv: %p", 
                *self,
                *other,
                *CCode,
                *argv
            );
        }
    }   
    void MSClientCreate_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientCreate_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientDestroy_1_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientDestroy_1_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    /* 
    void MSClientStep_2_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    */
    /* 
    void MSClientStep_2_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    */
    /* 
    void MSClientStep_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    */
    /* 
    void MSClientStep_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    */
    void MSClientOther_68_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_68_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_25_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_25_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_15_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_13_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_12_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_12_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_10_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_10_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_4_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientOther_4_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientDraw_0_After(CallbackParams &Args)
    {
        // LogHelper(Args, __FUNCTION__);
    }
    void MSClientDraw_0_Before(CallbackParams &Args)
    {
        // LogHelper(Args, __FUNCTION__);
    }
    void MSClientPreCreate_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void MSClientPreCreate_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_outgame_Step_0_Before(CallbackParams &Args)
    {
        Organik::GetLogger()->LogSimple("gml_Object_outgame_Step_0_Before called");
        Organik::UIManager::GetInstance()->StepEvent();
    }
    void gml_Object_outgame_Draw_0_After(CallbackParams &Args)
    {
        Organik::GetLogger()->LogSimple("gml_Object_outgame_Draw_0_After called");
        Organik::UIManager::GetInstance()->DrawEvent();
    }

    void gml_Object_obj_research_button_Create_0_Before(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Create_0_After(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Mouse_11_Before(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Mouse_11_After(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Mouse_10_Before(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Mouse_10_After(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Mouse_7_Before(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Mouse_7_After(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Draw_0_Before(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_Draw_0_After(CallbackParams &Args) 
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PreCreate_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PreCreate_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PARENT_Create_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PARENT_Create_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PARENT_Alarm_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PARENT_Alarm_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PARENT_PreCreate_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_research_button_PARENT_PreCreate_0_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_rb_dailybonus_PreCreate_0_Before(CallbackParams &Args)
    {
        if (GetLogger())
            {
                CInstance *self = std::get<0>(Args);
                RValue *name = new RValue();
                g_ModuleInterface->GetInstanceMember(
                    self,
                    "name",
                    name
                );
                GetLogger()->LogSimple("gml_Object_obj_research_button_Create_0_After called");
                GetLogger()->LogFormatted("Self: %p, name: %s", 
                    self,
                    name
                );
    
                delete name; 
            }
    }
    void gml_Object_rb_dailybonus_PreCreate_0_After(CallbackParams &Args)
    {
        if (GetLogger())
            {
                CInstance *self = std::get<0>(Args);
                RValue *name = new RValue();
                g_ModuleInterface->GetInstanceMember(
                    self,
                    "name",
                    name
                );
                GetLogger()->LogSimple("gml_Object_obj_research_button_Create_0_After called");
                GetLogger()->LogFormatted("Self: %p, name: %s", 
                    self,
                    name
                );
    
                delete name; 
            }
    }
    void gml_Object_rb_dailybonus_Create_0_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}

    void gml_Object_rb_dailybonus_Create_0_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_cursor_mainmenu_Create_0_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_cursor_mainmenu_Create_0_After(CallbackParams &Args)
    { 
        auto uim = Organik::UIManager::GetInstance();
        if (uim == nullptr)
        {
            if (GetLogger())
            {
                GetLogger()->LogSimple("UIManager instance is null in gml_Object_obj_cursor_mainmenu_Create_0_After");
            }
            return;
        }
        
        uim->Add(
            &btn1
        );
    }
    void gml_Object_obj_cursor_mainmenu_Step_0_Before(CallbackParams &Args)
    { 
        Organik::UIManager::GetInstance()->StepEvent();
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_perk_hframe_Step_2_Before(CallbackParams &Args)
    {
        if (objLocalPlayerIndex == -1)
        {
            CInstance *global;
            // LogHelper(Args, __FUNCTION__);
            if (!GetInterface()->GetGlobalInstance(&global) == AurieStatus::AURIE_SUCCESS)
            {
                if (GetLogger())
                {
                    GetLogger()->LogSimple("Failed to get global instance in gml_Object_obj_cursor_mainmenu_Step_0_Before");
                }
                return;
            }
            RValue count = YYTK::GetInterface()->CallBuiltin(
                "variable_instance_names_count",
                {global->ToRValue()}
            );
            int buf = count.ToInt32();
            std::vector<RValue*> vec = (g_ModuleInterface->CallBuiltin(
                "variable_instance_get_names",
                {global->ToRValue()}
            )).ToRefVector();

            for (auto [i, name] : vec | std::views::enumerate)
            {
                std::string nameStr = name->ToString();
                if (GetLogger())
                {
                    GetLogger()->LogFormatted("perk_hframe Step_2. Instance variable #%d: %s", i, nameStr.c_str());
                }
            }
        }
    }

    void gml_Object_obj_cursor_mainmenu_Step_0_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}

    void gml_Object_obj_cursor_mainmenu_Mouse_54_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_cursor_mainmenu_Mouse_54_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}


    void gml_Object_obj_cursor_mainmenu_Mouse_53_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_cursor_mainmenu_Mouse_53_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_cursor_mainmenu_PreCreate_0_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_cursor_mainmenu_PreCreate_0_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    
    void gml_Object_obj_player_Alarm_10_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_10_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_5_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_5_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_3_Before(CallbackParams &Args)
    { 
        Organik::UIManager::GetInstance()->Remove(
            [](const Organik::UIElement &elem) {
                return typeid(elem) == typeid(Organik::Button&);
            }
        );
    }
    void gml_Object_obj_player_Alarm_3_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_1_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_1_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_0_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_player_Alarm_0_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_allgame_Step_2_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);   
    }
    void gml_Object_obj_allgame_Step_2_After(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
        Organik::UIManager::GetInstance()->StepEvent();
    }
    void gml_Object_obj_mm_research_trigger_Step_0_Before(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
        Organik::UIManager::GetInstance()->StepEvent();
    }

    void gml_Object_obj_mm_research_trigger_Step_0_After(CallbackParams &Args)
    { /* LogHelper(Args, __FUNCTION__); */}

    void OutgameCreateBefore(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }

    /// @brief called after the internal outgame constructor is called, which is after the OutgameCreateBefore callback.
    /// @param Args is a tuple containing the following:
    /// - CInstance *self: Reference to the newly created CInstance to our object.
    void OutgameCreateAfter(CallbackParams &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_mm_research_trigger_PreCreate_0_Before(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
    void gml_Object_obj_mm_research_trigger_PreCreate_0_After(CallbackParams &Args)
    { LogHelper(Args, __FUNCTION__);}
}
