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
#include <ranges>
#include "Utils.h"

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
    void LogHelper(CodeEventArgs &Args, const char* caller)
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
        }
    }   
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    void gml_Object_obj_splash_screen_Keyboard_13_Before(CodeEventArgs &Args)
    {
        LogHelper(Args, __FUNCTION__);
        
    }
    void gml_Object_obj_splash_screen_Keyboard_13_After(CodeEventArgs &Args)
    {
        LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_outgame_Step_0_Before(CodeEventArgs &Args)
    {
        //Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
        // Organik::GetLogger()->LogSimple("gml_Object_outgame_Step_0_Before called");
    }
    void gml_Object_outgame_Draw_0_After(CodeEventArgs &Args)
    {
      //Organik::GetLogger()->LogFormatted("%s - %s:%d", __FUNCTION__, __FILE__, __LINE__);
        // Organik::GetLogger()->LogSimple("gml_Object_outgame_Draw_0_After called");
       //Organik::UIManager::GetInstance()->DrawEvent();
    }
    void gml_Object_outgame_Alarm_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_6_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_6_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_7_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Alarm_7_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Other_10_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_Other_10_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_KeyPress_32_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_KeyPress_32_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_KeyPress_35_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_KeyPress_35_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_KeyPress_46_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_outgame_KeyPress_46_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_research_button_Create_0_Before(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Create_0_After(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Mouse_11_Before(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Mouse_11_After(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Mouse_10_Before(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Mouse_10_After(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Mouse_7_Before(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Mouse_7_After(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Draw_0_Before(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_Draw_0_After(CodeEventArgs &Args) 
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PreCreate_0_Before(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PreCreate_0_After(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PARENT_Create_0_Before(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PARENT_Create_0_After(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PARENT_Alarm_0_Before(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PARENT_Alarm_0_After(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PARENT_PreCreate_0_Before(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_research_button_PARENT_PreCreate_0_After(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_rb_dailybonus_PreCreate_0_Before(CodeEventArgs &Args)
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
    void gml_Object_rb_dailybonus_PreCreate_0_After(CodeEventArgs &Args)
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
    void gml_Object_rb_dailybonus_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}

    void gml_Object_rb_dailybonus_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_Create_0_After(CodeEventArgs &Args)
    { 
    }       
    void gml_Object_obj_cursor_mainmenu_Step_0_Before(CodeEventArgs &Args)
    { 
        // LogHelper(Args, __FUNCTION__);
    }
    void gml_Object_obj_perk_hframe_Step_2_Before(CodeEventArgs &Args)
    {
        // LogHelper(Args, __FUNCTION__);
    }

/*TODO log the instance variables of perk objects
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
*/
    void gml_Object_obj_cursor_mainmenu_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }

    void gml_Object_obj_cursor_mainmenu_Mouse_54_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_Mouse_54_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_Mouse_53_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_Mouse_53_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_cursor_mainmenu_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_10_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_10_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_3_Before(CodeEventArgs &Args)
    { 
    }
    void gml_Object_obj_player_Alarm_3_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_player_Alarm_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_allgame_Step_2_Before(CodeEventArgs &Args)
    { /* UIManager::GetInstance()->StepEvent(); */ }
    void gml_Object_obj_allgame_Step_0_Before(CodeEventArgs &Args)
    { 
    }
    void gml_Object_obj_allgame_Step_2_After(CodeEventArgs &Args)
    { 
        
     }
    void gml_Object_obj_allgame_Draw_0_After(CodeEventArgs &Args)
    {
        // GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
        // UIManager::GetInstance()->DrawEvent();
    }
    void gml_Object_obj_LUT_end_Draw_75_After(CodeEventArgs &Args)
    {
        // Add ImGui frame management here BEFORE calling the original frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        Organik::UIManager::GetConsole()->Draw(std::string("Organik Console"), &Organik::UIManager::GetInstance()->showConsole);
        std::vector<CInstance*> activeInstances;
        if (Organik::UIManager::GetInstance()->showVariableViewer)
        {
            RValue layers = g_ModuleInterface->CallBuiltin(
                "layer_get_all",
                {}
            );
            for (auto layer : layers.ToRefVector())
            {
                int layerID = layer->ToInt32();
                RValue layerElements = g_ModuleInterface->CallBuiltin(
                    "layer_get_all_elements",
                    {RValue(layerID)}
                );
                for (auto element : layerElements.ToRefVector())
                {
                    RValue elementType = g_ModuleInterface->CallBuiltin(
                        "layer_get_element_type",
                        {RValue(element->ToInt32())}
                    );
                    if (elementType.ToInt32() == 2) // instance == 2
                    {
                        RValue elementInstanceID = g_ModuleInterface->CallBuiltin(
                            "layer_instance_get_instance",
                            {RValue(element->ToInt32())}
                        );
                        CInstance* instance = Organik::Utils::GetInstanceFromID(elementInstanceID.ToInt32());
                        if (instance)
                        {
                            activeInstances.push_back(instance);
                        }
                    }
                }
            }
            std::vector<CInstance*> extraInstances;
            auto playerInstances = Organik::Utils::findInstances(Organik::Utils::getObj_PlayerIndex());
            if (!playerInstances.empty())
                extraInstances.append_range(playerInstances);
            auto chatInstances = Organik::Utils::findInstances(Organik::Utils::getObj_PlayerIndex());
            if (!chatInstances.empty())
                extraInstances.append_range(chatInstances);
            auto statsInstances = Organik::Utils::findInstances(Organik::Utils::getObj_Chat_ParentIndex());
            if (!statsInstances.empty())
                activeInstances.append_range(statsInstances);
            auto perksInstances = Organik::Utils::findInstances(Organik::Utils::getObj_Perk_ParentIndex());
            if (!perksInstances.empty())
                activeInstances.append_range(perksInstances);
            auto itemsInstances = Organik::Utils::findInstances(Organik::Utils::getObj_Item_ParentIndex());
            if (!itemsInstances.empty())
                extraInstances.append_range(itemsInstances);
            auto weaponsInstances = Organik::Utils::findInstances(Organik::Utils::getObj_Weapon_ParentIndex());
            if (!weaponsInstances.empty())
                extraInstances.append_range(weaponsInstances);
            auto pickupsInstances = Organik::Utils::findInstances(Organik::Utils::getObj_Pickup_ParentIndex());
            if (!pickupsInstances.empty())
                extraInstances.append_range(pickupsInstances);
            auto itemSlotsInstances = Organik::Utils::findInstances(Organik::Utils::getObj_Item_SlotIndex());
            if (!itemSlotsInstances.empty())
                extraInstances.append_range(itemSlotsInstances);
            if (!extraInstances.empty())
                activeInstances.append_range(extraInstances);
            ShowVariableViewer(&UIManager::GetInstance()->showVariableViewer, activeInstances);
        }


        // Render ImGui
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        GetLogger()->LogSimple("Step Event");
        UIManager::GetInstance()->StepEvent(); 

        // GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
        //GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
        
    }
    void gml_Object_obj_mm_research_trigger_Step_0_Before(CodeEventArgs &Args)
    { /* LogHelper(Args, __FUNCTION__); */ }

    void gml_Object_obj_mm_research_trigger_Step_0_After(CodeEventArgs &Args)
    { /* LogHelper(Args, __FUNCTION__); */}

    void OutgameCreateBefore(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }

    void OutgameCreateAfter(CodeEventArgs &Args)
    {
        GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);
    }
    void gml_Object_obj_mm_research_trigger_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}
    void gml_Object_obj_mm_research_trigger_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args);}

    void gml_Object_obj_all_items_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_all_items_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_item_drop_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_drop_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Other_12_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Other_12_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Other_11_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Other_11_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Alarm_11_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Alarm_11_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Alarm_2_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Alarm_2_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Alarm_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Alarm_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_item_parent_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_item_parent_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_item_parent_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_item_parent_Keyboard_115_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Keyboard_115_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Other_22_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Other_22_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Other_14_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Other_14_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_KeyPress_17_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_item_parent_KeyPress_17_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_mod_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_mod_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_mod_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_mod_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_mod_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_mod_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_all_items_KeyPress_114_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_all_items_KeyPress_114_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_all_items_KeyPress_115_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_all_items_KeyPress_115_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_itemSystem_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_itemSystem_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_itemSystem_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_itemSystem_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_perkSystem_Alarm_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_perkSystem_Alarm_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_perkSystem_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_perkSystem_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_T_perk_PARENT_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_PARENT_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_PARENT_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_PARENT_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Riotguard_PARENT_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Riotguard_PARENT_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Riotguard_PARENT_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Riotguard_PARENT_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_SWAT_PARENT_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_SWAT_PARENT_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_SWAT_PARENT_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_SWAT_PARENT_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Breacher_PARENT_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Breacher_PARENT_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Breacher_PARENT_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_T_perk_Breacher_PARENT_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_testmode_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Alarm_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Alarm_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Alarm_3_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Alarm_3_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Alarm_2_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Alarm_2_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buffs_debuffs_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buffs_debuffs_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buffs_debuffs_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buffs_debuffs_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buffs_debuffs_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buffs_debuffs_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buffs_debuffs_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Alarm_10_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Alarm_10_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_Draw_72_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_parent_Draw_72_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_parent_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_parent_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_parent_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_parent_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_parent_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_parent_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_weaponcarry_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_itemupgrade_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ricochet_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ricochet_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ricochet_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ricochet_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ricochet_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ricochet_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_terrorlevel_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_elemental_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ultradrop_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ultradrop_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ultradrop_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ultradrop_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ultradrop_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_ultradrop_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_friendlyfire_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_artefact_slowdown_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_artefact_slowdown_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_slowdown_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_healing_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_shop_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_crit_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_strafe_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_powerup_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_buff_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_pistol_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_madness_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_mysterybonus_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_instagib_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_tactical_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_artefact_randomrun_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_riotguard_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_riotguard_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_riotguard_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_riotguard_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_riotguard_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_riotguard_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_riotguard_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_guardian1_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_guardian1_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_guardian1_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_guardian1_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_guardian1_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_breacher_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_breacher_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_breacher_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_breacher_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_breacher_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_breacher_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_breacher_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_warmup_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_iframe_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_iframe_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_iframe_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_iframe_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_iframe_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_iframe_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_iframe_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_iframe_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_aegis_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_aegis_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_aegis2_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_enrage_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_enrage_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_enrage_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatup_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_closer_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_closer_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_closer_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_closer_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_closer_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_closer_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_closer_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_closer_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgun_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shotgunmaster_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shieldoc_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_bloodinfusion_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_bloodinfusion_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_creditsdisplay_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_creditsdisplay_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_creditsdisplay_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_bloodinfusion1_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_lowhpregen_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_lowhpregen_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lowhpregen_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_scavengerbits_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_scavengerbits_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavengerbits_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_recovery_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_recovery_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_recovery_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lifeline_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_return_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_berserk_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_berserk_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_berserk_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_berserk_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_berserk_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_berserk_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_berserk_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_berserk_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reloadsurge2_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reloadsurge2_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge2_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scraparmor_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scraparmor_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scraparmor_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scraparmor_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scraparmor_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scraparmor_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_rogue_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_rogue_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_rogue_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_rogue_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rogue_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assassin_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assassin_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assassin_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assassin_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assassin_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assassin_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assassin_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_marksman_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_marksman_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_marksman_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_marksman_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_marksman_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_marksman_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_marksman_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_longrange_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_longrange_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_longrange_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_longrange_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_longrange_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_longrange_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_longrange_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_longrange_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ejectsurge_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ejectsurge_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ejectsurge_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ejectsurge_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ejectsurge_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_ejectsurge_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_ejectsurge_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ejectsurge_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dodgeboost_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dodgeboost_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeboost_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_powertuning_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_powertuning_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powertuning_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mastercard_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mastercard_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mastercard_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mastercard_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mastercard_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mastercard_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_crossbow_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_backstab_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_backstab_Other_15_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Other_15_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_backstab_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_diehard_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_diehard_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_diehard_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_discipline_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dance_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dance_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dance_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dance_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dance_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dance_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dance_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dance_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_chest_weapon_wood_arena_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_chest_weapon_wood_arena_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_chest_weapon_wood_arena_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_chest_weapon_wood_arena_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_headshotammo_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_headshotammo_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_headshotammo_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_headshotammo_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_headshotammo_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_headshotammo_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_perfection_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specialized_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specialized_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specialized_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specialized_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specialized_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_specialized_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_specialized_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specialized_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_customammo_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_mindmg_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_holdbreathold_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_holdbreathold_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreathold_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reactivereload_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reactivereload_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reactivereload_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reactivereload_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reactivereload_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reactivereload_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reactivereload_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reactivereload_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_standstill_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_standstill_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_standstill_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_stealback_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_stealback_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stealback_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_commando_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_commando_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_commando_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_commando_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_commando_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_raider_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_raider_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_raider_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_raider_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_raider_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_raider_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_raider_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assaultgunner_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assaultgunner_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assaultgunner_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_assaultgunner_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_assaultgunner_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_squadleader_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_squadleader_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_squadleader_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_squadleader_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_squadleader_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_squadleader_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_squadleader_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ultrashot_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_wepupgrade_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drill_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drill_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drill_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drill_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drill_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_drill_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_drill_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drill_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rankup_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rankup_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rankup_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rankup_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rankup_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_rankup_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fieldration_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_specializedammo_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadstack_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadstack_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadstack_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadstack_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadstack_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reloadstack_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reloadstack_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadstack_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_obsession_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_obsession_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_obsession_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_obsession_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_obsession_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_obsession_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_obsession_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_obsession_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powerstep_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powerstep_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powerstep_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powerstep_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powerstep_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_powerstep_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_powerstep_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_powerstep_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemcdvariant_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemcdvariant_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemcdvariant_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemcdvariant_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemcdvariant_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_itemcdvariant_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_itemcdvariant_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemcdvariant_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killer_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_stubborn_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_stubborn_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_stubborn_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reloadsurge_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_reloadsurge_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_reloadsurge_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_fortify_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_holdbreath_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_holdbreath_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_holdbreath_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hollowpoint_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_cover_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_cover_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cover_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_edge_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_edge_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_edge_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_scarred_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_scarred_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scarred_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_combo_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_combo_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_combo_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dogtags_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dogtags_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dogtags_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_explosiveammo_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_explosiveammo_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_explosiveammo_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_explosiveammo_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_explosiveammo_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_elemental_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_elemental_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elemental_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_demolisher_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_demolisher_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_demolisher_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_demolisher_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_demolisher_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_annihilator_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_annihilator_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_annihilator_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_annihilator_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_annihilator_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_lasercaster_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_lasercaster_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_lasercaster_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_lasercaster_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_lasercaster_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_engineer_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_engineer_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_engineer_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_engineer_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_Draw_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_engineer_Draw_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_engineer_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_engineer_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatcontrol_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatcontrol_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatcontrol_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatcontrol_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatcontrol_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_heatcontrol_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_heatcontrol_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatcontrol_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_pistolextender_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_statusextender_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hframe_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hframe_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hframe_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hframe_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
//    void gml_Object_obj_perk_hframe_Step_2_Before(CodeEventArgs &Args)
//    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_hframe_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_hframe_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_hframe_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_focus_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_focus_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_focus_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_grenadier_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_grenadier_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_grenadier_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_calm_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_m79_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_force_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dodgeheat_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_dodgeheat_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_dodgeheat_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_sunrise_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_sunrise_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_sunrise_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_sunrise_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_sunrise_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_sunrise_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_sunrise_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_sunrise_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_selfrepair_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_selfrepair_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_selfrepair_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_ammoregen_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_ammoregen_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ammoregen_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_heatrecharge_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_heatrecharge_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_heatrecharge_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_transmutate_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_drone1_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_elementalpower_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_morph_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_morph_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_morph_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_cultist_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_cultist_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_cultist_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_coolhead_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_coolhead_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_coolhead_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_coolhead_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_coolhead_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_coolhead_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_steady_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_steady_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_steady_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_steady_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_steady_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_steady_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthcrate_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthcrate_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthcrate_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthcrate_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthcrate_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthcrate_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_barrier_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_none_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_none_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_none_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_none_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_randomperk_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_talent_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_talent_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_talent_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_talent_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_talent_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_talent_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healthy_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_classweapon_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_perk_classweapon_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_classweapon_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_killshield_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_shophack_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_itemslot_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_healroom_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_prepared_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_credits_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_luck_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_scavenging_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_nimble_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_terrorlevel_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_brightguard_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_brightguard_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_brightguard_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_brightguard_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_brightguard_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_brightguard_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ballisticsdiv_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ballisticsdiv_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ballisticsdiv_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ballisticsdiv_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ballisticsdiv_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_ballisticsdiv_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_orderofzir_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_orderofzir_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_orderofzir_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_orderofzir_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_orderofzir_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_perk_orderofzir_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_statboosts_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_statboosts_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_statboosts_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_movespeed_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_movespeed_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_move_barbed_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_maxhealth_curse_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_cheat_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_hardmode_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_hardmode_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_hardmode_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_hardmode_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_hardmode_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_hardmode_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_debuff_loop_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_debuff_loop_Other_5_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Other_5_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Other_4_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_Other_4_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_loop_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healthreg_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firerate_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_healing_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_overclock_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_overclock_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_overclock_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_healing_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shield_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_invulner_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_explosion_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_mines_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_mines_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_mines_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_thunder_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_thunder_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_thunder_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_fire_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_fire_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fire_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_drones_target_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_fanOfKnives_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_fanOfKnives_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_fanOfKnives_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_turrets_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_turrets_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_turrets_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_slowmo_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_slowmo_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_slowmo_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_keycard_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_keycard_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_keycard_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_keycard_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_keycard_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_keycard_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_keycard_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_keycard_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_shieldreg_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_incdamage_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_damageresi_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_instagib_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_instagib_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_instagib_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_enmdmgresi_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_timed_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_timed_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_timed_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_timed_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_timed_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_timed_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_timed_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_timed_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_dodge_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammoreg_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_reloadspeed_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_spreadcontrol_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_flaregun_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_smokescreen_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_psycloud_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_psycloud_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_psycloud_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_critchance_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_crystal_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_crystal_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_crystal_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_ammo2_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_ammo2_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ammo2_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_poison_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_bleeding_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_burning_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_Step_2_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_debuff_stun_Step_2_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_debuff_stun_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_stun_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_antishield_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_antishield_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_antishield_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_antishield_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_antishield_Step_0_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_debuff_antishield_Step_0_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_debuff_antishield_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_debuff_antishield_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_speedboost_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_stimpack_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_airhorn_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_ministim_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_Step_1_Before(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_expboost_onlyexp_Step_1_After(CodeEventArgs &Args)
    { /*LogHelper(Args, __FUNCTION__);*/ }
    void gml_Object_obj_buff_expboost_onlyexp_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_expboost_onlyexp_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_Create_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_Create_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_Destroy_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_Destroy_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_Alarm_1_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_Alarm_1_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_PreCreate_0_Before(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
    void gml_Object_obj_buff_firsttime_PreCreate_0_After(CodeEventArgs &Args)
    { GetLogger()->LogEventCallback(__FILE__, __LINE__, __FUNCTION__, Args); }
}