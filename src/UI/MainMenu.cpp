#include "../ModuleMain.h"
#include "MainMenu.h"
#include "UIElement.h"
#include "../Utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "UIManager.h"
#include "../InstanceHelper.h"
#include "../VariableHelper.h"
#include "../ScriptHelper.h"
#include "../room_list.h"

using namespace Organik;
using namespace Utils;

namespace Organik
{
    struct Variables;
    struct Objects;
    struct Rooms;
    struct Scripts;
}

#define GETSCR(script) Organik::Scripts::ScriptPointers[Organik::Scripts::Indexes[Organik::Scripts::gml_Script_##script]]


static int  Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }

bool spawnMode_items = false;
bool assumeObjectPrefix = true;
#define INLINE_SLOT_AND_VAR_GETTER(objname, varname) \
    static int32_t varname##Slot; \
    static inline int32_t get##objname##varname##Slot() { \
        if (varname##Slot) \
            return varname##Slot; \
        g_ModuleInterface->GetVariableSlot(Organik::Instances::get##objname()->ToRValue(), #varname, varname##Slot); \
        if (!varname##Slot) \
            Organik::Utils::Crash("Failed to resolve " #objname " variable slot: " #varname); \
        return varname##Slot; \
    } \
    static inline RValue *get##objname##varname() { \
        static RValue *varname; \
        if (varname##Slot) { \
            varname = &Organik::Instances::get##objname()->InternalGetYYVarRef(varname##Slot); \
            if (varname) \
                return varname; \
        } \
        get##objname##varname##Slot(); \
        if (!varname##Slot) { \
            if (!varname) \
                Organik::Utils::Crash("Failed to resolve " #objname " variable: " #varname); \
            return varname; \
        } \
        varname = &Organik::Instances::get##objname()->InternalGetYYVarRef(varname##Slot); \
        if (varname) \
            return varname; \
        Organik::Utils::Crash("Failed to resolve " #objname " variable: " #varname); \
        return nullptr; \
    }

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
    CInstance* allgameObj = Utils::FirstInstanceOrNullptr(OBJ_INDEX(obj_allgame));
    CInstance* outgameObj = Utils::FirstInstanceOrNullptr(OBJ_INDEX(outgame));
    CInstance* localPlayerObj = Utils::FirstInstanceOrNullptr(OBJ_INDEX(obj_localPlayer));
    CInstance* remotePlayerObj = Utils::FirstInstanceOrNullptr(OBJ_INDEX(obj_remotePlayer));
    bool isMultiplayer = remotePlayerObj != nullptr;
    CInstance* objRoomCtrl = Utils::FirstInstanceOrNullptr(OBJ_INDEX(obj_room_ctrl));
    bool gamestarted = allgameObj ? allgameObj->InternalGetYYVarRef(VAR_HASH(gamestarted)).ToBoolean() : false;
    if (ImGui::BeginMenu("Godmode"))
    {
        if (ImGui::MenuItem("Non-Beta Multiplayer (26.1)", "Alt+F2", g_EnableMultiplayerCompat, outgameObj != nullptr))
        {
            g_EnableMultiplayerCompat = !g_EnableMultiplayerCompat;
            outgameObj->InternalGetYYVarRef(VAR_HASH(ClientVersion)) = g_EnableMultiplayerCompat ? "26.1" : "26.2";
        }
        if (ImGui::MenuItem("Devcom Mode (!)", "", 
            outgameObj ? outgameObj->InternalGetYYVarRef(VAR_HASH(devcom)).ToBoolean() : false, // default false (avoid nullptr)
            outgameObj != nullptr
        ))
        {
            outgameObj->InternalGetYYVarRef(VAR_HASH(devcom)) = !(outgameObj->InternalGetYYVarRef(VAR_HASH(devcom)).ToBoolean());
        }
        if (ImGui::MenuItem("Development Display", "", 
            outgameObj ? outgameObj->InternalGetYYVarRef(VAR_HASH(developmentdisplay)).ToBoolean() : false,
            outgameObj != nullptr
        ))
        {
            outgameObj->InternalGetYYVarRef(VAR_HASH(developmentdisplay)) = !(outgameObj->InternalGetYYVarRef(VAR_HASH(developmentdisplay)).ToBoolean());
        }
        if (ImGui::BeginMenu("Player"))
        {
            if (ImGui::MenuItem("Invincible", "Ctrl+Shift+I", g_EnableInvincibility, gamestarted))
            {
                g_EnableInvincibility = !g_EnableInvincibility;
                localPlayerObj->InternalGetYYVarRef(VAR_HASH(invincible)) = g_EnableInvincibility;
                if (g_EnableInvincibility)
                {
                    localPlayerObj->InternalGetYYVarRef(VAR_HASH(invincibletimer)) = RValue(99999.00000);
                }
                else
                {
                    localPlayerObj->InternalGetYYVarRef(VAR_HASH(invincibletimer)) = RValue(0.00000);
                }
            }
            bool amIDead = (remotePlayerObj)  //since single player doesn't allow respawning, only enable in mulit.
                            ?(localPlayerObj->InternalGetYYVarRef(VAR_HASH(multiplayerDeath)).ToBoolean()) 
                            : false;
            bool mpDead = (remotePlayerObj) 
                            ? (localPlayerObj 
                                ? (localPlayerObj->InternalGetYYVarRef(VAR_HASH(multiplayerDeath)).ToBoolean())
                                : false)
                            : false;
            if (ImGui::BeginMenu("Revive Self...", gamestarted && (amIDead)))
            {
                if (ImGui::MenuItem("... on Partner", "", false, isMultiplayer && !mpDead))
                {
                    CScript* reviveScript = GETSCR(scr_player_multiplayer_revive);

                    if (reviveScript == nullptr)
                    {
                        MessageBoxA(nullptr, "Script gml_Script_scr_player_multiplayer_revive not found", "Error", MB_OK | MB_ICONERROR);
                    }
                    else
                    {
                        localPlayerObj->m_X = remotePlayerObj->m_X;
                        localPlayerObj->m_Y = remotePlayerObj->m_Y;
                        localPlayerObj->m_Direction = remotePlayerObj->m_Direction;
                        RValue result;
                        auto mID = RValue(localPlayerObj->m_ID);
                        std::vector<RValue*> args = 
                        {
                            &mID
                        };
                        reviveScript->m_Functions->m_ScriptFunction(
                            Utils::GetGlobalInstance(),
                            Utils::GetGlobalInstance(),
                            result,
                            args.size(),
                            args.data()
                        );
                    }
                }
                if (ImGui::MenuItem("... at Spawn TP", "", false, isMultiplayer && !mpDead))
                {
                    CScript* reviveScript = GETSCR(scr_player_multiplayer_revive);

                    if (reviveScript == nullptr)
                    {
                        MessageBoxA(nullptr, "Script gml_Script_scr_player_multiplayer_revive not found", "Error", MB_OK | MB_ICONERROR);
                    }
                    else
                    {
                        int32_t tpObjID = Objects::ObjIndexes[Objects::obj_teleporter];
                        CInstance *spawnTP = CInstance::FirstOrDefault([tpObjID](CInstance* x) -> bool {
                            if (x->m_ObjectIndex == tpObjID)
                            {
                                return (x->InternalGetYYVarRef(VAR_HASH(endingteleporter)).ToBoolean());
                            }
                        });
                        localPlayerObj->m_X = 
                        localPlayerObj->m_Y = remotePlayerObj->m_Y;
                        localPlayerObj->m_Direction = remotePlayerObj->m_Direction;
                        RValue result;
                        auto mID = RValue(localPlayerObj->m_ID);
                        std::vector<RValue*> args = 
                        {
                            &mID
                        };
                        reviveScript->m_Functions->m_ScriptFunction(
                            Utils::GetGlobalInstance(),
                            Utils::GetGlobalInstance(),
                            result,
                            args.size(),
                            args.data()
                        );
                    }
                }
                if (ImGui::MenuItem("... on Partner", "", false, isMultiplayer && !mpDead))
                {
                    CScript* reviveScript = GETSCR(scr_player_multiplayer_revive);

                    if (reviveScript == nullptr)
                    {
                        MessageBoxA(nullptr, "Script gml_Script_scr_player_multiplayer_revive not found", "Error", MB_OK | MB_ICONERROR);
                    }
                    else
                    {
                        localPlayerObj->m_X = remotePlayerObj->m_X;
                        localPlayerObj->m_Y = remotePlayerObj->m_Y;
                        localPlayerObj->m_Direction = remotePlayerObj->m_Direction;
                        RValue result;
                        auto mID = RValue(localPlayerObj->m_ID);
                        std::vector<RValue*> args = 
                        {
                            &mID
                        };
                        reviveScript->m_Functions->m_ScriptFunction(
                            Utils::GetGlobalInstance(),
                            Utils::GetGlobalInstance(),
                            result,
                            args.size(),
                            args.data()
                        );
                    }
                }
            }
            ImGui::EndMenu(); // player
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
                        CScript* createScript = scr::ScriptPointers[
                            scr::Indexes[
                                scr::gml_Script_scr_instance_create
                            ]
                        ];
                        if (createScript == nullptr)
                        {
                            MessageBoxA(nullptr, "Script gml_Script_scr_instance_create not loaded", "Error", MB_OK | MB_ICONERROR);                                continue;
                            continue;
                        }
                        RValue result;
                        RValue posX = RValue(localPlayerObj->m_X);
                        RValue posY = RValue(localPlayerObj->m_Y);
                        RValue objIndexR = RValue(objIndex);
                        std::vector<RValue*> args = 
                        {
                            &posX,
                            &posY,
                            &objIndexR
                        };
                        createScript->m_Functions->m_ScriptFunction(
                            Utils::GetGlobalInstance(),
                            Utils::GetGlobalInstance(),
                            result,
                            args.size(),
                            args.data()
                        );
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
                            
                            objRoomCtrl->InternalGetYYVarRef(VAR_HASH(roomTransitionTarget)) = Organik::Rooms::AssetIndexes[i];
                            objRoomCtrl->InternalGetYYVarRef(VAR_HASH(roomTransitionRunning)) = RValue(1);
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
        if (ImGui::MenuItem("HG Slowdown Fix", "", g_HGSlowdownFix))
        {
            g_HGSlowdownFix = !g_HGSlowdownFix;
        }
        ImGui::EndMenu(); // Options/Bugfixes

    }
    ImGui::EndMainMenuBar();
}
