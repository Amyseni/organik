#include "Synthetik.h"
#include "zhl_internal.h"
#include "ChatBox.h"
#include "ChatTab.h"
#include "GlobalChatTab.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "DefinitionHelpers/InstanceHelper.h"
#include "Organik/UI/UIManager.h"
#include <vector>
#include <string>
bool Organik::ChatBox::g_ShowGlobalChat = false;
bool Organik::ChatBox::g_CloseGlobalChat = false;
bool Organik::ChatBox::g_GrabInput = false;
namespace Organik
{   
    
    class PartyChatTab : public ChatTab {};
    ChatBox::ChatBox() : UIElement("ChatBox")
    {
        this->name = "ChatBox";
    }
    ChatBox::ChatBox(const std::string& name) : UIElement(name)
    {
        this->name = name;
    }
    
    ChatBox::~ChatBox()
    {
        for (auto& tab : tabs)
        {
            tab.second.reset();
        }
        tabs.clear();
    }
    
    void ChatBox::Draw(bool& out_mousedOver, bool* p_open, const std::string &title)
    {
        ImGui::SetNextWindowSize(ImVec2(600, 350), ImGuiCond_FirstUseEver);
        bool open = true;
        if (!g_ShowGlobalChat)
            return;
        if (!ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar ))
        {
            ImGui::End();
            return;
        }
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |  ImGuiHoveredFlags_ChildWindows))
        {
            out_mousedOver = true;
        }
        if (ImGui::BeginTabBar("##chatTabs", 
            ImGuiTabBarFlags_Reorderable | 
            ImGuiTabBarFlags_AutoSelectNewTabs | 
            ImGuiTabBarFlags_TabListPopupButton | 
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | 
            ImGuiTabBarFlags_DrawSelectedOverline | 
            ImGuiTabBarFlags_FittingPolicyScroll
        ))
        {
            std::lock_guard<std::mutex> lock(tabsListLock);
            ChatTab* activeTab = nullptr;
            std::unique_lock<std::mutex> tabLock;
            char *name = new char[256];
            for (auto& [hash, p_Tab] : tabs)
            {
                bool stayOpen = true;
                memset(name, 0, 256);
                p_Tab->GetName(&name[0]);
                if (ImGui::BeginTabItem(&name[0], &stayOpen, ImGuiTabItemFlags_NoPushId ))
                {
                    p_Tab->Draw(out_mousedOver, &stayOpen);
                    activeTab = p_Tab.get();
                    ImGui::EndTabItem();
                }
            }
            if (activeTab)
            {
                if(ImGui::InputTextWithHint("##chatInput", "Type your message here...", &activeTab->GetInputBuffer(), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll ))
                {
                    if (!activeTab->GetInputBuffer().empty())
                    {
                        activeTab->Send(4, DoBuiltin(&gml_steam_get_persona_name, {}).ToCString(), activeTab->GetInputBuffer());
                        activeTab->GetInputBuffer().clear();
                        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
                    }
                    else
                    {
                        Organik::ChatBox::g_CloseGlobalChat = true;
                    }
                }
                if (ImGui::IsKeyDown(ImGuiKey_Escape))
                {
                    Organik::ChatBox::g_CloseGlobalChat = true;
                }
                if (Organik::ChatBox::g_GrabInput)
                {
                    ImGui::SetKeyboardFocusHere(-1);
                    Organik::ChatBox::g_GrabInput = false;
                }
            }

            delete[] name;
            
            
            ImGui::EndTabBar();
        }
        if (out_mousedOver)
        {
            UIManager::GetInstance()->setHovered();
        }

        ImGui::End();
    }
}
