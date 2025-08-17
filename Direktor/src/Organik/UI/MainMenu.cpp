#include "MainMenu.h"
#include "UIElement.h"
#include "Utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "UIManager.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include <GameMaker_Defs.h>
#include "VariableHelper.h"
#include "DefinitionHelpers/BuiltinHelper.h"
#include "ScriptHelper.h"
#include "room_list.h"

using namespace Organik;
using namespace Utils;

namespace Organik
{
    struct Variables;
    struct Objects;
    struct Rooms;
    struct Scripts;
}

#define GETSCR(script) Organik::Scripts::Indexes[Organik::Scripts::gml_Script_##script]

using Settings = Organik::Utils::Settings;
static int  Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }

bool spawnMode_items = false;
bool assumeObjectPrefix = true;

std::string g_cursedBoxInput;
constexpr uint32_t MIN_ITEMNAME_LENGTH = sizeof("obj_item0_c4");
void MainMenu::Draw(bool& out_mousedOver, bool* p_open, const std::string &title)
{
    
    if (!ImGui::BeginMainMenuBar())
    {
        ImGui::EndMainMenuBar();
        return;
    }
    #define VAR_HASH(var) Organik::Variables::Hashes[Organik::Variables::##var]
    #define OBJ_INDEX(obj) Organik::Objects::ObjIndexes[Organik::Objects::##obj]
    CInstance* allgameObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_allgame); });
    CInstance* outgameObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(outgame); });
    CInstance* localPlayerObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_localPlayer); });
    CInstance* remotePlayerObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_remotePlayer); });
    bool isMultiplayer = remotePlayerObj != nullptr;
    CInstance* objRoomCtrl = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_room_ctrl); });
    bool gamestarted = allgameObj ? allgameObj->InternalReadYYVar(VAR_HASH(gamestarted))->ToBoolean() : false;
    if (ImGui::BeginMenu("Godmode"))
    {
        if (ImGui::MenuItem("Non-Beta Multiplayer (26.1)", "Alt+F2", Settings::g_EnableMultiplayerCompat, outgameObj != nullptr))
        {
            Settings::g_EnableMultiplayerCompat = !Settings::g_EnableMultiplayerCompat;
            YYSetString(outgameObj->InternalReadYYVar(VAR_HASH(ClientVersion)), const_cast<char*>(Settings::g_EnableMultiplayerCompat ? "26.1" : "26.2"));
        }
        if (ImGui::MenuItem("Devcom Mode (!)", "", 
            outgameObj ? outgameObj->InternalReadYYVar(VAR_HASH(devcom))->ToBoolean() : false, // default false (avoid nullptr)
            outgameObj != nullptr
        ))
        {
            *(outgameObj->InternalReadYYVar(VAR_HASH(devcom))) = RValue(!(outgameObj->InternalReadYYVar(VAR_HASH(devcom))->ToBoolean()));
        }
        if (ImGui::MenuItem("Development Display", "", 
            outgameObj ? outgameObj->InternalReadYYVar(VAR_HASH(developmentdisplay))->ToBoolean() : false,
            outgameObj != nullptr
        ))
        {
            (*outgameObj->InternalReadYYVar(VAR_HASH(developmentdisplay))) = !(outgameObj->InternalReadYYVar(VAR_HASH(developmentdisplay))->ToBoolean());
        }
        if (ImGui::BeginMenu("Uh oh... (Stinky poopy)", true))
        {
            if (ImGui::InputTextWithHint("Victim", "127.0.0.1", &g_cursedBoxInput, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
            {
                int bufferIndex = DoBuiltin(&gml_buffer_create, {
                    RValue(1 << 24), // size
                    RValue(eBuffer_Fixed), // type
                    RValue(1) // flags
                }).ToInt32();

                RValue buf = DoBuiltin(&gml_buffer_fill, {
                    RValue(bufferIndex), 
                    RValue(0), 
                    RValue(eBuffer_U8), 
                    RValue(43), 
                    RValue(1<<24)
                });
                CInstance* networkParentObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ID == OBJ_INDEX(obj_networking_parent); });
                int socketUDP = networkParentObj->operator[]("socketUDP")->m_i32;
                // int32_t networkTime = networkParentObj->operator[]("networkTime")->m_i32;
                // RValue result;
                // std::vector<RValue*> args = {
                //     &RValue(socketUDP), 
                //     &RValue(g_cursedBoxInput), 
                    
                // ScriptFromId(GETSCR(scr_send_packet))->m_Functions->m_Function.m_ScriptFunction(
                //     GetGlobalInstance(),
                //     GetGlobalInstance(),
                //     &result, // result
                    
                //     {
                //         &RValue(socketUDP), 
                //         &RValue(g_cursedBoxInput), 
                //         &RValue(eBuffer_U8), 
                //         &RValue(43), 
                //         &RValue(1<<24)
                //     }
                // );
                // DoBuiltin(&gml_script)

                
                
                // g_ModuleInterface->CallBuiltin("buffer_fill", 
                //     {
                //         RValue(socketUDP), 
                //         RValue(g_cursedBoxInput.c_str()),
                        
                //         RValue(YYTK::eBuffer_U8), 
                //         RValue(43), 
                //         RValue(1<<24)
                //     }
                // );
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Player"))
        {
            if (ImGui::MenuItem("Invincible", "Ctrl+Shift+I", Settings::g_EnableInvincibility, gamestarted))
            {
                Settings::g_EnableInvincibility = !Settings::g_EnableInvincibility;
                (*localPlayerObj->InternalReadYYVar(VAR_HASH(invincible))) = Settings::g_EnableInvincibility;
                if (Settings::g_EnableInvincibility)
                {
                    (*localPlayerObj->InternalReadYYVar(VAR_HASH(invincibletimer))) = RValue(99999.00000);
                }
                else
                {
                    (*localPlayerObj->InternalReadYYVar(VAR_HASH(invincibletimer))) = RValue(0.00000);
                }
            }
            bool amIDead = (isMultiplayer)  //since single player doesn't allow respawning, only enable in mulit.
                            ?(localPlayerObj->InternalReadYYVar(VAR_HASH(multiplayerDeath))->ToBoolean()) 
                            : false;
            bool mpDead = (isMultiplayer)  //since single player doesn't allow respawning, only enable in mulit.
                            ?(remotePlayerObj->InternalReadYYVar(VAR_HASH(multiplayerDeath))->ToBoolean()) 
                            : false;
            if (ImGui::MenuItem("Revive Self", "", false, gamestarted && (amIDead)))
            {
                CInstance* cursorCamera = CInstance::FirstOrDefault(
                    [](CInstance* ci) -> bool { return ci->m_ID == OBJ_INDEX(obj_cursor_camera); }
                );
                (*localPlayerObj->operator[](VAR_HASH(multiplayerDeathRespawnTime))) = RValue(0.02f);
            }
            ImGui::EndMenu(); // player
        }
        ImGui::EndMenu(); // Godmode
    }
    if (ImGui::BeginMenu("Objects"))
    {
        if (ImGui::BeginMenu("Spawn"))
        {
            ImGui::Checkbox("Filter Items", &spawnMode_items);
            ImGui::SameLine();
            ImGui::Checkbox("Assume obj_[name]", &assumeObjectPrefix);
            static ImGuiTextFilter filter;
            ImGui::Text("Item search: \n"
                "  Type to filter objects by name (ignore obj_)\n"
                "  Use the checkboxes to filter object types.\n"
                "  \"^enm_\" - display objects starting with obj_enm_\"\n"
                "  \"grenade,missile\" display objects containing \"grenade\" or \"missile\"\n"
                "  \"-boss\" exclude objects containing \"beam\"\n"
                "Click on an object to spawn it at your position.\n"
                "Should work in multiplayer (best results as host)."
            );
            filter.Draw("Filter");
            if (ImGui::BeginChild("##itemList", ImVec2(0.0, 250), ImGuiChildFlags_Border))
            {
                for (uint32_t i = 0; i < Organik::Objects::ObjIndexes.size(); i++)
                {
                    const char* objName = Organik::Objects::ObjectNamesArr[i];
                    const int32_t objIndex = Organik::Objects::ObjIndexes[i];
                    const uint32_t objNameLen = strlen(objName);
                    // c4 is 1 char longer, but sizeof counts the null terminator
                    if (assumeObjectPrefix)
                    {
                        ImGuiTextFilter filter2("obj_");
                        if (!filter2.PassFilter(objName))
                            continue;
                    }
                    if (strlen(filter.InputBuf))
                    {
                        if (filter.InputBuf[0] == '^')
                        {
                            const char* input = (assumeObjectPrefix ? std::string("obj_") + filter.InputBuf : filter.InputBuf).c_str();
                            size_t inputLen = strlen(input);
                            if (objNameLen < (inputLen <= 5 ? (5+inputLen) : inputLen))
                                continue;
                            for (uint32_t j = 0; j < strlen(filter.InputBuf) - 1; j++)
                            {
                                if (objName[j] != input[j])
                                    continue;
                            }
                        }
                        else
                        {
                            if (assumeObjectPrefix)
                            {
                                ImGuiTextFilter filter2("obj_");
                                if (!filter2.PassFilter(objName))
                                    continue;
                                if (!filter.PassFilter(objName))
                                    continue;
                            }
                            else
                            {
                                if (!filter.PassFilter(objName))
                                    continue;
                            }
                        }
                    }
                    if (ImGui::Selectable(objName, false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
                    {
                        using scr = Organik::Scripts;
                        if (scr::gml_Script_scr_instance_create < 0)
                        {
                            MessageBoxA(nullptr, "Script gml_Script_scr_instance_create not found", "Error", MB_OK | MB_ICONERROR);
                            continue;
                        }
                        RValue result;
                        RValue posX = RValue(localPlayerObj->m_X);
                        RValue posY = RValue(localPlayerObj->m_Y);
                        RValue objIndexR = RValue(objIndex);
                        std::vector<RValue> args = 
                        {
                            posX,
                            posY,
                            objIndexR
                        };
                        Script_Perform(scr::Indexes[scr::gml_Script_scr_instance_create], GetGlobalInstance(), GetGlobalInstance(), args.size(), &result, args.data());
                        // createScript->m_Functions->m_Function.m_ScriptFunction(
                        //     GetGlobalInstance(),
                        //     GetGlobalInstance(),
                        //     &result,
                        //     args.size(),
                        //     args.data()
                        // );
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndMenu(); // Items
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Rooms"))
    {
        if (ImGui::BeginMenu("Goto Room"))
        {
            static ImGuiTextFilter filter;
            ImGui::Text("Room search: \n"
                "WORKS BEST IF YOU ARE IN A RUN.\nMAY CRASH IF USED IN A MENU.\n"
                "  Type to filter rooms by name (case insensitive)\n"
                "  \"tutorial\" display rooms containing \"tutorial\"\n"
                "  \"tutorial,test\" display rooms containing \"tutorial\" or \"test\"\n"
                "  \"-boss\" exclude rooms containing \"boss\"\n"
                "Click on a room to teleport to it.\nWorks in multiplayer if you are the host."
            );
            filter.Draw("Filter");
            if (ImGui::BeginChild("##roomList", ImVec2(300, 180), ImGuiChildFlags_Border))
            {
                for (uint32_t i = 0; i < Organik::Rooms::AssetIndexes.size(); i++)
                {
                    if (filter.PassFilter(Organik::Rooms::RoomNamesArr[i]))
                    {
                        if (ImGui::Selectable(Organik::Rooms::RoomNamesArr[i], false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
                        {
                            int scriptIndex = Organik::Scripts::gml_Script_scr_goto_room;
                            
                            (*objRoomCtrl->InternalReadYYVar(VAR_HASH(roomTransitionTarget))) = Organik::Rooms::AssetIndexes[i];
                            (*objRoomCtrl->InternalReadYYVar(VAR_HASH(roomTransitionRunning))) = RValue((const int32_t)1l);
                        }
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndMenu(); // Goto Room
        }
        ImGui::EndMenu(); // Move/Teleport
    }
    if (ImGui::BeginMenu("Windows"))
    {
        for (const auto& [name, info] : UIManager::windowTypes)
        {
            if (ImGui::MenuItem(name.c_str(), std::get<0>(info).c_str(), std::get<2>(info)() != nullptr)) 
            {
                std::get<1>(info)();
            }
        }
        ImGui::EndMenu(); // Windows
    }
    if (ImGui::BeginMenu("Instances"))
    {
        ImGui::EndMenu(); // Instances
    }
    if (ImGui::BeginMenu("Options/Bugfixes"))
    {
        if (ImGui::MenuItem("HG Slowdown Fix/Unstuck", "", false, gamestarted))
            *localPlayerObj->InternalReadYYVar(VAR_HASH(mod_movespeed)) = RValue(1.00000);
        ImGui::EndMenu(); // Options/Bugfixes

    }
    ImGui::EndMainMenuBar();
}
