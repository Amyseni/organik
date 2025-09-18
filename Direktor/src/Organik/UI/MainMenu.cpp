#include "Synthetik.h"
#include "MainMenu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "UIManager.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include <GameMaker_Defs.h>
#include "VariableHelper.h"
#include "Globals.h"
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
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
    {
        out_mousedOver = true;
    }
    #define OBJ_INDEX(obj) Organik::Objects::ObjIndexes[Organik::Objects::##obj]
    CInstance* allgameObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_allgame); });
    CInstance* outgameObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(outgame); });
    CInstance* localPlayerObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_localPlayer); });
    CInstance* remotePlayerObj = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_remotePlayer); });
    bool isMultiplayer = remotePlayerObj != nullptr;
    
    CInstance* objRoomCtrl = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_room_ctrl); });
    CInstance* objControl = CInstance::FirstOrDefault([](CInstance* ci) -> bool { return ci->m_ObjectIndex == OBJ_INDEX(obj_control); });
    bool gamestarted = allgameObj ? allgameObj->InternalReadYYVar(VAR_HASH(gamestarted))->ToBoolean() : false;
    if (ImGui::BeginMenu("Godmode"))
    {
        out_mousedOver = true;
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
        if (ImGui::BeginMenu("Player"))
        {
            out_mousedOver = true;
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
            if (ImGui::MenuItem("Roll Me (Randomize Class)", "Ctrl+Shift+I", Settings::g_EnableInvincibility, !gamestarted))
            {
                std::vector<RValue*> classPerks = outgameObj->InternalReadYYVar(VAR_HASH(perkslots))->ToRefVector();
                if (classPerks.size() > 0)
                {
                    std::vector<RValue*> subclassPerks = classPerks[outgameObj->InternalReadYYVar(VAR_HASH(selectedclass))->ToInt32()]->ToRefVector();
                    std::vector<RValue*> subclassPerkSet = subclassPerks[outgameObj->InternalReadYYVar(VAR_HASH(selectedsubclass))->ToInt32()]->ToRefVector();
                    std::vector<RValue*> perkGroups = outgameObj->InternalReadYYVar(VAR_HASH(perk_group))->ToRefVector();
                    std::vector<RValue*> perkClasses = outgameObj->InternalReadYYVar(VAR_HASH(perk_class))->ToRefVector();
                    auto mt = Utils::getrng();
                    // 2 = Pistol, 4 = Core Class Perk

                    std::vector<RValue> pistolPerks;
                    std::vector<RValue> corePerks;
                    std::vector<RValue> otherPerks;
                    for (uint i=0; i<perkGroups.size(); i++)
                    {
                        RValue* perkGroup = perkGroups[i];
                        if (perkGroup->ToDouble() == 2.0) {
                            pistolPerks.push_back(RValue(static_cast<double>(i)));
                            continue;
                        }

                        if (perkGroup->ToDouble() == 4.0) {
                            corePerks.push_back(RValue(static_cast<double>(i)));
                            continue;
                        }
                        double classId = perkClasses[i]->ToDouble();
                        if (classId > 0.0 && classId < 5.0)
                        {
                            otherPerks.push_back(RValue(static_cast<double>(i)));
                        }
                    };
                    
                    for(uint i=0;i<subclassPerkSet.size(); i++)
                    {
                        RValue* perk = subclassPerkSet[i];
                        if (perk->ToDouble() == 0.0)
                            continue;
                        if (i == 0)
                        {
                            double newPerk = corePerks[(*mt)() % corePerks.size()].ToDouble();
//                             Organik::GetLogger()->LogFormatted("Setting core perk slot %d to %f", i, newPerk);
                            *perk = RValue(newPerk);
                            continue;
                        }
                        if (i == subclassPerkSet.size() - 1)
                        {
                            double newPerk = pistolPerks[(*mt)() % pistolPerks.size()].ToDouble();
//                             Organik::GetLogger()->LogFormatted("Setting pistol perk slot %d to %f", i, newPerk);
                            *perk = RValue(newPerk);
                            continue;
                        }
                        
                        if (i == 1 || i == 2)
                        {
                            continue;
                        }
                        double newPerk = otherPerks[(*mt)() % otherPerks.size()].ToDouble();
//                         Organik::GetLogger()->LogFormatted("Setting other perk slot %d to %f", i, newPerk);
                        *perk = RValue(newPerk);
                    }
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
        out_mousedOver = true;
        if (ImGui::BeginMenu("Spawn"))
        {
            // static ImGuiTextFilter filter;
            // ImGui::Text("Object search: \n"
            //     "  Type to filter objects by name (ignore obj_)\n"
            //     "  \"^enm_\" - display objects starting with obj_enm_\"\n"
            //     "  \"grenade,missile\" display objects containing \"grenade\" or \"missile\"\n"
            //     "  \"-boss\" exclude objects containing \"beam\"\n"
            //     "Click on an object to select it.\n"
            // );
            // filter.Draw("Filter");
            // if (ImGui::BeginChild("##itemList", ImVec2(0.0, 250), ImGuiChildFlags_Border))
            // {
            //     for (uint32_t i = 0; i < Organik::Objects::ObjIndexes.size(); i++)
            //     {
            //         const char* objName = Organik::Objects::ObjectNamesArr[i];
            //         const int32_t objIndex = Organik::Objects::ObjIndexes[i];
            //         const uint32_t objNameLen = strlen(objName);
            //         if (strlen(filter.InputBuf))
            //         {
            //             if (!filter.PassFilter(objName))
            //                 continue;
            //         }
                    auto fn = [&] (int32_t objIndex) {
                        RValue result = RValue(-5);
//                         Organik::GetLogger()->LogFormatted("Calling instance_create (%p) for object index %d", &gml_Script_instance_create, objIndex);
                        RValue posX = RValue(localPlayerObj->m_X);
                        RValue posY = RValue(localPlayerObj->m_Y);
                        RValue objIndexR = RValue(objIndex);
                        std::vector<RValue*> args = 
                        {
                            &posX,
                            &posY,
                            &objIndexR
                        };
                        gml_Script_instance_create(
                            GetGlobalInstance(),
                            GetGlobalInstance(),
                            &result,
                            args.size(),
                            args.data()
                        );
                    };
                    DrawObjectSelector(fn);
            //     }
            // }
            
            ImGui::EndMenu(); // Items
        }
        if (ImGui::BeginMenu("Replace Bullets"))
        {
            ImGui::Text("Replace all Bullets with:");
            ImGui::NewLine();
            ImGui::Text(*getReplaceBulletIndex() ? DoBuiltin(&gml_object_get_name, {RValue(*getReplaceBulletIndex())}).ToCString() : "None");
            auto fn = [&] (int32_t objIndex) {
                *getReplaceBulletIndex() = static_cast<double>(objIndex);
            };
            DrawObjectSelector(fn);
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Rooms"))
    {
        out_mousedOver = true;
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
                            
                            int32_t roomSeed = (*Utils::getrng())();
                            (*objRoomCtrl->InternalReadYYVar(VAR_HASH(roomSeed))) = RValue(roomSeed);
                            (*objRoomCtrl->InternalReadYYVar(VAR_HASH(roomTransitionTarget))) = Organik::Rooms::AssetIndexes[i];
                            (*objRoomCtrl->InternalReadYYVar(VAR_HASH(roomTransitionRunning))) = RValue(true);

                            if (objControl == nullptr)
                            {
//                                 GetLogger()->LogFormatted("obj_control instance not found.");
                                continue;
                            }
                            if (!isMultiplayer)
                            {
//                                 GetLogger()->LogFormatted("%d not multiplayer", __LINE__);
                                return;

                            }
                            RValue buffer = DoBuiltin(&gml_buffer_create, {
                                RValue(16), // size
                                RValue(eBuffer_Grow), // type
                                RValue(1) 
                            });
                            IBuffer* p_Buffer = reinterpret_cast<IBuffer*>(buffer.ToPointer());
                            RValue rBuffIndex = RValue(buffer.ToInt32());  // buffer of packet data
                            RValue ret_DoesntMatter = RValue(-5);
                            short packetID = 37;
                            short roomID = (short) Organik::Rooms::AssetIndexes[i];
                            int32_t roomClearedAmount = objRoomCtrl->InternalReadYYVar(VAR_HASH(roomclearedamount))->ToInt32();
                            RValue godRoom = RValue(0);

                            DoBuiltin(&gml_buffer_write, { rBuffIndex,
                                RValue(eBuffer_U16),
                                RValue(packetID)
                            });
                            DoBuiltin(&gml_buffer_write, { rBuffIndex,
                                RValue(eBuffer_S16),
                                RValue(roomID)
                            });
                            int32_t currentFloor = objRoomCtrl->InternalReadYYVar(VAR_HASH(currentfloor))->ToInt32();

                            DoBuiltin(&gml_buffer_write, { rBuffIndex,
                                RValue(eBuffer_S32),
                                RValue(roomID)
                            });
                            DoBuiltin(&gml_buffer_write, { rBuffIndex,
                                RValue(eBuffer_S32),
                                RValue(roomSeed)
                            });
                            DoBuiltin(&gml_buffer_write, { rBuffIndex,
                                RValue(eBuffer_S32),
                                RValue(roomClearedAmount)
                            });
                            DoBuiltin(&gml_buffer_write, { rBuffIndex,
                                RValue(eBuffer_Bool),
                                godRoom
                            });
                            RValue rReliable = RValue(1ll);           // 1 = reliable packet
                            RValue ret = RValue(0);
                            
                            std::vector<RValue*> args = 
                            {
                                &rBuffIndex, // the buffer
                                &rReliable   // u can also just lie
                            };
                            gml_Script_scr_send_packet(
                                GetGlobalInstance(),
                                GetGlobalInstance(),
                                &ret,
                                args.size(),
                                args.data()
                            );
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
        out_mousedOver = true;
        for (const auto& [name, info] : UIManager::windowTypes)
        {
            if (ImGui::MenuItem(name.c_str(), std::get<0>(info).c_str(), std::get<2>(info)() != nullptr)) 
            {
                std::get<1>(info)();
            }
        }
        ImGui::EndMenu(); // Windows
    }
    if (ImGui::BeginMenu("Options/Bugfixes"))
    {
        out_mousedOver = true;
        if (ImGui::MenuItem("HG Slowdown Fix/Unstuck", "", false, gamestarted))
            *localPlayerObj->InternalReadYYVar(VAR_HASH(mod_movespeed)) = RValue(1.00000);
        ImGui::MenuItem("Enable Bug Reporting", "", Settings::GetEnableBugWebhook());
        ImGui::EndMenu(); // Options/Bugfixes
    }
    if (ImGui::MenuItem("Join us on Discord!"))
    {   
        RValue out;
        RValue args;
        YYCreateString(&args, "https://discord.gg/KrTA2QkyCx ");
        gml_YoYo_OpenURL(&out, GetGlobalInstance(), GetGlobalInstance(), 1, &args);
    }
    ImGui::EndMainMenuBar();
}
