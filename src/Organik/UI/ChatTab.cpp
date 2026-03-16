#include "Synthetik.h"
#include "ChatTab.h"
#include "ChatBox.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "UIManager.h"
namespace Organik
{
    ChatTab::ChatTab(ChatTab&& other) noexcept
        : UIElement(std::move(other)), instanceID(other.instanceID), name(std::move(other.name)), inputBuffer(std::move(other.inputBuffer)), hasUnread(other.hasUnread)
    {
        std::unique_lock<std::shared_mutex> lock(other.messagesLock);
        messages.swap(other.messages);
        other.messages.clear();
    }
    ChatTab& ChatTab::operator=(ChatTab&& other) noexcept
    {
        inputBuffer.clear( );
        name.clear( );
        if (this != &other)
        {
            std::unique_lock<std::shared_mutex> lock1(selfLock, std::adopt_lock);
            std::unique_lock<std::shared_mutex> lock2(other.selfLock, std::adopt_lock);
            UIElement::operator=(std::move(other));
            instanceID = other.instanceID;
            other.name._Swap_data(name);
            other.inputBuffer._Swap_data(inputBuffer);
            hasUnread = other.hasUnread;

            std::unique_lock<std::shared_mutex> lock3(other.messagesLock);
            messages.swap(other.messages);
            other.messages.clear();
        }
        return *this;
    }
    void ChatTab::Draw(bool& out_mousedOver, bool* p_open)
    {
        ImGui::PushFont(Organik::UIManager::GetInstance()->GetConsoleFont());
        DO_LOG5("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);
        if (ImGui::BeginChild(ImGuiID(this), ImVec2(0.0, -24.0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding))
        {
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
            {
                out_mousedOver = true;
            }
            // Display messages 
            {
                std::shared_lock<std::shared_mutex> lock(messagesLock);
            for (const auto& message : messages)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImU32(message->color));
                ImGui::Text("%s: %s", message->name.c_str(), message->text.c_str());
                ImGui::PopStyleColor();
            }
            }
            if (this->GetUnreadFlag())
            {
                ImGui::SetScrollHereY(1.0f);
                this->SetUnreadFlag(false);
            }  
        }
        ImGui::EndChild();
        ImGui::PopFont();

        
    }
    void ChatTab::Clear()
    {
        std::unique_lock<std::shared_mutex> lock(messagesLock);
        for (auto& message : messages)
        {
            delete message;
        }
        messages.clear();
    }
    void ChatTab::Send(int32_t icon, const std::string& name, const std::string& message)
    {
        {
            std::unique_lock<std::shared_mutex> lock(messagesLock);
        Message* msg = new Message(icon, name, message);
        messages.push_back(std::move(msg));
        }

    }

    void ChatTab::Receive(int32_t icon, const std::string& name, const std::string& message)
    {
        {
            std::unique_lock<std::shared_mutex> lock(messagesLock);
        Message* msg = new Message(icon, name, message);
        messages.push_back(std::move(msg));
        }
        double soundID = static_cast<double>(DoBuiltin(gml_asset_get_index,"sound_chat_message").ToInt32());
        DoBuiltin(&gml_audio_play_sound, 
            soundID,
            18, // ???
            false // ??? 
        );
    }
}
