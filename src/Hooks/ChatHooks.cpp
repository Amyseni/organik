#include "Synthetik.h"
#include "Organik/UI/UIManager.h"
#include "Organik/UI/ChatBox.h"
#include "Organik/UI/PartyChatTab.h"
#include "Organik/UI/GlobalChatTab.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "zhl.h" 
static int32_t packetsToHandleDSList = 0;
static bool chatExists = false;
HOOK_EVENT(obj_chat_parent, EVENT_CREATE, 0)
{
    super(self, other);
    
    ChatBox::GetInstance();
    chatExists = true;
}

PartyChatTab* pcTab = nullptr;
GlobalChatTab* gcTab = nullptr;
bool ChatParentCreate(CInstance* self, CInstance* other, YYObjectBase* Result)
{
    if (!chatExists)
    {
        ChatBox::GetInstance();
        chatExists = true;
    }
    return true;
}
HOOK_EVENT(obj_net_chat, EVENT_CREATE, 0)
{
    if (ImGui::GetCurrentContext())
        GetLogger()->LogFormatted("%s: net_chat_Create called", __FILE__);
    
    super(self, other);

    if (self)
    {
        GetLogger()->LogFormatted("Creating PartyChatTab with instance ID: %d", self->m_ID);
        pcTab = new PartyChatTab(self->m_ID);
        GetLogger()->LogFormatted("PartyChatTab created successfully");
        ChatBox::GetInstance()->AddTab(pcTab);
        GetLogger()->LogFormatted("PartyChatTab added to ChatBox");
        RValue* show = self->InternalReadYYVar(VAR_HASH(show));
        if (show)
            *show = RValue(false);
        return;
    }
}

HOOK_EVENT(obj_net_global_chat, EVENT_CREATE, 0)
{
    if (ImGui::GetCurrentContext())
        GetLogger()->LogFormatted("%s: global_chat_Create called", __FILE__);
    super(self, other);
    if (self)
    {
        gcTab = new GlobalChatTab(self->m_ID);
        ChatBox::GetInstance()->AddTab(gcTab);
        RValue* show = self->InternalReadYYVar(VAR_HASH(show));
        if (show)
            *show = RValue(false);
    }
}
HOOK_GLOBAL(gml_Script_scr_send_MS_packet, (CInstance* self, CInstance* other, RValue* Result, int ArgumentCount, RValue** Arguments) -> RValue*)
{
    Result = super(self, other, Result, ArgumentCount, Arguments);
    if (!(ImGui::GetCurrentContext()) || (packetsToHandleDSList != 0))
        return Result;

    CInstance* MSClient = CInstance::FirstOrDefault(
        [](CInstance* ci) -> bool {
            return ci->m_ObjectIndex == Organik::Objects::ObjIndexes[Organik::Objects::obj_net_MS_client];
        }
    );
    if (!MSClient)
    {
        return Result; // No MSClient instance found
    }

    RValue* listID = MSClient->InternalReadYYVar(VAR_HASH(packetsToHandle));
    if (!listID)
    {
        return Result; // No packetsToHandle list found
    }
    packetsToHandleDSList = static_cast<int32_t>(listID->ToDouble());
    Organik::GetLogger()->LogFormatted("Packets to handle DS list set to: %d", packetsToHandleDSList);
    return Result;
}
void ChatAddHooks()
{
    return;
}
// arguments:
// 0: Some weird timestamp - don't care
// 1: char* name (of sender)
// 2: char* message 
// 3: Icon to use 
// 4: remoteAdd (send network packets)
HOOK_SCRIPT(scr_add_chat_entry)
{
    // Ensure the chat box is initialized
    ChatBox* chatBox = ChatBox::GetInstance();
    if (!chatBox)
        return Result;
//    Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);

    // Get the sender's name from the arguments
    std::string senderName = (Arguments[1])->ToString();
    // Get the message from the arguments
    std::string message = (Arguments[2])->ToString();
    int32_t icon = 0;
//    Organik::GetLogger()->LogFormatted("%s(%s): #%d", __FILE__, __FUNCTION__, __LINE__);

    if (ArgumentCount > 3)
    {
        // Get the icon from the arguments, if provided
        icon = (Arguments[3])->ToInt32();
    }
    if (ArgumentCount > 4)
    {
        bool remoteAdd = (Arguments[4])->ToBoolean();
        if (!remoteAdd)
        {
            auto [pcTabLock, partyChat] = ChatBox::GetInstance()->GetTab<Organik::PartyChatTab>();
            if (partyChat)
            {
                partyChat->Send(icon, senderName, message);
            }
            pcTabLock.unlock();
            return Result;
        }
    }
    auto [pcTabLock, partyChat] = ChatBox::GetInstance()->GetTab<Organik::PartyChatTab>();
    if (partyChat)
    {
        partyChat->Receive(icon, senderName, message);
    }
    pcTabLock.unlock();
    return Result;
}
HOOK_GLOBAL(gml_ds_list_add, (CInstance* self, CInstance* other, RValue* Result, int ArgumentCount, RValue* Arguments) -> void)
{
    if (!packetsToHandleDSList)
    {
        super(self, other, Result, ArgumentCount, Arguments);
        return; // No packets to handle
    }
    int32_t listID = YYGetInt32(Arguments, 0);
    if (listID != packetsToHandleDSList)
    {
        super(self, other, Result, ArgumentCount, Arguments);
        return; // Not the packetsToHandle list
    }
    // Handle the packet
    int32_t bufferID = static_cast<int32_t>(YYGetReal(Arguments, 1));
    RValue resultUnused = RValue();
    DoBuiltin(&gml_buffer_seek, { bufferID, eBuffer_Seek::eBuffer_Start, 0});
    int32_t packetType = DoBuiltin(&gml_buffer_read, { bufferID, eBuffer_U8 }).ToInt32();

    if (packetType > 4)
    {
        if (DoBuiltin(&gml_buffer_get_size, { bufferID }).ToInt32() < 5)
        {
            DoBuiltin(&gml_buffer_delete, { bufferID });
            return;
        }
        //? don't forget to throw away 4 bytes for no discernable reason
        DoBuiltin(&gml_buffer_read, { bufferID, eBuffer_S32 }); 
    }
    switch(packetType)
    {
        case 35:
        {
            RValue nameR = RValue();
            DoBuiltinRef(&gml_buffer_read, nameR, { bufferID, eBuffer_String });
            
            //? No one was using this string, right? :)
            DoBuiltin(&gml_buffer_read, { bufferID, eBuffer_String }); 
            
            RValue timeR = DoBuiltin(&gml_buffer_read, { bufferID, eBuffer_D64 });
            RValue messageR = RValue();
            DoBuiltinRef(&gml_buffer_read, messageR, { bufferID, eBuffer_String });
            RValue iconR = DoBuiltin(&gml_buffer_read, { bufferID, eBuffer_S32 });
            std::string name = nameR.ToCString();
            std::string message = messageR.ToCString();
            int32_t icon = iconR.ToInt32();

            auto [gcTabLock, globalChat] = ChatBox::GetInstance()->GetTab<GlobalChatTab>();
            if (globalChat)
            {
                globalChat->Receive(icon, name, message);
                gcTabLock.unlock();
            }
            super(self, other, Result, ArgumentCount, Arguments);
            return;
        }
        default:
            break;

    }
    super(self, other, Result, ArgumentCount, Arguments);
}
