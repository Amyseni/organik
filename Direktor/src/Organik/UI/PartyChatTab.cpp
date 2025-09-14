#include "Synthetik.h"
#include "zhl_internal.h"
#include "PartyChatTab.h"
#include "ChatBox.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "DefinitionHelpers/InstanceHelper.h"

void Organik::PartyChatTab::Draw(bool& out_mousedOver, bool* p_open)
{
    ChatTab::Draw(out_mousedOver, p_open);
}
void Organik::PartyChatTab::Send(int32_t icon, const std::string& name, const std::string& message)
{
//     Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);
    uint32_t color = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (icon == 4)
    {
        color = ImGui::GetColorU32(ImVec4(0.0f, 0.6f, 0.85f, 1.0f));
    }
    if (icon > 5)
        color = *reinterpret_cast<uint32_t*>(&icon);

    std::unique_lock<std::mutex> lock(messagesLock);
    Message* msg = new Message(color, name, message);
    messages.push_back(msg);
    messagesLock.unlock();
    int32_t buffer = DoBuiltin(&gml_buffer_create, { 50, eBuffer_Grow, 1 }).ToInt32();
//     Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);
    RValue result;
    RValue nameR = RValue();
    RValue messageR = RValue();
    YYCreateString(&nameR, name.c_str());
    YYCreateString(&messageR, message.c_str());
    DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_U16, 41});
    DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_String, nameR });
    DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_String, messageR });
    DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_S32, icon });
//     Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);
    RValue buffR = RValue(buffer);
    RValue reliable = RValue(1ll);
    std::vector<RValue*> args =
    {
        &buffR,
        &reliable
    };
    double soundID = static_cast<double>(DoBuiltin(gml_asset_get_index, {RValue("sound_chat_message")}).ToInt32());
    DoBuiltin(&gml_audio_play_sound, {
        RValue(soundID),
        RValue(18), // ???
        RValue(false) // ???
    });
//     Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);
    this->SetUnreadFlag(true);
    gml_Script_scr_send_packet(GetGlobalInstance(), GetGlobalInstance(), &result, static_cast<int>(args.size()), args.data());
}
void Organik::PartyChatTab::Receive(int32_t icon, const std::string& name, const std::string& message)
{
    uint32_t color = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (icon == 4)
    {
        color = ImGui::GetColorU32(ImVec4(0.0f, 0.6f, 0.85f, 1.0f));
    }
    if (icon > 5)
        color = *reinterpret_cast<uint32_t*>(&icon);
    
    std::unique_lock<std::mutex> lock(messagesLock);
    Message* msg = new Message(color, name, message);
    messages.push_back(msg);
    messagesLock.unlock();
    double soundID = static_cast<double>(DoBuiltin(gml_asset_get_index, {RValue("sound_chat_message")}).ToInt32());
    DoBuiltin(&gml_audio_play_sound, {
        RValue(soundID),
        RValue(18), // ???
        RValue(false) // ???
    });
    this->SetUnreadFlag(true);
//     Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);
}