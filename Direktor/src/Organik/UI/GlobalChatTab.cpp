#include "Synthetik.h"
#include "zhl_internal.h"
#include "GlobalChatTab.h"
#include "ChatBox.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "DefinitionHelpers/InstanceHelper.h"

namespace Organik {
    void GlobalChatTab::Draw(bool& out_mousedOver, bool* p_open)
    {
        ChatTab::Draw(out_mousedOver, p_open);
    }
    void GlobalChatTab::Send(int32_t icon, const std::string& name, const std::string& message)
    {
        int32_t buffer = DoBuiltin(&gml_buffer_create, { 20, eBuffer_Grow, 1 }).ToInt32();
        RValue result;
        RValue nameR = RValue();
        RValue messageR = RValue();
        RValue playerID = RValue();
        RValue networkTime = RValue(std::chrono::system_clock::now().time_since_epoch().count());
        YYCreateString(&nameR, name.c_str());
        YYCreateString(&messageR, message.c_str());
        YYCreateString(&playerID, "696969");

        DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_U8, 35 });
        DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_String, nameR });
        DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_String, playerID });
        DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_D64, 0.0 });
        DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_String, messageR });
        DoBuiltinRef(&gml_buffer_write, result, { buffer, eBuffer_S32, icon });
        RValue buffR = RValue(buffer);
        std::vector<RValue*> args =
        {
            &buffR,
        };
        
        gml_Script_scr_send_MS_packet(GetGlobalInstance(), GetGlobalInstance(), &result, static_cast<int>(args.size()), args.data());
        
    }
    void GlobalChatTab::Receive(int32_t icon, const std::string& name, const std::string& message)
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
        lock.unlock();
        double soundID = static_cast<double>(DoBuiltin(gml_asset_get_index, {RValue("sound_chat_message")}).ToInt32());
        DoBuiltin(&gml_audio_play_sound, {
            RValue(soundID), 
            RValue(18), // ???
            RValue(false) // ??? 
        });
        this->SetUnreadFlag(true);
    }
}
