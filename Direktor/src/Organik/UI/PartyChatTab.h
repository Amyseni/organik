#pragma once
#include "Synthetik.h"
#include "ChatTab.h"

namespace Organik
{
    class PartyChatTab : public ChatTab
    {
    public:
        int32_t instanceID;
        PartyChatTab(int32_t instanceID) : ChatTab(instanceID, std::string("Party Chat").c_str())
            , instanceID(instanceID)
        {
        }
        ~PartyChatTab() = default;
        inline void GetName(char* buf) override
        {
            sprintf(buf, "Party Chat (%p)", reinterpret_cast<void*>((uintptr_t)typeid(PartyChatTab).hash_code()));
        }
        inline int32_t GetHash() override {
            static int32_t hash = static_cast<int32_t>(typeid(PartyChatTab).hash_code());
            return hash;
        }
        void Draw(bool& out_mousedOver, bool* p_open) override;
        void Send(int32_t icon, const std::string& name, const std::string& message) override;
        void Receive(int32_t icon, const std::string& name, const std::string& message) override;

        
    };
}