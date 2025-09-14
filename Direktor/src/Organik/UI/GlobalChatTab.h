#pragma once
#include "Synthetik.h"
#include "zhl_internal.h"
#include "ChatTab.h"


namespace Organik
{
    class GlobalChatTab : public ChatTab
    {
        const char* ret;
    public:
        int32_t instanceID;
        GlobalChatTab(const GlobalChatTab& other) = delete;
        GlobalChatTab& operator=(const GlobalChatTab& other) = delete;
        GlobalChatTab(GlobalChatTab&& other) noexcept;
        GlobalChatTab&& operator=(GlobalChatTab&& other) noexcept;
        ~GlobalChatTab() override = default;

        GlobalChatTab(int32_t instanceID) : ChatTab(instanceID, "Global Chat")
        {
            this->instanceID = instanceID;
        }

        inline void GetName(char* buf) override
        {
            sprintf(buf, "Global Chat (%d)", this->instanceID);
        };

        int32_t GetHash() override
        {
            static int32_t hash = static_cast<int32_t>(typeid(GlobalChatTab).hash_code());
            return hash;
        }

        void Draw(bool& out_mousedOver, bool* p_open) override;
        void Send(int32_t icon, const std::string& name, const std::string& message) override;
        void Receive(int32_t icon, const std::string& name, const std::string& message) override;
    };
}